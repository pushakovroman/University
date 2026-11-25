/**
 * @file RestorationLab.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы реставрационной мастерской, материалов, диагностики и нарядов.
 * @details Содержит 4 класса: RestorationMaterial, ConditionReport,
 * RestorationTask, RestorationWorkshop.
 */

#ifndef MUSEUM_RESTORATION_LAB_H
#define MUSEUM_RESTORATION_LAB_H

#include "Artifacts.h"
#include "MuseumExceptions.h"
#include "StaffAndHR.h"
#include <string>
#include <vector>

/**
 * @class RestorationMaterial
 * @brief Расходный реставрационный материал (лак, пигмент, клей, растворитель).
 * @details Учёт реставрационных расходников (лаки, пигменты, клеи) в граммах и оценка складской стоимости.
 * @see RestorationTask, RestorationWorkshop
 */
class RestorationMaterial {
public:
    /**
     * @brief Конструктор класса RestorationMaterial.
     * @param materialName Наименование реставрационного материала.
     * @param quantityGrams Количество материала на складе в граммах (>= 0).
     * @param costPerGram Стоимость одного грамма материала (>= 0).
     * @param isToxic Признак токсичности реагента.
     */
    RestorationMaterial(const std::string& materialName = "Dammar Varnish",
                        double quantityGrams = 500.0,
                        double costPerGram = 0.8,
                        bool isToxic = false);

    /**
     * @brief Списывает gramsNeeded материала со склада лаборатории при достаточном остатке.
     * @param gramsNeeded Требуемая масса расходника в граммах (> 0).
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     */
    void consumeMaterial(double gramsNeeded);
    /**
     * @brief Пополняет запас расходного материала на gramsAdded граммов.
     * @param gramsAdded Масса пополняемого запаса в граммах (> 0).
     */
    void restockMaterial(double gramsAdded);
    /**
     * @brief Возвращает суммарную стоимость текущего остатка: quantityGrams_ * costPerGram_.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateStockValue() const;

    /// @brief Возвращает наименование реставрационного материала.
    const std::string& materialName() const noexcept { return materialName_; }
    /// @brief Возвращает остаток материала на складе (г).
    double quantityGrams() const noexcept { return quantityGrams_; }
    /// @brief Возвращает цену одного грамма материала.
    double costPerGram() const noexcept { return costPerGram_; }

private:
    std::string materialName_; ///< Наименование реставрационного материала.
    double quantityGrams_; ///< Количество материала на складе в граммах (>= 0).
    double costPerGram_; ///< Стоимость одного грамма материала (>= 0).
    bool isToxic_; ///< Признак токсичности реагента.
};

/**
 * @class ConditionReport
 * @brief Экспертный акт осмотра технического состояния экспоната.
 * @details Экспертный акт технического осмотра экспоната: оценивает срочность (1–5), трудозатраты и пригодность к перевозке.
 * @see Artifact
 */
class ConditionReport {
public:
    /**
     * @brief Конструктор класса ConditionReport.
     * @param reportId Регистрационный номер акта технического осмотра.
     * @param artifactId Инвентарный номер экспоната.
     * @param inspectorName Имя эксперта, проводившего осмотр.
     * @param recordedCondition Зафиксированный процент сохранности [0..100].
     * @param hasStructuralCracks Признак наличия структурных трещин/разрушений.
     */
    ConditionReport(const std::string& reportId,
                    const std::string& artifactId,
                    const std::string& inspectorName,
                    double recordedCondition,
                    bool hasStructuralCracks);

    /**
     * @brief Оценивает срочность реставрации по шкале 1–5 (трещины или <25% -> 5.
     * @details <50% -> 4; <75% -> 3; иначе 1).
     * @return Значение типа @c int, вычисленное методом.
     */
    int assessUrgencyLevel() const;
    /**
     * @brief Рекомендует трудозатраты в часах: (100 - condition) * 0.8 + (20 ч при наличии трещин).
     * @return Значение типа @c double, вычисленное методом.
     */
    double recommendConservationHours() const;
    /**
     * @brief Подтверждает пригодность к транспортировке: нет структурных трещин и сохранность >= 65%.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isFitForTransport() const;

    /// @brief Возвращает номер акта осмотра.
    const std::string& reportId() const noexcept { return reportId_; }
    /// @brief Возвращает зафиксированный процент сохранности.
    double recordedCondition() const noexcept { return recordedCondition_; }

private:
    std::string reportId_; ///< Регистрационный номер акта технического осмотра.
    std::string artifactId_; ///< Инвентарный номер экспоната.
    std::string inspectorName_; ///< Имя эксперта, проводившего осмотр.
    double recordedCondition_; ///< Зафиксированный процент сохранности [0..100].
    bool hasStructuralCracks_; ///< Признак наличия структурных трещин/разрушений.
};

/**
 * @class RestorationTask
 * @brief Наряд-задание на проведение реставрации экспоната.
 * @details Наряд-задание на консервацию/реставрацию, координирующий списание материала и работу реставратора.
 * @see Artifact, Restorer, RestorationMaterial, RestorationWorkshop
 */
class RestorationTask {
public:
    /**
     * @brief Конструктор класса RestorationTask.
     * @param taskId Идентификатор наряда-задания на реставрацию.
     * @param artifactId Инвентарный номер экспоната.
     * @param assignedRestorerId Табельный номер назначенного реставратора.
     * @param targetCondition Целевой процент сохранности после реставрации [50..100].
     * @param estimatedCost Предварительная сметная стоимость работ.
     */
    RestorationTask(const std::string& taskId,
                    const std::string& artifactId,
                    const std::string& assignedRestorerId,
                    double targetCondition,
                    double estimatedCost);

    /**
     * @brief Списывает расходник, восстанавливает состояние до targetCondition_ (минимум +10%) через реставратора и закрывает наряд.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @param restorer Реставратор, выполняющий или принимающий задание.
     * @param material Материал скульптуры или расходный реставрационный материал.
     * @param gramsNeeded Требуемая масса расходника в граммах (> 0).
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void executeTask(Artifact& artifact,
                     Restorer& restorer,
                     RestorationMaterial& material,
                     double gramsNeeded);
    /**
     * @brief Рассчитывает фактическую себестоимость: gramsUsed * costPerGram + laborHours * 45.0.
     * @param material Материал скульптуры или расходный реставрационный материал.
     * @param gramsUsed Фактически израсходованная масса материала (г).
     * @param laborHours Фактические трудозатраты в часах.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateActualCost(const RestorationMaterial& material,
                               double gramsUsed,
                               double laborHours) const;
    /**
     * @brief Возвращает статус завершения наряда-задания (isFinished_).
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isCompleted() const;

    /// @brief Возвращает код задания на реставрацию.
    const std::string& taskId() const noexcept { return taskId_; }
    /// @brief Возвращает сметную стоимость задания.
    double estimatedCost() const noexcept { return estimatedCost_; }

private:
    std::string taskId_; ///< Идентификатор наряда-задания на реставрацию.
    std::string artifactId_; ///< Инвентарный номер экспоната.
    std::string assignedRestorerId_; ///< Табельный номер назначенного реставратора.
    double targetCondition_; ///< Целевой процент сохранности после реставрации [50..100].
    double estimatedCost_; ///< Предварительная сметная стоимость работ.
    bool isFinished_; ///< Флаг завершения реставрационного задания.
};

/**
 * @class RestorationWorkshop
 * @brief Реставрационная мастерская (лаборатория) музея.
 * @details Консервационная мастерская, управляющая очередью нарядов с квотой одновременных задач.
 * @see RestorationTask, Artifact, Restorer, RestorationMaterial
 */
class RestorationWorkshop {
public:
    /**
     * @brief Конструктор класса RestorationWorkshop.
     * @param workshopName Наименование реставрационной мастерской.
     * @param maxConcurrentTasks Предельное число одновременно выполняемых задач (>= 1).
     * @param equipmentBudget Бюджет оборудования мастерской.
     */
    RestorationWorkshop(const std::string& workshopName = "Central Conservation Lab",
                        int maxConcurrentTasks = 10,
                        double equipmentBudget = 25000.0);

    /**
     * @brief Регистрирует наряд TASK-<inv> (цель 95%, смета 1200) с проверкой квоты очереди и квалификации мастера.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @param restorer Реставратор, выполняющий или принимающий задание.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void acceptForRestoration(const Artifact& artifact, const Restorer& restorer);
    /**
     * @brief Находит активный наряд по taskId и выполняет его со стандартным расходом 25 г материала.
     * @param taskId Идентификатор наряда-задания на реставрацию.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @param restorer Реставратор, выполняющий или принимающий задание.
     * @param material Материал скульптуры или расходный реставрационный материал.
     * @throw ArtifactNotFoundException При нарушении бизнес-правил или инвариантов операции.
     * @throw InsufficientFundsException При нарушении бизнес-правил или инвариантов операции.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void completeTaskById(const std::string& taskId,
                          Artifact& artifact,
                          Restorer& restorer,
                          RestorationMaterial& material);
    /**
     * @brief Подсчитывает число незавершённых заданий в очереди мастерской.
     * @return Значение типа @c int, вычисленное методом.
     */
    int countPendingTasks() const;

    /// @brief Возвращает общее число заданий в мастерской.
    std::size_t totalTasks() const noexcept { return activeTasks_.size(); }

private:
    std::string workshopName_; ///< Наименование реставрационной мастерской.
    int maxConcurrentTasks_; ///< Предельное число одновременно выполняемых задач (>= 1).
    double equipmentBudget_; ///< Бюджет оборудования мастерской.
    std::vector<RestorationTask> activeTasks_; ///< Список нарядов-заданий мастерской.
};

#endif // MUSEUM_RESTORATION_LAB_H
