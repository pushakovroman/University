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
 * @details Базовый полиморфный класс сотрудника музея: хранит реквизиты, статус смены и рассчитывает оклад со стажевой надбавкой.
 * @see Curator, TourGuide, Restorer, SecurityGuard, HRDepartment
 */
class Employee {
public:
    /**
     * @brief Конструктор класса Employee.
     * @param employeeId Табельный идентификатор сотрудника.
     * @param fullName Полное имя автора, сотрудника или посетителя.
     * @param position Наименование должности сотрудника.
     * @param baseSalary Базовый месячный оклад до надбавок (>= 0).
     * @param yearsOfExperience Профессиональный стаж работы в годах (>= 0).
     */
    Employee(const std::string& employeeId,
             const std::string& fullName,
             const std::string& position,
             double baseSalary,
             int yearsOfExperience);
    /// @brief Виртуальный деструктор по умолчанию.
    virtual ~Employee() = default;

    /**
     * @brief Открывает рабочую смену сотрудника (isOnDuty_ = true).
     */
    void startShift();
    /**
     * @brief Завершает рабочую смену сотрудника (isOnDuty_ = false).
     */
    void endShift();
    /**
     * @brief Рассчитывает месячный оклад с надбавкой +3% за каждый год стажа (максимум +60%).
     * @return Значение типа @c double, вычисленное методом.
     */
    virtual double calculateMonthlySalary() const;

    /// @brief Возвращает табельный номер сотрудника.
    const std::string& employeeId() const noexcept { return employeeId_; }
    /// @brief Возвращает полное имя.
    const std::string& fullName() const noexcept { return fullName_; }
    /// @brief Возвращает должность сотрудника.
    const std::string& position() const noexcept { return position_; }
    /// @brief Возвращает базовый месячный оклад.
    double baseSalary() const noexcept { return baseSalary_; }
    /// @brief Возвращает стаж работы в годах.
    int yearsOfExperience() const noexcept { return yearsOfExperience_; }
    /// @brief Возвращает true, если сотрудник на смене.
    bool isOnDuty() const noexcept { return isOnDuty_; }

private:
    std::string employeeId_; ///< Табельный идентификатор сотрудника.
    std::string fullName_; ///< Полное имя автора, сотрудника или посетителя.
    std::string position_; ///< Наименование должности сотрудника.
    double baseSalary_; ///< Базовый месячный оклад до надбавок (>= 0).
    int yearsOfExperience_; ///< Профессиональный стаж работы в годах (>= 0).
    bool isOnDuty_; ///< Признак нахождения сотрудника на смене.
};

/**
 * @class Curator
 * @brief Научный куратор (хранитель музейных фондов и выставок).
 * @details Научный куратор, проверяющий соответствие эпохи и организующий размещение экспонатов на выставках.
 * @see Employee, Artifact, Exhibition, ExhibitionHall
 */
class Curator : public Employee {
public:
    /**
     * @brief Конструктор класса Curator.
     * @param employeeId Табельный идентификатор сотрудника.
     * @param fullName Полное имя автора, сотрудника или посетителя.
     * @param baseSalary Базовый месячный оклад до надбавок (>= 0).
     * @param yearsOfExperience Профессиональный стаж работы в годах (>= 0).
     * @param specializationEra Историческая эпоха специализации куратора (или "Universal").
     * @param academicDegree Учёная степень куратора.
     */
    Curator(const std::string& employeeId,
            const std::string& fullName,
            double baseSalary,
            int yearsOfExperience,
            const std::string& specializationEra,
            const std::string& academicDegree = "PhD in Art History");

    /**
     * @brief Проверяет соответствие эпохи экспоната специализации куратора (или универсальный профиль "Universal").
     * @param artifact Музейный экспонат, участвующий в операции.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool canCurateArtifact(const Artifact& artifact) const;
    /**
     * @brief Добавляет экспонат на выставку, проверяя смену, специализацию куратора и правила выставки.
     * @param exhibition Выставка для добавления в расписание, организации или продажи билета.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @param hall Выставочный зал, задействованный в операции.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     * @throw RestorationRequiredException При нарушении бизнес-правил или инвариантов операции.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void organizeExhibition(Exhibition& exhibition,
                            const Artifact& artifact,
                            const ExhibitionHall& hall);
    /**
     * @brief Экспертно подтверждает пригодность экспоната к показу (!artifact.needsRestoration()).
     * @param artifact Музейный экспонат, участвующий в операции.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool evaluateArtifactCondition(const Artifact& artifact) const;

    /// @brief Возвращает число организованных выставок.
    int curatedExhibitionsCount() const noexcept { return curatedExhibitionsCount_; }
    /// @brief Возвращает профильную эпоху куратора.
    const std::string& specializationEra() const noexcept { return specializationEra_; }

private:
    std::string specializationEra_; ///< Историческая эпоха специализации куратора (или "Universal").
    int curatedExhibitionsCount_; ///< Счётчик организованных куратором выставок.
    std::string academicDegree_; ///< Учёная степень куратора.
};

/**
 * @class TourGuide
 * @brief Экскурсовод музея.
 * @details Штатный экскурсовод, владеющий набором языков и проводящий групповые экскурсии с накопительным рейтингом.
 * @see Employee, GuidedTour
 */
class TourGuide : public Employee {
public:
    /**
     * @brief Конструктор класса TourGuide.
     * @param employeeId Табельный идентификатор сотрудника.
     * @param fullName Полное имя автора, сотрудника или посетителя.
     * @param baseSalary Базовый месячный оклад до надбавок (>= 0).
     * @param yearsOfExperience Профессиональный стаж работы в годах (>= 0).
     * @param spokenLanguages Перечень рабочих языков экскурсовода.
     */
    TourGuide(const std::string& employeeId,
              const std::string& fullName,
              double baseSalary,
              int yearsOfExperience,
              const std::vector<std::string>& spokenLanguages);

    /**
     * @brief Проверяет наличие языка language в списке spokenLanguages_ экскурсовода.
     * @param language Язык рукописи, экскурсии или озвучки аудиогида.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool speaksLanguage(const std::string& language) const;
    /**
     * @brief Проводит экскурсию при условии нахождения на смене и владения языком тура.
     * @param tour Групповая экскурсия для проведения.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     * @throw ExhibitionClosedException При нарушении бизнес-правил или инвариантов операции.
     */
    void leadGuidedTour(GuidedTour& tour);
    /**
     * @brief Пересчитывает рейтинг гида как среднее текущего значения и новой оценки (ограниченной [1.0, 5.0]).
     * @param newReviewScore Новая оценка посетителей в диапазоне [1.0, 5.0].
     */
    void updateRating(double newReviewScore);

    /// @brief Возвращает число проведённых экскурсий.
    int conductedToursCount() const noexcept { return conductedToursCount_; }
    /// @brief Возвращает текущий рейтинг экскурсовода.
    double guideRating() const noexcept { return guideRating_; }

private:
    std::vector<std::string> spokenLanguages_; ///< Перечень рабочих языков экскурсовода.
    int conductedToursCount_; ///< Счётчик проведённых экскурсий.
    double guideRating_; ///< Текущий средний рейтинг экскурсовода (1.0–5.0).
};

/**
 * @class Restorer
 * @brief Художник-реставратор музейной лаборатории.
 * @details Художник-реставратор с квалификационным разрядом 1–5; для шедевров (>= 100 000) требуется разряд >= 4.
 * @see Employee, Artifact, RestorationTask, RestorationWorkshop
 */
class Restorer : public Employee {
public:
    /**
     * @brief Конструктор класса Restorer.
     * @param employeeId Табельный идентификатор сотрудника.
     * @param fullName Полное имя автора, сотрудника или посетителя.
     * @param baseSalary Базовый месячный оклад до надбавок (>= 0).
     * @param yearsOfExperience Профессиональный стаж работы в годах (>= 0).
     * @param specialtyMaterial Профильный материал реставратора.
     * @param certificationLevel Квалификационный разряд реставратора [1..5].
     */
    Restorer(const std::string& employeeId,
             const std::string& fullName,
             double baseSalary,
             int yearsOfExperience,
             const std::string& specialtyMaterial,
             int certificationLevel = 3);

    /**
     * @brief Проверяет допуск реставратора: для шедевров (>= 100 000) требуется разряд >= 4, для прочих >= 1.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isQualifiedFor(const Artifact& artifact) const;
    /**
     * @brief Восстанавливает состояние экспоната на repairAmount при активной смене и достаточной квалификации.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @param repairAmount Прирост процента сохранности в ходе реставрации.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     * @throw InvalidArtifactDataException При нарушении бизнес-правил или инвариантов операции.
     */
    void performRestoration(Artifact& artifact, double repairAmount);
    /**
     * @brief Повышает квалификационный разряд реставратора на 1 (максимум до 5-го).
     */
    void upgradeCertification();

    /// @brief Возвращает квалификационный разряд реставратора (1–5).
    int certificationLevel() const noexcept { return certificationLevel_; }
    /// @brief Возвращает число отреставрированных предметов.
    int restoredItemsCount() const noexcept { return restoredItemsCount_; }

private:
    std::string specialtyMaterial_; ///< Профильный материал реставратора.
    int certificationLevel_; ///< Квалификационный разряд реставратора [1..5].
    int restoredItemsCount_; ///< Счётчик отреставрированных мастером экспонатов.
};

/**
 * @class SecurityGuard
 * @brief Сотрудник внутренней службы безопасности музея.
 * @details Сотрудник службы безопасности с закреплением за залом, лицензией, рацией и учётом отработанных инцидентов.
 * @see Employee, ExhibitionHall, DisplayCase, AlarmTrigger, SecurityControlCenter
 */
class SecurityGuard : public Employee {
public:
    /**
     * @brief Конструктор класса SecurityGuard.
     * @param employeeId Табельный идентификатор сотрудника.
     * @param fullName Полное имя автора, сотрудника или посетителя.
     * @param baseSalary Базовый месячный оклад до надбавок (>= 0).
     * @param yearsOfExperience Профессиональный стаж работы в годах (>= 0).
     * @param licenseNumber Номер охранной лицензии сотрудника.
     * @param hasRadio Наличие исправной радиостанции на дежурстве.
     */
    SecurityGuard(const std::string& employeeId,
                  const std::string& fullName,
                  double baseSalary,
                  int yearsOfExperience,
                  const std::string& licenseNumber,
                  bool hasRadio = true);

    /**
     * @brief Назначает охранника на пост в указанном выставочном зале.
     * @param hall Выставочный зал, задействованный в операции.
     */
    void assignToHall(const ExhibitionHall& hall);
    /**
     * @brief Осматривает и запирает витрину; требует активной смены сотрудника.
     * @param displayCase Осматриваемая и запираемая витрина.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void inspectDisplayCase(DisplayCase& displayCase);
    /**
     * @brief Фиксирует отработку инцидента; требует активной смены и наличия радиостанции.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void respondToIncident();

    /// @brief Возвращает номер закреплённого за охранником зала.
    int assignedHallNumber() const noexcept { return assignedHallNumber_; }
    /// @brief Возвращает число отработанных инцидентов.
    int resolvedIncidentsCount() const noexcept { return resolvedIncidentsCount_; }

private:
    int assignedHallNumber_; ///< Номер зала, закреплённого за охранником.
    std::string licenseNumber_; ///< Номер охранной лицензии сотрудника.
    bool hasRadio_; ///< Наличие исправной радиостанции на дежурстве.
    int resolvedIncidentsCount_; ///< Счётчик отработанных охранником инцидентов.
};

/**
 * @class HRDepartment
 * @brief Отдел кадров музея.
 * @details Отдел кадров музея: ведёт штатный реестр с квотой, считает сотрудников на смене и общий месячный ФОТ.
 * @see Employee, Museum
 */
class HRDepartment {
public:
    /**
     * @brief Конструктор класса HRDepartment.
     * @param departmentHead ФИО руководителя отдела кадров.
     * @param maxStaffQuota Максимальная штатная численность музея (>= 1).
     */
    HRDepartment(const std::string& departmentHead = "Elena Volkova",
                 int maxStaffQuota = 50);

    /**
     * @brief Зачисляет сотрудника в штат с проверкой лимита квоты maxStaffQuota_.
     * @param employee Сотрудник, принимаемый в штат.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void hireEmployee(const Employee& employee);
    /**
     * @brief Увольняет сотрудника по employeeId из штатного списка.
     * @param employeeId Табельный идентификатор сотрудника.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void dismissEmployee(const std::string& employeeId);
    /**
     * @brief Подсчитывает количество штатных сотрудников, находящихся сейчас на смене.
     * @return Значение типа @c int, вычисленное методом.
     */
    int countStaffOnDuty() const;
    /**
     * @brief Рассчитывает суммарный месячный фонд оплаты труда всех сотрудников в штате.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateTotalPayroll() const;

    /// @brief Возвращает текущую численность штата.
    std::size_t staffCount() const noexcept { return staffRoster_.size(); }
    /// @brief Возвращает константный список сотрудников в штате.
    const std::vector<Employee>& staffRoster() const noexcept { return staffRoster_; }

private:
    std::string departmentHead_; ///< ФИО руководителя отдела кадров.
    int maxStaffQuota_; ///< Максимальная штатная численность музея (>= 1).
    std::vector<Employee> staffRoster_; ///< Штатный реестр сотрудников музея.
};

#endif // MUSEUM_STAFF_AND_HR_H
