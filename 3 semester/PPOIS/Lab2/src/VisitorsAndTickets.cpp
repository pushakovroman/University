/**
 * @file VisitorsAndTickets.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов посетителей, билетов, скидок, абонементов, гардероба и отзывов.
 */

#include "VisitorsAndTickets.h"

#include <algorithm>
#include <sstream>

namespace {
constexpr int kChildMaxAge = 14;
constexpr int kSeniorMinAge = 60;
constexpr double kMemberAdditionalDiscount = 0.15;
constexpr double kMaxAllowedDiscount = 0.70;
constexpr int kPointsPerCurrencyUnit = 5;
constexpr double kCurrencyPerPoint = 0.10;
constexpr int kGoldTierThreshold = 500;
constexpr int kPlatinumTierThreshold = 1500;
} // namespace

DiscountPolicy::DiscountPolicy(double studentDiscount,
                               double seniorDiscount,
                               double childDiscount,
                               int groupThreshold,
                               double groupDiscount)
    : studentDiscount_(studentDiscount),
      seniorDiscount_(seniorDiscount),
      childDiscount_(childDiscount),
      groupThreshold_(groupThreshold),
      groupDiscount_(groupDiscount) {}

double DiscountPolicy::calculateDiscountPercent(int visitorAge, bool isStudent, bool isMember) const {
    double discount = 0.0;
    if (visitorAge <= kChildMaxAge) {
        discount = childDiscount_;
    } else if (visitorAge >= kSeniorMinAge) {
        discount = seniorDiscount_;
    } else if (isStudent) {
        discount = studentDiscount_;
    }
    if (isMember) {
        discount += kMemberAdditionalDiscount;
    }
    return std::min(kMaxAllowedDiscount, discount);
}

double DiscountPolicy::applyGroupDiscount(double totalAmount, int groupSize) const {
    if (groupSize >= groupThreshold_) {
        return totalAmount * (1.0 - groupDiscount_);
    }
    return totalAmount;
}

double DiscountPolicy::applyDiscount(double basePrice, double discountFraction) const {
    double clamped = std::clamp(discountFraction, 0.0, kMaxAllowedDiscount);
    return basePrice * (1.0 - clamped);
}

Visitor::Visitor(const std::string& visitorId,
                 const std::string& fullName,
                 int age,
                 bool isStudent,
                 double walletBalance,
                 bool hasMembership)
    : visitorId_(visitorId),
      fullName_(fullName),
      age_(age),
      isStudent_(isStudent),
      walletBalance_(walletBalance),
      hasMembership_(hasMembership) {}

void Visitor::payAmount(double amount) {
    if (amount > walletBalance_) {
        throw InsufficientFundsException(amount, walletBalance_);
    }
    walletBalance_ -= std::max(0.0, amount);
}

void Visitor::topUpWallet(double amount) {
    if (amount > 0.0) {
        walletBalance_ += amount;
    }
}

bool Visitor::isEligibleForConcession() const {
    return age_ <= kChildMaxAge || age_ >= kSeniorMinAge || isStudent_;
}

Ticket::Ticket(const std::string& ticketNumber,
               const std::string& visitorId,
               const std::string& exhibitionId,
               double finalPrice)
    : ticketNumber_(ticketNumber),
      visitorId_(visitorId),
      exhibitionId_(exhibitionId),
      finalPrice_(finalPrice),
      isUsed_(false),
      isRefunded_(false) {}

void Ticket::validateAndUse(const std::string& targetExhibitionId) {
    if (!isValidForExhibition(targetExhibitionId)) {
        throw InvalidTicketException(ticketNumber_);
    }
    isUsed_ = true;
}

void Ticket::refundTicket(Visitor& visitor) {
    if (isUsed_ || isRefunded_) {
        throw InvalidTicketException(ticketNumber_);
    }
    isRefunded_ = true;
    visitor.topUpWallet(finalPrice_);
}

bool Ticket::isValidForExhibition(const std::string& targetExhibitionId) const {
    return !isUsed_ && !isRefunded_ && exhibitionId_ == targetExhibitionId;
}

MembershipCard::MembershipCard(const std::string& cardNumber,
                               const std::string& holderVisitorId,
                               const std::string& tierName,
                               int loyaltyPoints)
    : cardNumber_(cardNumber),
      holderVisitorId_(holderVisitorId),
      tierName_(tierName),
      loyaltyPoints_(std::max(0, loyaltyPoints)),
      isActive_(true) {}

void MembershipCard::accruePoints(double spentAmount) {
    if (!isActive_ || spentAmount <= 0.0) {
        return;
    }
    loyaltyPoints_ += static_cast<int>(spentAmount) * kPointsPerCurrencyUnit;
    upgradeTierIfEligible();
}

double MembershipCard::redeemPoints(int pointsToUse) {
    if (!isActive_ || pointsToUse <= 0 || pointsToUse > loyaltyPoints_) {
        throw InvalidTicketException("MembershipCard " + cardNumber_);
    }
    loyaltyPoints_ -= pointsToUse;
    return static_cast<double>(pointsToUse) * kCurrencyPerPoint;
}

void MembershipCard::upgradeTierIfEligible() {
    if (loyaltyPoints_ >= kPlatinumTierThreshold) {
        tierName_ = "Platinum";
    } else if (loyaltyPoints_ >= kGoldTierThreshold) {
        tierName_ = "Gold";
    }
}

void MembershipCard::deactivateCard() {
    isActive_ = false;
}

CloakroomTag::CloakroomTag(int tagNumber)
    : tagNumber_(tagNumber), visitorId_(""), itemsCount_(0), isOccupied_(false) {}

void CloakroomTag::acceptItems(const Visitor& visitor, int count) {
    if (isOccupied_ || count <= 0) {
        throw HallOvercapacityException("CloakroomTag #" + std::to_string(tagNumber_), 1);
    }
    visitorId_ = visitor.visitorId();
    itemsCount_ = count;
    isOccupied_ = true;
}

void CloakroomTag::releaseItems(const Visitor& visitor) {
    if (!isOccupied_ || visitorId_ != visitor.visitorId()) {
        throw SecurityBreachException("Cloakroom tag mismatch for " + visitor.fullName());
    }
    clearTag();
}

void CloakroomTag::clearTag() {
    visitorId_.clear();
    itemsCount_ = 0;
    isOccupied_ = false;
}

VisitorFeedback::VisitorFeedback(const std::string& feedbackId,
                                 const std::string& visitorName,
                                 const std::string& exhibitionId,
                                 int ratingStars,
                                 const std::string& comment)
    : feedbackId_(feedbackId),
      visitorName_(visitorName),
      exhibitionId_(exhibitionId),
      ratingStars_(std::clamp(ratingStars, 1, 5)),
      comment_(comment) {}

bool VisitorFeedback::isPositive() const {
    return ratingStars_ >= 4;
}

bool VisitorFeedback::requiresManagerAttention() const {
    return ratingStars_ <= 2;
}

std::string VisitorFeedback::formatSummary() const {
    std::ostringstream oss;
    oss << visitorName_ << " [" << ratingStars_ << "/5]: " << comment_;
    return oss.str();
}
