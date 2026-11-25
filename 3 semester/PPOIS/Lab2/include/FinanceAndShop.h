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
 * @details Расчётный счёт и финансовый бюджет музея с учётом доходов, расходов и государственных субсидий.
 * @see TicketOffice, DonationGrant, GiftShop, Museum
 */
class MuseumBudget {
public:
    /**
     * @brief Конструктор класса MuseumBudget.
     * @param fiscalYear Финансовый год бюджета.
     * @param initialBalance Начальный остаток средств на счёте (>= 0).
     * @param stateSubsidy Размер плановой государственной субсидии (>= 0).
     */
    MuseumBudget(int fiscalYear = 2026,
                 double initialBalance = 100000.0,
                 double stateSubsidy = 50000.0);

    /**
     * @brief Зачисляет положительный доход на баланс и увеличивает счётчик totalRevenue_.
     * @param amount Денежная сумма операции.
     */
    void receiveIncome(double amount);
    /**
     * @brief Списывает расход amount с баланса при достаточности средств.
     * @param amount Денежная сумма операции.
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     */
    void allocateExpense(double amount);
    /**
     * @brief Зачисляет плановую государственную субсидию stateSubsidy_ в доход бюджета.
     */
    void applyStateSubsidy();
    /**
     * @brief Возвращает чистый операционный результат периода: totalRevenue_ - totalExpenses_.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateNetBalance() const;

    /// @brief Возвращает текущий денежный остаток бюджета.
    double currentBalance() const noexcept { return currentBalance_; }
    /// @brief Возвращает суммарные поступления бюджета.
    double totalRevenue() const noexcept { return totalRevenue_; }
    /// @brief Возвращает суммарные расходы бюджета.
    double totalExpenses() const noexcept { return totalExpenses_; }

private:
    int fiscalYear_; ///< Финансовый год бюджета.
    double currentBalance_; ///< Текущий баланс бюджета музея.
    double totalRevenue_; ///< Накопленная сумма доходов за период.
    double totalExpenses_; ///< Накопленная сумма расходов за период.
    double stateSubsidy_; ///< Размер плановой государственной субсидии (>= 0).
};

/**
 * @class TicketOffice
 * @brief Билетная касса музея.
 * @details Билетная касса: применяет DiscountPolicy, проверяет выставку/зал, списывает оплату и оформляет возвраты.
 * @see DiscountPolicy, Visitor, Exhibition, ExhibitionHall, Ticket, MuseumBudget
 */
class TicketOffice {
public:
    /**
     * @brief Конструктор класса TicketOffice.
     * @param officeId Идентификатор билетной кассы.
     * @param discountPolicy Используемая политика расчёта скидок.
     */
    TicketOffice(const std::string& officeId = "BOX-01",
                 const DiscountPolicy& discountPolicy = DiscountPolicy());

    /**
     * @brief Комплексная продажа билета: проверка открытости выставки, допуск в зал, расчёт скидки, оплата, зачисление в кассу/бюджет, выдача Ticket.
     * @param visitor Посетитель музея, участвующий в транзакции.
     * @param exhibition Выставка для добавления в расписание, организации или продажи билета.
     * @param hall Выставочный зал, задействованный в операции.
     * @param budget Финансовый бюджет музея для зачисления или списания средств.
     * @return Значение типа @c Ticket, вычисленное методом.
     * @throw ExhibitionClosedException При нарушении бизнес-правил или инвариантов операции.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     */
    Ticket sellTicket(Visitor& visitor,
                      const Exhibition& exhibition,
                      ExhibitionHall& hall,
                      MuseumBudget& budget);
    /**
     * @brief Оформляет возврат: списывает сумму из бюджета, возвращает деньги на кошелёк посетителя и корректирует кассу.
     * @param ticket Возвращаемый входной билет.
     * @param visitor Посетитель музея, участвующий в транзакции.
     * @param budget Финансовый бюджет музея для зачисления или списания средств.
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     * @throw InvalidTicketException При нарушении бизнес-правил или инвариантов операции.
     */
    void processTicketRefund(Ticket& ticket, Visitor& visitor, MuseumBudget& budget);
    /**
     * @brief Инкассирует накопленную наличность кассы на расчётный счёт музея.
     * @param budget Финансовый бюджет музея для зачисления или списания средств.
     */
    void transferCashToBudget(MuseumBudget& budget);

    /// @brief Возвращает число проданных через кассу билетов.
    int soldTicketsCount() const noexcept { return soldTicketsCount_; }
    /// @brief Возвращает сумму наличности в кассе.
    double cashInRegister() const noexcept { return cashInRegister_; }

private:
    std::string officeId_; ///< Идентификатор билетной кассы.
    double cashInRegister_; ///< Сумма наличных в билетной кассе.
    int soldTicketsCount_; ///< Счётчик проданных кассой билетов.
    DiscountPolicy discountPolicy_; ///< Используемая политика расчёта скидок.
};

/**
 * @class DonationGrant
 * @brief Благотворительный грант или спонсорский взнос мецената.
 * @details Целевой спонсорский грант мецената с защитой от двойного зачисления и проверкой покрытия реставрационной сметы.
 * @see MuseumBudget, RestorationTask
 */
class DonationGrant {
public:
    /**
     * @brief Конструктор класса DonationGrant.
     * @param grantId Регистрационный код благотворительного гранта.
     * @param sponsorName Имя мецената или название спонсорского фонда.
     * @param amount Денежная сумма операции.
     * @param targetPurpose Целевое назначение гранта.
     */
    DonationGrant(const std::string& grantId,
                  const std::string& sponsorName,
                  double amount,
                  const std::string& targetPurpose);

    /**
     * @brief Однократно переводит сумму гранта на счёт бюджета.
     * @details Повторный перевод блокируется.
     * @param budget Финансовый бюджет музея для зачисления или списания средств.
     * @throw InvalidTicketException При нарушении бизнес-правил или инвариантов операции.
     */
    void transferToBudget(MuseumBudget& budget);
    /**
     * @brief Проверяет, покрывает ли сумма гранта сметную стоимость задания task.estimatedCost().
     * @param task Наряд на реставрацию для оценки финансового покрытия.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool canFundRestoration(const RestorationTask& task) const;
    /**
     * @brief Возвращает true для крупных меценатов (взнос >= 10 000).
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isLargePatron() const;

    /// @brief Возвращает идентификатор гранта.
    const std::string& grantId() const noexcept { return grantId_; }
    /// @brief Возвращает сумму гранта.
    double amount() const noexcept { return amount_; }
    /// @brief Возвращает true, если грант уже зачислен в бюджет.
    bool isTransferred() const noexcept { return isTransferred_; }

private:
    std::string grantId_; ///< Регистрационный код благотворительного гранта.
    std::string sponsorName_; ///< Имя мецената или название спонсорского фонда.
    double amount_; ///< Денежная сумма операции.
    std::string targetPurpose_; ///< Целевое назначение гранта.
    bool isTransferred_; ///< Признак зачисления гранта на счёт музея.
};

/**
 * @class SouvenirItem
 * @brief Товарная позиция в сувенирной лавке музея.
 * @details Товарная позиция сувенирного магазина с учётом складского остатка и скидок по карте лояльности.
 * @see MembershipCard, GiftShop
 */
class SouvenirItem {
public:
    /**
     * @brief Конструктор класса SouvenirItem.
     * @param skuCode Артикул (SKU) сувенирного товара.
     * @param itemName Наименование товара сувенирной лавки.
     * @param unitPrice Розничная цена за одну единицу товара (>= 0).
     * @param stockQuantity Начальный складской остаток в штуках (>= 0).
     * @param relatedExhibitionId Идентификатор связанной выставки.
     */
    SouvenirItem(const std::string& skuCode,
                 const std::string& itemName,
                 double unitPrice,
                 int stockQuantity,
                 const std::string& relatedExhibitionId = "EXH-01");

    /**
     * @brief Списывает count единиц товара со склада магазина при достаточном наличии.
     * @param count Количество посетителей, вещей или единиц товара.
     * @throw TicketSoldOutException При нарушении бизнес-правил или инвариантов операции.
     */
    void sellUnits(int count);
    /**
     * @brief Пополняет складской остаток сувенира на count единиц.
     * @param count Количество посетителей, вещей или единиц товара.
     */
    void restockUnits(int count);
    /**
     * @brief Рассчитывает цену со скидкой активной клубной карты: Platinum -20%, Gold -15%, Silver -10%.
     * @param card Клубная карта посетителя для расчёта скидки.
     * @return Значение типа @c double, вычисленное методом.
     */
    double applyMemberDiscountPrice(const MembershipCard& card) const;

    /// @brief Возвращает артикул сувенира.
    const std::string& skuCode() const noexcept { return skuCode_; }
    /// @brief Возвращает наименование сувенира.
    const std::string& itemName() const noexcept { return itemName_; }
    /// @brief Возвращает розничную цену единицы товара.
    double unitPrice() const noexcept { return unitPrice_; }
    /// @brief Возвращает остаток товара на складе.
    int stockQuantity() const noexcept { return stockQuantity_; }

private:
    std::string skuCode_; ///< Артикул (SKU) сувенирного товара.
    std::string itemName_; ///< Наименование товара сувенирной лавки.
    double unitPrice_; ///< Розничная цена за одну единицу товара (>= 0).
    int stockQuantity_; ///< Начальный складской остаток в штуках (>= 0).
    std::string relatedExhibitionId_; ///< Идентификатор связанной выставки.
};

/**
 * @class GiftShop
 * @brief Сувенирный магазин при музее.
 * @details Сувенирная лавка музея: ведёт ассортимент, проводит продажи с зачислением в бюджет и оценивает остатки.
 * @see SouvenirItem, Visitor, MuseumBudget
 */
class GiftShop {
public:
    /**
     * @brief Конструктор класса GiftShop.
     * @param shopName Название сувенирного магазина при музее.
     */
    explicit GiftShop(const std::string& shopName = "Museum Art Shop");

    /**
     * @brief Добавляет новую товарную позицию в ассортимент сувенирной лавки.
     * @param item Добавляемая товарная позиция сувенира.
     */
    void addSouvenirItem(const SouvenirItem& item);
    /**
     * @brief Ищет товар по SKU, списывает оплату с посетителя, уменьшает остаток и переводит выручку в бюджет.
     * @param visitor Посетитель музея, участвующий в транзакции.
     * @param skuCode Артикул (SKU) сувенирного товара.
     * @param quantity Количество приобретаемых единиц товара.
     * @param budget Финансовый бюджет музея для зачисления или списания средств.
     * @return Значение типа @c double, вычисленное методом.
     * @throw ArtifactNotFoundException При нарушении бизнес-правил или инвариантов операции.
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     * @throw TicketSoldOutException При нарушении бизнес-правил или инвариантов операции.
     */
    double purchaseSouvenir(Visitor& visitor,
                            const std::string& skuCode,
                            int quantity,
                            MuseumBudget& budget);
    /**
     * @brief Вычисляет общую розничную стоимость всех товарных остатков в магазине.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateInventoryValue() const;

    /// @brief Возвращает дневную выручку сувенирной лавки.
    double dailySales() const noexcept { return dailySales_; }
    /// @brief Возвращает число завершённых покупок.
    int transactionsCount() const noexcept { return transactionsCount_; }

private:
    std::string shopName_; ///< Название сувенирного магазина при музее.
    double dailySales_; ///< Накопленная дневная выручка сувенирной лавки.
    int transactionsCount_; ///< Количество завершённых покупок в магазине.
    std::vector<SouvenirItem> inventory_; ///< Ассортимент товарных позиций сувенирного магазина.
};

/**
 * @class Museum
 * @brief Главный класс «Музей», объединяющий все подсистемы музейного комплекса.
 * @details Паттерн Facade: единая точка координации каталога, хранилища, бюджета, отдела кадров, службы безопасности и выставочных залов.
 * @see CollectionCatalog, StorageVault, MuseumBudget, HRDepartment, SecurityControlCenter, ExhibitionHall, Artifact
 */
class Museum {
public:
    /**
     * @brief Конструктор класса Museum.
     * @param museumName Официальное название музейного комплекса.
     * @param foundedYear Год основания музея.
     * @param city Город расположения музея.
     */
    Museum(const std::string& museumName = "State Historical & Art Museum",
           int foundedYear = 1895,
           const std::string& city = "Minsk");

    /**
     * @brief Регистрирует новый выставочный зал в структуре музейного комплекса.
     * @param hall Выставочный зал, задействованный в операции.
     */
    void addExhibitionHall(const ExhibitionHall& hall);
    /**
     * @brief Сквозная приёмка нового экспоната: размещает в StorageVault и регистрирует в CollectionCatalog.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @param targetHallNumber Целевой номер зала приписки в каталоге.
     * @param description Краткое научное описание регистрируемого экспоната.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void acquireNewArtifact(Artifact& artifact, int targetHallNumber, const std::string& description);
    /**
     * @brief Вычисляет ФОТ через HRDepartment и списывает зарплату штата из MuseumBudget.
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     */
    void payMonthlyStaffSalaries();
    /**
     * @brief Суммирует максимальную вместимость всех выставочных залов комплекса.
     * @return Значение типа @c int, вычисленное методом.
     */
    int calculateTotalVisitorCapacity() const;
    /**
     * @brief Формирует сводную текстовую строку состояния музея (залы, каталог, хранилище, штат, баланс).
     * @return Значение типа @c std::string, вычисленное методом.
     */
    std::string generateAnnualSummaryReport() const;

    /// @brief Возвращает название музея.
    const std::string& museumName() const noexcept { return museumName_; }
    /// @brief Возвращает ссылку на главный каталог коллекции.
    CollectionCatalog& catalog() noexcept { return catalog_; }
    /// @brief Возвращает ссылку на фондохранилище музея.
    StorageVault& vault() noexcept { return vault_; }
    /// @brief Возвращает ссылку на финансовый бюджет музея.
    MuseumBudget& budget() noexcept { return budget_; }
    /// @brief Возвращает ссылку на отдел кадров музея.
    HRDepartment& hrDepartment() noexcept { return hrDepartment_; }
    /// @brief Возвращает ссылку на диспетчерский пульт охраны.
    SecurityControlCenter& securityCenter() noexcept { return securityCenter_; }
    /// @brief Возвращает ссылку на список выставочных залов музея.
    std::vector<ExhibitionHall>& halls() noexcept { return halls_; }

private:
    std::string museumName_; ///< Официальное название музейного комплекса.
    int foundedYear_; ///< Год основания музея.
    std::string city_; ///< Город расположения музея.
    CollectionCatalog catalog_; ///< Главный каталог коллекции музея.
    StorageVault vault_; ///< Центральное фондохранилище музея.
    MuseumBudget budget_; ///< Финансовый бюджет музея.
    HRDepartment hrDepartment_; ///< Отдел кадров музея.
    SecurityControlCenter securityCenter_; ///< Центральный пульт службы безопасности.
    std::vector<ExhibitionHall> halls_; ///< Список выставочных залов музейного комплекса.
};

#endif // MUSEUM_FINANCE_AND_SHOP_H
