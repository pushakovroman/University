/**
 * @file FinanceAndShop.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы финансов, кассы, грантов, сувенирного магазина и главного комплекса Музея.
 * @details Содержит 6 классов: MuseumBudget, TicketOffice, DonationGrant,
 * SouvenirItem, GiftShop, Museum.
 */

#ifndef MUSEUM_FINANCE_AND_SHOP_H
#define MUSEUM_FINANCE_AND_SHOP_H

#include "Artifacts.h"
#include "ExhibitionsAndEvents.h"
#include "HallsAndClimate.h"
#include "MuseumExceptions.h"
#include "RestorationLab.h"
#include "SecuritySystem.h"
#include "StaffAndHR.h"
#include "StorageAndLogistics.h"
#include "VisitorsAndTickets.h"
#include <string>
#include <vector>

/**
 * @class MuseumBudget
 * @brief Финансовый бюджет и расчётный счёт музея.
 */
class MuseumBudget {
public:
    MuseumBudget(int fiscalYear = 2026,
                 double initialBalance = 100000.0,
                 double stateSubsidy = 50000.0);

    void receiveIncome(double amount);
    void allocateExpense(double amount);
    void applyStateSubsidy();
    double calculateNetBalance() const;

    double currentBalance() const noexcept { return currentBalance_; }
    double totalRevenue() const noexcept { return totalRevenue_; }
    double totalExpenses() const noexcept { return totalExpenses_; }

private:
    int fiscalYear_;
    double currentBalance_;
    double totalRevenue_;
    double totalExpenses_;
    double stateSubsidy_;
};

/**
 * @class TicketOffice
 * @brief Билетная касса музея.
 */
class TicketOffice {
public:
    TicketOffice(const std::string& officeId = "BOX-01",
                 const DiscountPolicy& discountPolicy = DiscountPolicy());

    Ticket sellTicket(Visitor& visitor,
                      const Exhibition& exhibition,
                      ExhibitionHall& hall,
                      MuseumBudget& budget);
    void processTicketRefund(Ticket& ticket, Visitor& visitor, MuseumBudget& budget);
    void transferCashToBudget(MuseumBudget& budget);

    int soldTicketsCount() const noexcept { return soldTicketsCount_; }
    double cashInRegister() const noexcept { return cashInRegister_; }

private:
    std::string officeId_;
    double cashInRegister_;
    int soldTicketsCount_;
    DiscountPolicy discountPolicy_;
};

/**
 * @class DonationGrant
 * @brief Благотворительный грант или спонсорский взнос мецената.
 */
class DonationGrant {
public:
    DonationGrant(const std::string& grantId,
                  const std::string& sponsorName,
                  double amount,
                  const std::string& targetPurpose);

    void transferToBudget(MuseumBudget& budget);
    bool canFundRestoration(const RestorationTask& task) const;
    bool isLargePatron() const;

    const std::string& grantId() const noexcept { return grantId_; }
    double amount() const noexcept { return amount_; }
    bool isTransferred() const noexcept { return isTransferred_; }

private:
    std::string grantId_;
    std::string sponsorName_;
    double amount_;
    std::string targetPurpose_;
    bool isTransferred_;
};

/**
 * @class SouvenirItem
 * @brief Товарная позиция в сувенирной лавке музея.
 */
class SouvenirItem {
public:
    SouvenirItem(const std::string& skuCode,
                 const std::string& itemName,
                 double unitPrice,
                 int stockQuantity,
                 const std::string& relatedExhibitionId = "EXH-01");

    void sellUnits(int count);
    void restockUnits(int count);
    double applyMemberDiscountPrice(const MembershipCard& card) const;

    const std::string& skuCode() const noexcept { return skuCode_; }
    const std::string& itemName() const noexcept { return itemName_; }
    double unitPrice() const noexcept { return unitPrice_; }
    int stockQuantity() const noexcept { return stockQuantity_; }

private:
    std::string skuCode_;
    std::string itemName_;
    double unitPrice_;
    int stockQuantity_;
    std::string relatedExhibitionId_;
};

/**
 * @class GiftShop
 * @brief Сувенирный магазин при музее.
 */
class GiftShop {
public:
    explicit GiftShop(const std::string& shopName = "Museum Art Shop");

    void addSouvenirItem(const SouvenirItem& item);
    double purchaseSouvenir(Visitor& visitor,
                            const std::string& skuCode,
                            int quantity,
                            MuseumBudget& budget);
    double calculateInventoryValue() const;

    double dailySales() const noexcept { return dailySales_; }
    int transactionsCount() const noexcept { return transactionsCount_; }

private:
    std::string shopName_;
    double dailySales_;
    int transactionsCount_;
    std::vector<SouvenirItem> inventory_;
};

/**
 * @class Museum
 * @brief Главный класс «Музей», объединяющий все подсистемы музейного комплекса.
 */
class Museum {
public:
    Museum(const std::string& museumName = "State Historical & Art Museum",
           int foundedYear = 1895,
           const std::string& city = "Minsk");

    void addExhibitionHall(const ExhibitionHall& hall);
    void acquireNewArtifact(Artifact& artifact, int targetHallNumber, const std::string& description);
    void payMonthlyStaffSalaries();
    int calculateTotalVisitorCapacity() const;
    std::string generateAnnualSummaryReport() const;

    const std::string& museumName() const noexcept { return museumName_; }
    CollectionCatalog& catalog() noexcept { return catalog_; }
    StorageVault& vault() noexcept { return vault_; }
    MuseumBudget& budget() noexcept { return budget_; }
    HRDepartment& hrDepartment() noexcept { return hrDepartment_; }
    SecurityControlCenter& securityCenter() noexcept { return securityCenter_; }
    std::vector<ExhibitionHall>& halls() noexcept { return halls_; }

private:
    std::string museumName_;
    int foundedYear_;
    std::string city_;
    CollectionCatalog catalog_;
    StorageVault vault_;
    MuseumBudget budget_;
    HRDepartment hrDepartment_;
    SecurityControlCenter securityCenter_;
    std::vector<ExhibitionHall> halls_;
};

#endif // MUSEUM_FINANCE_AND_SHOP_H
