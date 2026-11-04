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
 */
class MuseumException : public std::runtime_error {
public:
    explicit MuseumException(const std::string& message,
                             const std::string& category = "MUSEUM_ERROR",
                             int errorCode = 1000);

    const std::string& category() const noexcept;
    int errorCode() const noexcept;
    std::string formatErrorReport() const;

private:
    std::string category_;
    int errorCode_;
};

/**
 * @class ArtifactNotFoundException
 * @brief Исключение: экспонат не найден в каталоге, витрине или фондохранилище.
 */
class ArtifactNotFoundException : public MuseumException {
public:
    explicit ArtifactNotFoundException(const std::string& inventoryNumber);
    const std::string& inventoryNumber() const noexcept;

private:
    std::string inventoryNumber_;
};

/**
 * @class InvalidArtifactDataException
 * @brief Исключение: некорректные параметры экспоната (габариты, год, стоимость, сохранность).
 */
class InvalidArtifactDataException : public MuseumException {
public:
    explicit InvalidArtifactDataException(const std::string& details);
};

/**
 * @class HallOvercapacityException
 * @brief Исключение: превышена максимальная вместимость зала или хранилища.
 */
class HallOvercapacityException : public MuseumException {
public:
    HallOvercapacityException(const std::string& locationName, int maxCapacity);
    int maxCapacity() const noexcept;

private:
    int maxCapacity_;
};

/**
 * @class ClimateOutOfRangeException
 * @brief Исключение: нарушение температурно-влажностного или светового режима хранения.
 */
class ClimateOutOfRangeException : public MuseumException {
public:
    ClimateOutOfRangeException(const std::string& parameterName, double actualValue, double allowedLimit);
    double actualValue() const noexcept;
    double allowedLimit() const noexcept;

private:
    double actualValue_;
    double allowedLimit_;
};

/**
 * @class TicketSoldOutException
 * @brief Исключение: все билеты или места на экскурсию/мероприятие распроданы.
 */
class TicketSoldOutException : public MuseumException {
public:
    explicit TicketSoldOutException(const std::string& eventTitle);
};

/**
 * @class InvalidTicketException
 * @brief Исключение: билет недействителен, уже использован или возвращён.
 */
class InvalidTicketException : public MuseumException {
public:
    explicit InvalidTicketException(const std::string& ticketNumber);
};

/**
 * @class InsufficientFundsException
 * @brief Исключение: недостаточно денежных средств у посетителя или в бюджете музея.
 */
class InsufficientFundsException : public MuseumException {
public:
    InsufficientFundsException(double requiredAmount, double availableBalance);
    double requiredAmount() const noexcept;
    double availableBalance() const noexcept;

private:
    double requiredAmount_;
    double availableBalance_;
};

/**
 * @class RestorationRequiredException
 * @brief Исключение: экспонат находится в аварийном состоянии и требует реставрации.
 */
class RestorationRequiredException : public MuseumException {
public:
    RestorationRequiredException(const std::string& artifactTitle, double currentCondition);
    double currentCondition() const noexcept;

private:
    double currentCondition_;
};

/**
 * @class SecurityBreachException
 * @brief Исключение: нарушение безопасности, отсутствие допуска или активная блокировка.
 */
class SecurityBreachException : public MuseumException {
public:
    explicit SecurityBreachException(const std::string& securityZone);
};

/**
 * @class StaffNotAvailableException
 * @brief Исключение: сотрудник не на смене, занят или не обладает нужной квалификацией.
 */
class StaffNotAvailableException : public MuseumException {
public:
    explicit StaffNotAvailableException(const std::string& staffName);
};

/**
 * @class ExhibitionClosedException
 * @brief Исключение: выставка закрыта для посещения или ещё не открыта.
 */
class ExhibitionClosedException : public MuseumException {
public:
    explicit ExhibitionClosedException(const std::string& exhibitionTitle);
};

/**
 * @class LoanAgreementViolationException
 * @brief Исключение: нарушение правил межмузейного займа/аренды экспоната.
 */
class LoanAgreementViolationException : public MuseumException {
public:
    explicit LoanAgreementViolationException(const std::string& reason);
};

#endif // MUSEUM_EXCEPTIONS_H
