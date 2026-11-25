/**
 * @file MuseumExceptions.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Иерархия из 13 собственных классов исключений предметной области «Музей».
 * @details Все классы исключений наследуются от базового класса MuseumException,
 * который расширяет стандартный std::runtime_error кодом ошибки и категорией.
 */

#ifndef MUSEUM_EXCEPTIONS_H
#define MUSEUM_EXCEPTIONS_H

#include <stdexcept>
#include <string>

/**
 * @class MuseumException
 * @brief Базовый класс исключений музейной системы.
 * @details Базовый класс иерархии исключений предметной области (код #1000, категория MUSEUM_ERROR).
 * @see ArtifactNotFoundException, InvalidArtifactDataException, ClimateOutOfRangeException
 */
class MuseumException : public std::runtime_error {
public:
    /**
     * @brief Конструктор класса MuseumException.
     * @param message Диагностическое текстовое сообщение об ошибке.
     * @param category Строковый идентификатор категории исключения.
     * @param errorCode Числовой код ошибки (1000–1012).
     */
    explicit MuseumException(const std::string& message,
                             const std::string& category = "MUSEUM_ERROR",
                             int errorCode = 1000);

    /**
     * @brief Возвращает строковый код категории исключения (например, "CLIMATE_ALERT").
     * @return Значение типа @c const std::string&, вычисленное методом.
     */
    const std::string& category() const noexcept;
    /**
     * @brief Возвращает уникальный числовой код ошибки (диапазон 1000–1012).
     * @return Значение типа @c int, вычисленное методом.
     */
    int errorCode() const noexcept;
    /**
     * @brief Форматирует структурированный отчёт вида "[CATEGORY #CODE] message".
     * @return Значение типа @c std::string, вычисленное методом.
     */
    std::string formatErrorReport() const;

private:
    std::string category_; ///< Строковый идентификатор категории исключения.
    int errorCode_; ///< Числовой код ошибки (1000–1012).
};

/**
 * @class ArtifactNotFoundException
 * @brief Исключение: экспонат не найден в каталоге, витрине или фондохранилище.
 * @details Выбрасывается при отсутствии инвентарного номера, SKU или наряда (код #1001, ARTIFACT_NOT_FOUND).
 * @see MuseumException, StorageVault, CollectionCatalog
 */
class ArtifactNotFoundException : public MuseumException {
public:
    /**
     * @brief Конструктор класса ArtifactNotFoundException.
     * @param inventoryNumber Уникальный инвентарный номер экспоната.
     */
    explicit ArtifactNotFoundException(const std::string& inventoryNumber);
    /**
     * @brief Возвращает ненайденный инвентарный номер, SKU или идентификатор задачи.
     * @return Значение типа @c const std::string&, вычисленное методом.
     */
    const std::string& inventoryNumber() const noexcept;

private:
    std::string inventoryNumber_; ///< Уникальный инвентарный номер экспоната.
};

/**
 * @class InvalidArtifactDataException
 * @brief Исключение: некорректные параметры экспоната (габариты, год, стоимость, сохранность).
 * @details Выбрасывается при нарушении числовых/логических инвариантов экспоната или габаритов (код #1002, INVALID_ARTIFACT).
 * @see MuseumException, Dimensions, Artifact
 */
class InvalidArtifactDataException : public MuseumException {
public:
    /**
     * @brief Конструктор класса InvalidArtifactDataException.
     * @param details Подробное описание нарушения валидации данных.
     */
    explicit InvalidArtifactDataException(const std::string& details);
};

/**
 * @class HallOvercapacityException
 * @brief Исключение: превышена максимальная вместимость зала или хранилища.
 * @details Выбрасывается при превышении вместимости зала, хранилища, стеллажа, квоты или диска камеры (код #1003, OVERCAPACITY).
 * @see MuseumException, ExhibitionHall, StorageVault
 */
class HallOvercapacityException : public MuseumException {
public:
    /**
     * @brief Конструктор класса HallOvercapacityException.
     * @param locationName Наименование переполненного объекта (зала, стеллажа, хранилища).
     * @param maxCapacity Максимальная вместимость хранилища или зала.
     */
    HallOvercapacityException(const std::string& locationName, int maxCapacity);
    /**
     * @brief Возвращает предельно допустимую вместимость зала, стеллажа или квоты.
     * @return Значение типа @c int, вычисленное методом.
     */
    int maxCapacity() const noexcept;

private:
    int maxCapacity_; ///< Максимальная вместимость хранилища или зала.
};

/**
 * @class ClimateOutOfRangeException
 * @brief Исключение: нарушение температурно-влажностного или светового режима хранения.
 * @details Выбрасывается при отклонении влажности или освещённости от норм хранения (код #1004, CLIMATE_ALERT).
 * @see MuseumException, ClimateControlUnit
 */
class ClimateOutOfRangeException : public MuseumException {
public:
    /**
     * @brief Конструктор класса ClimateOutOfRangeException.
     * @param parameterName Наименование нарушенного климатического параметра.
     * @param actualValue Фактически зафиксированное значение параметра.
     * @param allowedLimit Предельно допустимое значение параметра.
     */
    ClimateOutOfRangeException(const std::string& parameterName, double actualValue, double allowedLimit);
    /**
     * @brief Возвращает зафиксированное датчиком фактическое значение параметра.
     * @return Значение типа @c double, вычисленное методом.
     */
    double actualValue() const noexcept;
    /**
     * @brief Возвращает максимально допустимое пороговое значение для экспоната.
     * @return Значение типа @c double, вычисленное методом.
     */
    double allowedLimit() const noexcept;

private:
    double actualValue_; ///< Фактически зафиксированное значение параметра.
    double allowedLimit_; ///< Предельно допустимое значение параметра.
};

/**
 * @class TicketSoldOutException
 * @brief Исключение: все билеты или места на экскурсию/мероприятие распроданы.
 * @details Выбрасывается при нехватке мест на тур/мероприятие или сувениров на складе (код #1005, TICKET_SOLD_OUT).
 * @see MuseumException, GuidedTour, SpecialEvent, SouvenirItem
 */
class TicketSoldOutException : public MuseumException {
public:
    /**
     * @brief Конструктор класса TicketSoldOutException.
     * @param eventTitle Название распроданного события, тура или товара.
     */
    explicit TicketSoldOutException(const std::string& eventTitle);
};

/**
 * @class InvalidTicketException
 * @brief Исключение: билет недействителен, уже использован или возвращён.
 * @details Выбрасывается при повторном использовании билета, неактивной карте или повторном переводе гранта (код #1006, INVALID_TICKET).
 * @see MuseumException, Ticket, MembershipCard, DonationGrant
 */
class InvalidTicketException : public MuseumException {
public:
    /**
     * @brief Конструктор класса InvalidTicketException.
     * @param ticketNumber Уникальный номер билета.
     */
    explicit InvalidTicketException(const std::string& ticketNumber);
};

/**
 * @class InsufficientFundsException
 * @brief Исключение: недостаточно денежных средств у посетителя или в бюджете музея.
 * @details Выбрасывается при нехватке средств на счёте/кошельке или материала в мастерской (код #1007, INSUFFICIENT_FUNDS).
 * @see MuseumException, Visitor, MuseumBudget, RestorationMaterial
 */
class InsufficientFundsException : public MuseumException {
public:
    /**
     * @brief Конструктор класса InsufficientFundsException.
     * @param requiredAmount Требуемая сумма или объём ресурса.
     * @param availableBalance Фактически доступный остаток.
     */
    InsufficientFundsException(double requiredAmount, double availableBalance);
    /**
     * @brief Возвращает требуемую сумму (или массу материала в граммах).
     * @return Значение типа @c double, вычисленное методом.
     */
    double requiredAmount() const noexcept;
    /**
     * @brief Возвращает фактически доступный баланс (или остаток материала).
     * @return Значение типа @c double, вычисленное методом.
     */
    double availableBalance() const noexcept;

private:
    double requiredAmount_; ///< Требуемая сумма или объём ресурса.
    double availableBalance_; ///< Фактически доступный остаток.
};

/**
 * @class RestorationRequiredException
 * @brief Исключение: экспонат находится в аварийном состоянии и требует реставрации.
 * @details Выбрасывается при попытке экспонировать, упаковать или выдать в займ предмет с сохранностью < 40% (код #1008, RESTORATION_REQUIRED).
 * @see MuseumException, DisplayCase, TransportCrate, LoanAgreement
 */
class RestorationRequiredException : public MuseumException {
public:
    /**
     * @brief Конструктор класса RestorationRequiredException.
     * @param artifactTitle Наименование аварийного экспоната.
     * @param currentCondition Текущий процент сохранности аварийного экспоната.
     */
    RestorationRequiredException(const std::string& artifactTitle, double currentCondition);
    /**
     * @brief Возвращает текущий процент сохранности аварийного экспоната (< 40%).
     * @return Значение типа @c double, вычисленное методом.
     */
    double currentCondition() const noexcept;

private:
    double currentCondition_; ///< Текущий процент сохранности аварийного экспоната.
};

/**
 * @class SecurityBreachException
 * @brief Исключение: нарушение безопасности, отсутствие допуска или активная блокировка.
 * @details Выбрасывается при нарушении охранного режима, чужом гардеробном номерке или отозванном пропуске (код #1009, SECURITY_BREACH).
 * @see MuseumException, AlarmTrigger, AccessBadge, CloakroomTag
 */
class SecurityBreachException : public MuseumException {
public:
    /**
     * @brief Конструктор класса SecurityBreachException.
     * @param securityZone Описание зоны или объекта нарушения безопасности.
     */
    explicit SecurityBreachException(const std::string& securityZone);
};

/**
 * @class StaffNotAvailableException
 * @brief Исключение: сотрудник не на смене, занят или не обладает нужной квалификацией.
 * @details Выбрасывается при отсутствии сотрудника на смене, недостатке квалификации/языка/рации или недоступности аудиогида (код #1010, STAFF_UNAVAILABLE).
 * @see MuseumException, Curator, TourGuide, Restorer, SecurityGuard
 */
class StaffNotAvailableException : public MuseumException {
public:
    /**
     * @brief Конструктор класса StaffNotAvailableException.
     * @param staffName Имя сотрудника или идентификатор недоступного ресурса.
     */
    explicit StaffNotAvailableException(const std::string& staffName);
};

/**
 * @class ExhibitionClosedException
 * @brief Исключение: выставка закрыта для посещения или ещё не открыта.
 * @details Выбрасывается при операции над закрытой или пустой выставкой / пустым туром (код #1011, EXHIBITION_CLOSED).
 * @see MuseumException, Exhibition, GuidedTour, TicketOffice
 */
class ExhibitionClosedException : public MuseumException {
public:
    /**
     * @brief Конструктор класса ExhibitionClosedException.
     * @param exhibitionTitle Название закрытой выставки или неподготовленного тура.
     */
    explicit ExhibitionClosedException(const std::string& exhibitionTitle);
};

/**
 * @class LoanAgreementViolationException
 * @brief Исключение: нарушение правил межмузейного займа/аренды экспоната.
 * @details Выбрасывается при несоответствии рейтинга музея-партнёра или недостаточном страховом залоге (код #1012, LOAN_VIOLATION).
 * @see MuseumException, LoanAgreement
 */
class LoanAgreementViolationException : public MuseumException {
public:
    /**
     * @brief Конструктор класса LoanAgreementViolationException.
     * @param reason Причина нарушения условий договора межмузейного займа.
     */
    explicit LoanAgreementViolationException(const std::string& reason);
};

#endif // MUSEUM_EXCEPTIONS_H
