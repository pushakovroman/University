/**
 * @file MuseumExceptions.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов исключений предметной области «Музей».
 */

#include "MuseumExceptions.h"

#include <sstream>

namespace {
constexpr int kArtifactNotFoundCode = 1001;
constexpr int kInvalidArtifactCode = 1002;
constexpr int kOvercapacityCode = 1003;
constexpr int kClimateErrorCode = 1004;
constexpr int kTicketSoldOutCode = 1005;
constexpr int kInvalidTicketCode = 1006;
constexpr int kInsufficientFundsCode = 1007;
constexpr int kRestorationRequiredCode = 1008;
constexpr int kSecurityBreachCode = 1009;
constexpr int kStaffUnavailableCode = 1010;
constexpr int kExhibitionClosedCode = 1011;
constexpr int kLoanViolationCode = 1012;
} // namespace

MuseumException::MuseumException(const std::string& message,
                                 const std::string& category,
                                 int errorCode)
    : std::runtime_error(message), category_(category), errorCode_(errorCode) {}

const std::string& MuseumException::category() const noexcept {
    return category_;
}

int MuseumException::errorCode() const noexcept {
    return errorCode_;
}

std::string MuseumException::formatErrorReport() const {
    std::ostringstream stream;
    stream << "[" << category_ << " #" << errorCode_ << "] " << what();
    return stream.str();
}

ArtifactNotFoundException::ArtifactNotFoundException(const std::string& inventoryNumber)
    : MuseumException("Artifact not found: " + inventoryNumber, "ARTIFACT_NOT_FOUND", kArtifactNotFoundCode),
      inventoryNumber_(inventoryNumber) {}

const std::string& ArtifactNotFoundException::inventoryNumber() const noexcept {
    return inventoryNumber_;
}

InvalidArtifactDataException::InvalidArtifactDataException(const std::string& details)
    : MuseumException("Invalid artifact data: " + details, "INVALID_ARTIFACT", kInvalidArtifactCode) {}

HallOvercapacityException::HallOvercapacityException(const std::string& locationName, int maxCapacity)
    : MuseumException("Capacity exceeded in " + locationName + " (max: " + std::to_string(maxCapacity) + ")",
                      "OVERCAPACITY", kOvercapacityCode),
      maxCapacity_(maxCapacity) {}

int HallOvercapacityException::maxCapacity() const noexcept {
    return maxCapacity_;
}

ClimateOutOfRangeException::ClimateOutOfRangeException(const std::string& parameterName,
                                                       double actualValue,
                                                       double allowedLimit)
    : MuseumException("Climate violation for " + parameterName, "CLIMATE_ALERT", kClimateErrorCode),
      actualValue_(actualValue),
      allowedLimit_(allowedLimit) {}

double ClimateOutOfRangeException::actualValue() const noexcept {
    return actualValue_;
}

double ClimateOutOfRangeException::allowedLimit() const noexcept {
    return allowedLimit_;
}

TicketSoldOutException::TicketSoldOutException(const std::string& eventTitle)
    : MuseumException("Sold out: " + eventTitle, "TICKET_SOLD_OUT", kTicketSoldOutCode) {}

InvalidTicketException::InvalidTicketException(const std::string& ticketNumber)
    : MuseumException("Invalid or already used ticket: " + ticketNumber, "INVALID_TICKET", kInvalidTicketCode) {}

InsufficientFundsException::InsufficientFundsException(double requiredAmount, double availableBalance)
    : MuseumException("Insufficient funds: required " + std::to_string(requiredAmount) +
                      ", available " + std::to_string(availableBalance),
                      "INSUFFICIENT_FUNDS", kInsufficientFundsCode),
      requiredAmount_(requiredAmount),
      availableBalance_(availableBalance) {}

double InsufficientFundsException::requiredAmount() const noexcept {
    return requiredAmount_;
}

double InsufficientFundsException::availableBalance() const noexcept {
    return availableBalance_;
}

RestorationRequiredException::RestorationRequiredException(const std::string& artifactTitle,
                                                           double currentCondition)
    : MuseumException("Artifact requires restoration: " + artifactTitle,
                      "RESTORATION_REQUIRED", kRestorationRequiredCode),
      currentCondition_(currentCondition) {}

double RestorationRequiredException::currentCondition() const noexcept {
    return currentCondition_;
}

SecurityBreachException::SecurityBreachException(const std::string& securityZone)
    : MuseumException("Security breach in zone: " + securityZone, "SECURITY_BREACH", kSecurityBreachCode) {}

StaffNotAvailableException::StaffNotAvailableException(const std::string& staffName)
    : MuseumException("Staff member not available or unqualified: " + staffName,
                      "STAFF_UNAVAILABLE", kStaffUnavailableCode) {}

ExhibitionClosedException::ExhibitionClosedException(const std::string& exhibitionTitle)
    : MuseumException("Exhibition is currently closed: " + exhibitionTitle,
                      "EXHIBITION_CLOSED", kExhibitionClosedCode) {}

LoanAgreementViolationException::LoanAgreementViolationException(const std::string& reason)
    : MuseumException("Loan agreement violation: " + reason, "LOAN_VIOLATION", kLoanViolationCode) {}
