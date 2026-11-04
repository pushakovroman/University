/**
 * @file StaffAndHR.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы персонала музея (сотрудники, кураторы, гиды, реставраторы, охрана, отдел кадров).
 * @details Содержит 6 классов: Employee, Curator, TourGuide,
 * Restorer, SecurityGuard, HRDepartment.
 */

#ifndef MUSEUM_STAFF_AND_HR_H
#define MUSEUM_STAFF_AND_HR_H

#include "Artifacts.h"
#include "ExhibitionsAndEvents.h"
#include "HallsAndClimate.h"
#include "MuseumExceptions.h"
#include <string>
#include <vector>

/**
 * @class Employee
 * @brief Базовый класс штатного сотрудника музея.
 */
class Employee {
public:
    Employee(const std::string& employeeId,
             const std::string& fullName,
             const std::string& position,
             double baseSalary,
             int yearsOfExperience);
    virtual ~Employee() = default;

    void startShift();
    void endShift();
    virtual double calculateMonthlySalary() const;

    const std::string& employeeId() const noexcept { return employeeId_; }
    const std::string& fullName() const noexcept { return fullName_; }
    const std::string& position() const noexcept { return position_; }
    double baseSalary() const noexcept { return baseSalary_; }
    int yearsOfExperience() const noexcept { return yearsOfExperience_; }
    bool isOnDuty() const noexcept { return isOnDuty_; }

private:
    std::string employeeId_;
    std::string fullName_;
    std::string position_;
    double baseSalary_;
    int yearsOfExperience_;
    bool isOnDuty_;
};

/**
 * @class Curator
 * @brief Научный куратор (хранитель музейных фондов и выставок).
 */
class Curator : public Employee {
public:
    Curator(const std::string& employeeId,
            const std::string& fullName,
            double baseSalary,
            int yearsOfExperience,
            const std::string& specializationEra,
            const std::string& academicDegree = "PhD in Art History");

    bool canCurateArtifact(const Artifact& artifact) const;
    void organizeExhibition(Exhibition& exhibition,
                            const Artifact& artifact,
                            const ExhibitionHall& hall);
    bool evaluateArtifactCondition(const Artifact& artifact) const;

    int curatedExhibitionsCount() const noexcept { return curatedExhibitionsCount_; }
    const std::string& specializationEra() const noexcept { return specializationEra_; }

private:
    std::string specializationEra_;
    int curatedExhibitionsCount_;
    std::string academicDegree_;
};

/**
 * @class TourGuide
 * @brief Экскурсовод музея.
 */
class TourGuide : public Employee {
public:
    TourGuide(const std::string& employeeId,
              const std::string& fullName,
              double baseSalary,
              int yearsOfExperience,
              const std::vector<std::string>& spokenLanguages);

    bool speaksLanguage(const std::string& language) const;
    void leadGuidedTour(GuidedTour& tour);
    void updateRating(double newReviewScore);

    int conductedToursCount() const noexcept { return conductedToursCount_; }
    double guideRating() const noexcept { return guideRating_; }

private:
    std::vector<std::string> spokenLanguages_;
    int conductedToursCount_;
    double guideRating_;
};

/**
 * @class Restorer
 * @brief Художник-реставратор музейной лаборатории.
 */
class Restorer : public Employee {
public:
    Restorer(const std::string& employeeId,
             const std::string& fullName,
             double baseSalary,
             int yearsOfExperience,
             const std::string& specialtyMaterial,
             int certificationLevel = 3);

    bool isQualifiedFor(const Artifact& artifact) const;
    void performRestoration(Artifact& artifact, double repairAmount);
    void upgradeCertification();

    int certificationLevel() const noexcept { return certificationLevel_; }
    int restoredItemsCount() const noexcept { return restoredItemsCount_; }

private:
    std::string specialtyMaterial_;
    int certificationLevel_;
    int restoredItemsCount_;
};

/**
 * @class SecurityGuard
 * @brief Сотрудник внутренней службы безопасности музея.
 */
class SecurityGuard : public Employee {
public:
    SecurityGuard(const std::string& employeeId,
                  const std::string& fullName,
                  double baseSalary,
                  int yearsOfExperience,
                  const std::string& licenseNumber,
                  bool hasRadio = true);

    void assignToHall(const ExhibitionHall& hall);
    void inspectDisplayCase(DisplayCase& displayCase);
    void respondToIncident();

    int assignedHallNumber() const noexcept { return assignedHallNumber_; }
    int resolvedIncidentsCount() const noexcept { return resolvedIncidentsCount_; }

private:
    int assignedHallNumber_;
    std::string licenseNumber_;
    bool hasRadio_;
    int resolvedIncidentsCount_;
};

/**
 * @class HRDepartment
 * @brief Отдел кадров музея.
 */
class HRDepartment {
public:
    HRDepartment(const std::string& departmentHead = "Elena Volkova",
                 int maxStaffQuota = 50);

    void hireEmployee(const Employee& employee);
    void dismissEmployee(const std::string& employeeId);
    int countStaffOnDuty() const;
    double calculateTotalPayroll() const;

    std::size_t staffCount() const noexcept { return staffRoster_.size(); }
    const std::vector<Employee>& staffRoster() const noexcept { return staffRoster_; }

private:
    std::string departmentHead_;
    int maxStaffQuota_;
    std::vector<Employee> staffRoster_;
};

#endif // MUSEUM_STAFF_AND_HR_H
