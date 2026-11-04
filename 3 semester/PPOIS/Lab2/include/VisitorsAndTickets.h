/**
 * @file VisitorsAndTickets.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы посетителей, билетов, льготной политики, карт лояльности, гардероба и отзывов.
 * @details Содержит 6 классов: DiscountPolicy, Visitor, Ticket,
 * MembershipCard, CloakroomTag, VisitorFeedback.
 */

#ifndef MUSEUM_VISITORS_AND_TICKETS_H
#define MUSEUM_VISITORS_AND_TICKETS_H

#include "MuseumExceptions.h"
#include <string>

/**
 * @class DiscountPolicy
 * @brief Политика расчёта скидок и льгот на билеты музея.
 */
class DiscountPolicy {
public:
    DiscountPolicy(double studentDiscount = 0.30,
                   double seniorDiscount = 0.40,
                   double childDiscount = 0.50,
                   int groupThreshold = 10,
                   double groupDiscount = 0.20);

    double calculateDiscountPercent(int visitorAge, bool isStudent, bool isMember) const;
    double applyGroupDiscount(double totalAmount, int groupSize) const;
    double applyDiscount(double basePrice, double discountFraction) const;

private:
    double studentDiscount_;
    double seniorDiscount_;
    double childDiscount_;
    int groupThreshold_;
    double groupDiscount_;
};

/**
 * @class Visitor
 * @brief Посетитель музея.
 */
class Visitor {
public:
    Visitor(const std::string& visitorId,
            const std::string& fullName,
            int age,
            bool isStudent,
            double walletBalance,
            bool hasMembership = false);

    void payAmount(double amount);
    void topUpWallet(double amount);
    bool isEligibleForConcession() const;

    const std::string& visitorId() const noexcept { return visitorId_; }
    const std::string& fullName() const noexcept { return fullName_; }
    int age() const noexcept { return age_; }
    bool isStudent() const noexcept { return isStudent_; }
    double walletBalance() const noexcept { return walletBalance_; }
    bool hasMembership() const noexcept { return hasMembership_; }

private:
    std::string visitorId_;
    std::string fullName_;
    int age_;
    bool isStudent_;
    double walletBalance_;
    bool hasMembership_;
};

/**
 * @class Ticket
 * @brief Входной билет на выставку музея.
 */
class Ticket {
public:
    Ticket(const std::string& ticketNumber,
           const std::string& visitorId,
           const std::string& exhibitionId,
           double finalPrice);

    void validateAndUse(const std::string& targetExhibitionId);
    void refundTicket(Visitor& visitor);
    bool isValidForExhibition(const std::string& targetExhibitionId) const;

    const std::string& ticketNumber() const noexcept { return ticketNumber_; }
    double finalPrice() const noexcept { return finalPrice_; }
    bool isUsed() const noexcept { return isUsed_; }
    bool isRefunded() const noexcept { return isRefunded_; }

private:
    std::string ticketNumber_;
    std::string visitorId_;
    std::string exhibitionId_;
    double finalPrice_;
    bool isUsed_;
    bool isRefunded_;
};

/**
 * @class MembershipCard
 * @brief Клубная карта «Друг музея» с накопительной бонусной системой.
 */
class MembershipCard {
public:
    MembershipCard(const std::string& cardNumber,
                   const std::string& holderVisitorId,
                   const std::string& tierName = "Silver",
                   int loyaltyPoints = 0);

    void accruePoints(double spentAmount);
    double redeemPoints(int pointsToUse);
    void upgradeTierIfEligible();
    void deactivateCard();

    const std::string& cardNumber() const noexcept { return cardNumber_; }
    const std::string& tierName() const noexcept { return tierName_; }
    int loyaltyPoints() const noexcept { return loyaltyPoints_; }
    bool isActive() const noexcept { return isActive_; }

private:
    std::string cardNumber_;
    std::string holderVisitorId_;
    std::string tierName_;
    int loyaltyPoints_;
    bool isActive_;
};

/**
 * @class CloakroomTag
 * @brief Номерок музейного гардероба / ячейки хранения.
 */
class CloakroomTag {
public:
    explicit CloakroomTag(int tagNumber = 1);

    void acceptItems(const Visitor& visitor, int count);
    void releaseItems(const Visitor& visitor);
    void clearTag();

    int tagNumber() const noexcept { return tagNumber_; }
    int itemsCount() const noexcept { return itemsCount_; }
    bool isOccupied() const noexcept { return isOccupied_; }

private:
    int tagNumber_;
    std::string visitorId_;
    int itemsCount_;
    bool isOccupied_;
};

/**
 * @class VisitorFeedback
 * @brief Запись в книге отзывов и предложений музея.
 */
class VisitorFeedback {
public:
    VisitorFeedback(const std::string& feedbackId,
                    const std::string& visitorName,
                    const std::string& exhibitionId,
                    int ratingStars,
                    const std::string& comment);

    bool isPositive() const;
    bool requiresManagerAttention() const;
    std::string formatSummary() const;

    int ratingStars() const noexcept { return ratingStars_; }
    const std::string& exhibitionId() const noexcept { return exhibitionId_; }

private:
    std::string feedbackId_;
    std::string visitorName_;
    std::string exhibitionId_;
    int ratingStars_;
    std::string comment_;
};

#endif // MUSEUM_VISITORS_AND_TICKETS_H
