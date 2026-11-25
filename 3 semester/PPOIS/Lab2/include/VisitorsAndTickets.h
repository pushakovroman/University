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
 * @details Паттерн Strategy: инкапсулирует расчёт льготных (дети, пенсионеры, студенты, члены клуба) и групповых скидок.
 * @see TicketOffice, Visitor
 */
class DiscountPolicy {
public:
    /**
     * @brief Конструктор класса DiscountPolicy.
     * @param studentDiscount Доля скидки для студентов (по умолчанию 0.30).
     * @param seniorDiscount Доля скидки для пенсионеров (по умолчанию 0.40).
     * @param childDiscount Доля скидки для детей (по умолчанию 0.50).
     * @param groupThreshold Минимальный размер группы для оптовой скидки.
     * @param groupDiscount Доля групповой скидки (по умолчанию 0.20).
     */
    DiscountPolicy(double studentDiscount = 0.30,
                   double seniorDiscount = 0.40,
                   double childDiscount = 0.50,
                   int groupThreshold = 10,
                   double groupDiscount = 0.20);

    /**
     * @brief Рассчитывает долю скидки: дети (<=14) 50%, пенсионеры (>=60) 40%, студенты 30%.
     * @details +15% для членов клуба; максимум 70%.
     * @param visitorAge Возраст посетителя в полных годах.
     * @param isStudent Признак наличия студенческого статуса.
     * @param isMember Признак наличия активной клубной карты музея.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateDiscountPercent(int visitorAge, bool isStudent, bool isMember) const;
    /**
     * @brief Применяет групповую скидку (20%), если размер группы >= groupThreshold_ (10).
     * @param totalAmount Исходная суммарная стоимость группового заказа.
     * @param groupSize Количество человек в группе.
     * @return Значение типа @c double, вычисленное методом.
     */
    double applyGroupDiscount(double totalAmount, int groupSize) const;
    /**
     * @brief Вычисляет итоговую цену с ограничением скидки в пределах [0%, 70%].
     * @param basePrice Базовая цена до применения скидки.
     * @param discountFraction Размер скидки в долях [0.0, 0.70].
     * @return Значение типа @c double, вычисленное методом.
     */
    double applyDiscount(double basePrice, double discountFraction) const;

private:
    double studentDiscount_; ///< Доля скидки для студентов (по умолчанию 0.30).
    double seniorDiscount_; ///< Доля скидки для пенсионеров (по умолчанию 0.40).
    double childDiscount_; ///< Доля скидки для детей (по умолчанию 0.50).
    int groupThreshold_; ///< Минимальный размер группы для оптовой скидки.
    double groupDiscount_; ///< Доля групповой скидки (по умолчанию 0.20).
};

/**
 * @class Visitor
 * @brief Посетитель музея.
 * @details Профиль посетителя музея с возрастом, студенческим статусом, клубным членством и балансом кошелька.
 * @see Ticket, TicketOffice, CloakroomTag
 */
class Visitor {
public:
    /**
     * @brief Конструктор класса Visitor.
     * @param visitorId Уникальный идентификатор посетителя.
     * @param fullName Полное имя автора, сотрудника или посетителя.
     * @param age Возраст посетителя.
     * @param isStudent Признак наличия студенческого статуса.
     * @param walletBalance Начальный баланс кошелька посетителя.
     * @param hasMembership Признак участия в клубной программе лояльности.
     */
    Visitor(const std::string& visitorId,
            const std::string& fullName,
            int age,
            bool isStudent,
            double walletBalance,
            bool hasMembership = false);

    /**
     * @brief Списывает сумму amount с кошелька посетителя при достаточном балансе.
     * @param amount Денежная сумма операции.
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     */
    void payAmount(double amount);
    /**
     * @brief Пополняет баланс кошелька посетителя на положительную сумму amount.
     * @param amount Денежная сумма операции.
     */
    void topUpWallet(double amount);
    /**
     * @brief Возвращает true, если посетитель относится к льготной категории (<=14 лет, >=60 лет или студент).
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isEligibleForConcession() const;

    /// @brief Возвращает идентификатор посетителя.
    const std::string& visitorId() const noexcept { return visitorId_; }
    /// @brief Возвращает полное имя.
    const std::string& fullName() const noexcept { return fullName_; }
    /// @brief Возвращает возраст посетителя.
    int age() const noexcept { return age_; }
    /// @brief Возвращает признак студенческого статуса.
    bool isStudent() const noexcept { return isStudent_; }
    /// @brief Возвращает текущий баланс кошелька.
    double walletBalance() const noexcept { return walletBalance_; }
    /// @brief Возвращает признак членства в клубе музея.
    bool hasMembership() const noexcept { return hasMembership_; }

private:
    std::string visitorId_; ///< Уникальный идентификатор посетителя.
    std::string fullName_; ///< Полное имя автора, сотрудника или посетителя.
    int age_; ///< Возраст посетителя.
    bool isStudent_; ///< Признак наличия студенческого статуса.
    double walletBalance_; ///< Начальный баланс кошелька посетителя.
    bool hasMembership_; ///< Признак участия в клубной программе лояльности.
};

/**
 * @class Ticket
 * @brief Входной билет на выставку музея.
 * @details Номерной входной билет на конкретную выставку с защитой от повторного прохода и поддержкой возврата.
 * @see Visitor, TicketOffice
 */
class Ticket {
public:
    /**
     * @brief Конструктор класса Ticket.
     * @param ticketNumber Уникальный номер билета.
     * @param visitorId Уникальный идентификатор посетителя.
     * @param exhibitionId Уникальный идентификатор выставки.
     * @param finalPrice Итоговая оплаченная стоимость билета.
     */
    Ticket(const std::string& ticketNumber,
           const std::string& visitorId,
           const std::string& exhibitionId,
           double finalPrice);

    /**
     * @brief Валидирует билет для указанной выставки и гасит его (isUsed_ = true).
     * @param targetExhibitionId Идентификатор целевой выставки для проверки билета.
     * @throw InvalidTicketException При нарушении бизнес-правил или инвариантов операции.
     */
    void validateAndUse(const std::string& targetExhibitionId);
    /**
     * @brief Оформляет возврат неиспользованного билета и зачисляет finalPrice_ обратно на кошелёк посетителя.
     * @param visitor Посетитель музея, участвующий в транзакции.
     * @throw InvalidTicketException При нарушении бизнес-правил или инвариантов операции.
     */
    void refundTicket(Visitor& visitor);
    /**
     * @brief Проверяет, что билет не использован, не возвращён и соответствует targetExhibitionId.
     * @param targetExhibitionId Идентификатор целевой выставки для проверки билета.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isValidForExhibition(const std::string& targetExhibitionId) const;

    /// @brief Возвращает уникальный номер билета.
    const std::string& ticketNumber() const noexcept { return ticketNumber_; }
    /// @brief Возвращает оплаченную стоимость билета.
    double finalPrice() const noexcept { return finalPrice_; }
    /// @brief Возвращает true, если билет уже погашен.
    bool isUsed() const noexcept { return isUsed_; }
    /// @brief Возвращает true, если по билету выполнен возврат.
    bool isRefunded() const noexcept { return isRefunded_; }

private:
    std::string ticketNumber_; ///< Уникальный номер билета.
    std::string visitorId_; ///< Уникальный идентификатор посетителя.
    std::string exhibitionId_; ///< Уникальный идентификатор выставки.
    double finalPrice_; ///< Итоговая оплаченная стоимость билета.
    bool isUsed_; ///< Признак погашения входного билета.
    bool isRefunded_; ///< Признак возврата денежных средств по билету.
};

/**
 * @class MembershipCard
 * @brief Клубная карта «Друг музея» с накопительной бонусной системой.
 * @details Накопительная клубная карта «Друг музея» с автоповышением статуса Silver -> Gold (>=500) -> Platinum (>=1500).
 * @see SouvenirItem
 */
class MembershipCard {
public:
    /**
     * @brief Конструктор класса MembershipCard.
     * @param cardNumber Номер клубной карты «Друг музея».
     * @param holderVisitorId Идентификатор владельца клубной карты.
     * @param tierName Уровень статуса карты (Silver, Gold, Platinum).
     * @param loyaltyPoints Начальное число бонусных баллов.
     */
    MembershipCard(const std::string& cardNumber,
                   const std::string& holderVisitorId,
                   const std::string& tierName = "Silver",
                   int loyaltyPoints = 0);

    /**
     * @brief Начисляет 5 бонусных баллов за каждую единицу потраченной суммы и проверяет повышение статуса.
     * @param spentAmount Сумма покупки, от которой начисляются баллы.
     */
    void accruePoints(double spentAmount);
    /**
     * @brief Списывает pointsToUse баллов активной карты и возвращает денежный эквивалент (0.10 за балл).
     * @param pointsToUse Количество списываемых бонусных баллов.
     * @return Значение типа @c double, вычисленное методом.
     * @throw InvalidTicketException При нарушении бизнес-правил или инвариантов операции.
     */
    double redeemPoints(int pointsToUse);
    /**
     * @brief Автоматически повышает уровень карты: Gold при >= 500 баллов, Platinum при >= 1500 баллов.
     */
    void upgradeTierIfEligible();
    /**
     * @brief Блокирует клубную карту (isActive_ = false).
     */
    void deactivateCard();

    /// @brief Возвращает номер клубной карты.
    const std::string& cardNumber() const noexcept { return cardNumber_; }
    /// @brief Возвращает текущий уровень статуса карты.
    const std::string& tierName() const noexcept { return tierName_; }
    /// @brief Возвращает накопленные бонусные баллы.
    int loyaltyPoints() const noexcept { return loyaltyPoints_; }
    /// @brief Возвращает признак активности объекта.
    bool isActive() const noexcept { return isActive_; }

private:
    std::string cardNumber_; ///< Номер клубной карты «Друг музея».
    std::string holderVisitorId_; ///< Идентификатор владельца клубной карты.
    std::string tierName_; ///< Уровень статуса карты (Silver, Gold, Platinum).
    int loyaltyPoints_; ///< Начальное число бонусных баллов.
    bool isActive_; ///< Признак активности установки или клубной карты.
};

/**
 * @class CloakroomTag
 * @brief Номерок музейного гардероба / ячейки хранения.
 * @details Номерок музейного гардероба с привязкой к идентификатору владельца для предотвращения чужой выдачи.
 * @see Visitor
 */
class CloakroomTag {
public:
    /**
     * @brief Конструктор класса CloakroomTag.
     * @param tagNumber Номер бирки/ячейки гардероба.
     */
    explicit CloakroomTag(int tagNumber = 1);

    /**
     * @brief Принимает count вещей посетителя на свободный номерок гардероба.
     * @param visitor Посетитель музея, участвующий в транзакции.
     * @param count Количество посетителей, вещей или единиц товара.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void acceptItems(const Visitor& visitor, int count);
    /**
     * @brief Выдаёт вещи только владельцу с совпадающим visitorId_ и освобождает номерок.
     * @param visitor Посетитель музея, участвующий в транзакции.
     * @throw SecurityBreachException При нарушении бизнес-правил или инвариантов операции.
     */
    void releaseItems(const Visitor& visitor);
    /**
     * @brief Очищает данные о владельце и количестве вещей, освобождая номерок.
     */
    void clearTag();

    /// @brief Возвращает номер гардеробной бирки.
    int tagNumber() const noexcept { return tagNumber_; }
    /// @brief Возвращает количество сданных вещей.
    int itemsCount() const noexcept { return itemsCount_; }
    /// @brief Возвращает true, если номерок занят.
    bool isOccupied() const noexcept { return isOccupied_; }

private:
    int tagNumber_; ///< Номер бирки/ячейки гардероба.
    std::string visitorId_; ///< Уникальный идентификатор посетителя.
    int itemsCount_; ///< Количество принятых в гардероб вещей.
    bool isOccupied_; ///< Признак занятости гардеробного номерка.
};

/**
 * @class VisitorFeedback
 * @brief Запись в книге отзывов и предложений музея.
 * @details Запись в электронной книге отзывов с оценкой 1–5 звёзд и классификацией тональности.
 * @see Exhibition
 */
class VisitorFeedback {
public:
    /**
     * @brief Конструктор класса VisitorFeedback.
     * @param feedbackId Идентификатор записи отзыва.
     * @param visitorName Имя автора отзыва.
     * @param exhibitionId Уникальный идентификатор выставки.
     * @param ratingStars Оценка выставки по шкале 1–5 звёзд.
     * @param comment Текстовый комментарий посетителя.
     */
    VisitorFeedback(const std::string& feedbackId,
                    const std::string& visitorName,
                    const std::string& exhibitionId,
                    int ratingStars,
                    const std::string& comment);

    /**
     * @brief Возвращает true при оценке 4 или 5 звёзд.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isPositive() const;
    /**
     * @brief Возвращает true при негативной оценке <= 2 звёзд.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool requiresManagerAttention() const;
    /**
     * @brief Форматирует строку сводки вида: "<Имя> [<оценка>/5]: <комментарий>".
     * @return Значение типа @c std::string, вычисленное методом.
     */
    std::string formatSummary() const;

    /// @brief Возвращает выставленную оценку (1–5).
    int ratingStars() const noexcept { return ratingStars_; }
    /// @brief Возвращает идентификатор выставки.
    const std::string& exhibitionId() const noexcept { return exhibitionId_; }

private:
    std::string feedbackId_; ///< Идентификатор записи отзыва.
    std::string visitorName_; ///< Имя автора отзыва.
    std::string exhibitionId_; ///< Уникальный идентификатор выставки.
    int ratingStars_; ///< Оценка выставки по шкале 1–5 звёзд.
    std::string comment_; ///< Текстовый комментарий посетителя.
};

#endif // MUSEUM_VISITORS_AND_TICKETS_H
