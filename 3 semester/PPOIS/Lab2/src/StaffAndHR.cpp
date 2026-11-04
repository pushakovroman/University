/**
 * @file StaffAndHR.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов персонала музея и отдела кадров.
 */

#include "StaffAndHR.h"

#include <algorithm>

namespace {
constexpr double kAnnualExperienceBonusRate = 0.03;
constexpr double kMaxExperienceBonusRate = 0.60;
constexpr double kMasterpieceValueThreshold = 100000.0;
constexpr int kMinLevelForMasterpiece = 4;
constexpr int kMaxCertificationLevel = 5;
} // namespace

Employee::Employee(const std::string& employeeId,
                   const std::string& fullName,
                   const std::string& position,
                   double baseSalary,
                   int yearsOfExperience)
    : employeeId_(employeeId),
      fullName_(fullName),
      position_(position),
      baseSalary_(std::max(0.0, baseSalary)),
      yearsOfExperience_(std::max(0, yearsOfExperience)),
      isOnDuty_(false) {}

void Employee::startShift() {
    isOnDuty_ = true;
}

void Employee::endShift() {
    isOnDuty_ = false;
}

double Employee::calculateMonthlySalary() const {
    double bonusFactor = std::min(kMaxExperienceBonusRate,
                                  static_cast<double>(yearsOfExperience_) * kAnnualExperienceBonusRate);
    return baseSalary_ * (1.0 + bonusFactor);
}

Curator::Curator(const std::string& employeeId,
                 const std::string& fullName,
                 double baseSalary,
                 int yearsOfExperience,
                 const std::string& specializationEra,
                 const std::string& academicDegree)
    : Employee(employeeId, fullName, "Curator", baseSalary, yearsOfExperience),
      specializationEra_(specializationEra),
      curatedExhibitionsCount_(0),
      academicDegree_(academicDegree) {}

bool Curator::canCurateArtifact(const Artifact& artifact) const {
    return artifact.era().name() == specializationEra_ || specializationEra_ == "Universal";
}

void Curator::organizeExhibition(Exhibition& exhibition,
                                 const Artifact& artifact,
                                 const ExhibitionHall& hall) {
    if (!isOnDuty() || !canCurateArtifact(artifact)) {
        throw StaffNotAvailableException(fullName());
    }
    exhibition.addExhibit(artifact, hall);
    ++curatedExhibitionsCount_;
}

bool Curator::evaluateArtifactCondition(const Artifact& artifact) const {
    return !artifact.needsRestoration();
}

TourGuide::TourGuide(const std::string& employeeId,
                     const std::string& fullName,
                     double baseSalary,
                     int yearsOfExperience,
                     const std::vector<std::string>& spokenLanguages)
    : Employee(employeeId, fullName, "TourGuide", baseSalary, yearsOfExperience),
      spokenLanguages_(spokenLanguages),
      conductedToursCount_(0),
      guideRating_(5.0) {}

bool TourGuide::speaksLanguage(const std::string& language) const {
    for (const std::string& lang : spokenLanguages_) {
        if (lang == language) {
            return true;
        }
    }
    return false;
}

void TourGuide::leadGuidedTour(GuidedTour& tour) {
    if (!isOnDuty() || !speaksLanguage(tour.language())) {
        throw StaffNotAvailableException(fullName());
    }
    tour.conductTour();
    ++conductedToursCount_;
}

void TourGuide::updateRating(double newReviewScore) {
    double clamped = std::clamp(newReviewScore, 1.0, 5.0);
    guideRating_ = (guideRating_ + clamped) / 2.0;
}

Restorer::Restorer(const std::string& employeeId,
                   const std::string& fullName,
                   double baseSalary,
                   int yearsOfExperience,
                   const std::string& specialtyMaterial,
                   int certificationLevel)
    : Employee(employeeId, fullName, "Restorer", baseSalary, yearsOfExperience),
      specialtyMaterial_(specialtyMaterial),
      certificationLevel_(std::clamp(certificationLevel, 1, kMaxCertificationLevel)),
      restoredItemsCount_(0) {}

bool Restorer::isQualifiedFor(const Artifact& artifact) const {
    if (artifact.estimatedValue() >= kMasterpieceValueThreshold) {
        return certificationLevel_ >= kMinLevelForMasterpiece;
    }
    return certificationLevel_ >= 1;
}

void Restorer::performRestoration(Artifact& artifact, double repairAmount) {
    if (!isOnDuty() || !isQualifiedFor(artifact)) {
        throw StaffNotAvailableException(fullName());
    }
    artifact.repairCondition(repairAmount);
    ++restoredItemsCount_;
}

void Restorer::upgradeCertification() {
    certificationLevel_ = std::min(kMaxCertificationLevel, certificationLevel_ + 1);
}

SecurityGuard::SecurityGuard(const std::string& employeeId,
                             const std::string& fullName,
                             double baseSalary,
                             int yearsOfExperience,
                             const std::string& licenseNumber,
                             bool hasRadio)
    : Employee(employeeId, fullName, "SecurityGuard", baseSalary, yearsOfExperience),
      assignedHallNumber_(0),
      licenseNumber_(licenseNumber),
      hasRadio_(hasRadio),
      resolvedIncidentsCount_(0) {}

void SecurityGuard::assignToHall(const ExhibitionHall& hall) {
    assignedHallNumber_ = hall.hallNumber();
}

void SecurityGuard::inspectDisplayCase(DisplayCase& displayCase) {
    if (!isOnDuty()) {
        throw StaffNotAvailableException(fullName());
    }
    displayCase.lockCase();
}

void SecurityGuard::respondToIncident() {
    if (!isOnDuty() || !hasRadio_) {
        throw StaffNotAvailableException(fullName());
    }
    ++resolvedIncidentsCount_;
}

HRDepartment::HRDepartment(const std::string& departmentHead, int maxStaffQuota)
    : departmentHead_(departmentHead),
      maxStaffQuota_(std::max(1, maxStaffQuota)),
      staffRoster_() {}

void HRDepartment::hireEmployee(const Employee& employee) {
    if (static_cast<int>(staffRoster_.size()) >= maxStaffQuota_) {
        throw HallOvercapacityException("HRDepartment", maxStaffQuota_);
    }
    staffRoster_.push_back(employee);
}

void HRDepartment::dismissEmployee(const std::string& employeeId) {
    for (auto it = staffRoster_.begin(); it != staffRoster_.end(); ++it) {
        if (it->employeeId() == employeeId) {
            staffRoster_.erase(it);
            return;
        }
    }
    throw StaffNotAvailableException(employeeId);
}

int HRDepartment::countStaffOnDuty() const {
    int count = 0;
    for (const Employee& emp : staffRoster_) {
        if (emp.isOnDuty()) {
            ++count;
        }
    }
    return count;
}

double HRDepartment::calculateTotalPayroll() const {
    double total = 0.0;
    for (const Employee& emp : staffRoster_) {
        total += emp.calculateMonthlySalary();
    }
    return total;
}
