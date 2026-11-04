/**
 * @file FinanceAndShop.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов финансов, кассы, грантов, сувенирной лавки и комплекса Музея.
 */

#include "FinanceAndShop.h"

#include <algorithm>
#include <sstream>

namespace {
constexpr double kLargePatronMinAmount = 10000.0;
constexpr double kPlatinumShopDiscount = 0.20;
constexpr double kGoldShopDiscount = 0.15;
constexpr double kSilverShopDiscount = 0.10;
} // namespace

MuseumBudget::MuseumBudget(int fiscalYear, double initialBalance, double stateSubsidy)
    : fiscalYear_(fiscalYear),
      currentBalance_(std::max(0.0, initialBalance)),
      totalRevenue_(0.0),
      totalExpenses_(0.0),
      stateSubsidy_(std::max(0.0, stateSubsidy)) {}

void MuseumBudget::receiveIncome(double amount) {
    if (amount > 0.0) {
        currentBalance_ += amount;
        totalRevenue_ += amount;
    }
}

void MuseumBudget::allocateExpense(double amount) {
    if (amount < 0.0 || amount > currentBalance_) {
        throw InsufficientFundsException(amount, currentBalance_);
    }
    currentBalance_ -= amount;
    totalExpenses_ += amount;
}

void MuseumBudget::applyStateSubsidy() {
    receiveIncome(stateSubsidy_);
}

double MuseumBudget::calculateNetBalance() const {
    return totalRevenue_ - totalExpenses_;
}

TicketOffice::TicketOffice(const std::string& officeId, const DiscountPolicy& discountPolicy)
    : officeId_(officeId),
      cashInRegister_(0.0),
      soldTicketsCount_(0),
      discountPolicy_(discountPolicy) {}

Ticket TicketOffice::sellTicket(Visitor& visitor,
                                const Exhibition& exhibition,
                                ExhibitionHall& hall,
                                MuseumBudget& budget) {
    if (!exhibition.isOpen()) {
        throw ExhibitionClosedException(exhibition.title());
    }
    hall.admitVisitors(1);
    double disc = discountPolicy_.calculateDiscountPercent(
        visitor.age(), visitor.isStudent(), visitor.hasMembership());
    double price = discountPolicy_.applyDiscount(exhibition.baseTicketPrice(), disc);
    visitor.payAmount(price);
    cashInRegister_ += price;
    budget.receiveIncome(price);
    ++soldTicketsCount_;
    std::string ticketNo = "TCK-" + std::to_string(soldTicketsCount_);
    return Ticket(ticketNo, visitor.visitorId(), exhibition.exhibitionId(), price);
}

void TicketOffice::processTicketRefund(Ticket& ticket, Visitor& visitor, MuseumBudget& budget) {
    double amount = ticket.finalPrice();
    budget.allocateExpense(amount);
    ticket.refundTicket(visitor);
    cashInRegister_ = std::max(0.0, cashInRegister_ - amount);
}

void TicketOffice::transferCashToBudget(MuseumBudget& budget) {
    if (cashInRegister_ > 0.0) {
        budget.receiveIncome(cashInRegister_);
        cashInRegister_ = 0.0;
    }
}

DonationGrant::DonationGrant(const std::string& grantId,
                             const std::string& sponsorName,
                             double amount,
                             const std::string& targetPurpose)
    : grantId_(grantId),
      sponsorName_(sponsorName),
      amount_(std::max(0.0, amount)),
      targetPurpose_(targetPurpose),
      isTransferred_(false) {}

void DonationGrant::transferToBudget(MuseumBudget& budget) {
    if (isTransferred_) {
        throw InvalidTicketException("Grant already transferred: " + grantId_);
    }
    budget.receiveIncome(amount_);
    isTransferred_ = true;
}

bool DonationGrant::canFundRestoration(const RestorationTask& task) const {
    return amount_ >= task.estimatedCost();
}

bool DonationGrant::isLargePatron() const {
    return amount_ >= kLargePatronMinAmount;
}

SouvenirItem::SouvenirItem(const std::string& skuCode,
                           const std::string& itemName,
                           double unitPrice,
                           int stockQuantity,
                           const std::string& relatedExhibitionId)
    : skuCode_(skuCode),
      itemName_(itemName),
      unitPrice_(std::max(0.0, unitPrice)),
      stockQuantity_(std::max(0, stockQuantity)),
      relatedExhibitionId_(relatedExhibitionId) {}

void SouvenirItem::sellUnits(int count) {
    if (count <= 0 || count > stockQuantity_) {
        throw TicketSoldOutException("Souvenir " + itemName_);
    }
    stockQuantity_ -= count;
}

void SouvenirItem::restockUnits(int count) {
    if (count > 0) {
        stockQuantity_ += count;
    }
}

double SouvenirItem::applyMemberDiscountPrice(const MembershipCard& card) const {
    if (!card.isActive()) {
        return unitPrice_;
    }
    if (card.tierName() == "Platinum") {
        return unitPrice_ * (1.0 - kPlatinumShopDiscount);
    }
    if (card.tierName() == "Gold") {
        return unitPrice_ * (1.0 - kGoldShopDiscount);
    }
    return unitPrice_ * (1.0 - kSilverShopDiscount);
}

GiftShop::GiftShop(const std::string& shopName)
    : shopName_(shopName), dailySales_(0.0), transactionsCount_(0), inventory_() {}

void GiftShop::addSouvenirItem(const SouvenirItem& item) {
    inventory_.push_back(item);
}

double GiftShop::purchaseSouvenir(Visitor& visitor,
                                  const std::string& skuCode,
                                  int quantity,
                                  MuseumBudget& budget) {
    for (SouvenirItem& item : inventory_) {
        if (item.skuCode() == skuCode) {
            double totalCost = item.unitPrice() * static_cast<double>(quantity);
            visitor.payAmount(totalCost);
            item.sellUnits(quantity);
            dailySales_ += totalCost;
            ++transactionsCount_;
            budget.receiveIncome(totalCost);
            return totalCost;
        }
    }
    throw ArtifactNotFoundException(skuCode);
}

double GiftShop::calculateInventoryValue() const {
    double sum = 0.0;
    for (const SouvenirItem& item : inventory_) {
        sum += item.unitPrice() * static_cast<double>(item.stockQuantity());
    }
    return sum;
}

Museum::Museum(const std::string& museumName, int foundedYear, const std::string& city)
    : museumName_(museumName),
      foundedYear_(foundedYear),
      city_(city),
      catalog_(museumName + " Catalog", 2026),
      vault_("MAIN-VAULT", 200, 3),
      budget_(2026, 150000.0, 50000.0),
      hrDepartment_("HR Directorate", 100),
      securityCenter_("SEC-MAIN"),
      halls_() {}

void Museum::addExhibitionHall(const ExhibitionHall& hall) {
    halls_.push_back(hall);
}

void Museum::acquireNewArtifact(Artifact& artifact,
                                int targetHallNumber,
                                const std::string& description) {
    vault_.depositArtifact(artifact);
    catalog_.registerArtifact(artifact, targetHallNumber, description);
}

void Museum::payMonthlyStaffSalaries() {
    double payroll = hrDepartment_.calculateTotalPayroll();
    budget_.allocateExpense(payroll);
}

int Museum::calculateTotalVisitorCapacity() const {
    int total = 0;
    for (const ExhibitionHall& hall : halls_) {
        total += hall.maxVisitors();
    }
    return total;
}

std::string Museum::generateAnnualSummaryReport() const {
    std::ostringstream oss;
    oss << "Музей: " << museumName_ << " (" << city_ << ", осн. " << foundedYear_ << ")\n"
        << "Залов: " << halls_.size()
        << " | Экспонатов в каталоге: " << catalog_.totalEntries()
        << " | В фондохранилище: " << vault_.storedCount()
        << " | Штат: " << hrDepartment_.staffCount()
        << " | Баланс бюджета: " << budget_.currentBalance();
    return oss.str();
}
