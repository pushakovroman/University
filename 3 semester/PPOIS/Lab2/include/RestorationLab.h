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
 */
class RestorationMaterial {
public:
    RestorationMaterial(const std::string& materialName = "Dammar Varnish",
                        double quantityGrams = 500.0,
                        double costPerGram = 0.8,
                        bool isToxic = false);

    void consumeMaterial(double gramsNeeded);
    void restockMaterial(double gramsAdded);
    double calculateStockValue() const;

    const std::string& materialName() const noexcept { return materialName_; }
    double quantityGrams() const noexcept { return quantityGrams_; }
    double costPerGram() const noexcept { return costPerGram_; }

private:
    std::string materialName_;
    double quantityGrams_;
    double costPerGram_;
    bool isToxic_;
};

/**
 * @class ConditionReport
 * @brief Экспертный акт осмотра технического состояния экспоната.
 */
class ConditionReport {
public:
    ConditionReport(const std::string& reportId,
                    const std::string& artifactId,
                    const std::string& inspectorName,
                    double recordedCondition,
                    bool hasStructuralCracks);

    int assessUrgencyLevel() const;
    double recommendConservationHours() const;
    bool isFitForTransport() const;

    const std::string& reportId() const noexcept { return reportId_; }
    double recordedCondition() const noexcept { return recordedCondition_; }

private:
    std::string reportId_;
    std::string artifactId_;
    std::string inspectorName_;
    double recordedCondition_;
    bool hasStructuralCracks_;
};

/**
 * @class RestorationTask
 * @brief Наряд-задание на проведение реставрации экспоната.
 */
class RestorationTask {
public:
    RestorationTask(const std::string& taskId,
                    const std::string& artifactId,
                    const std::string& assignedRestorerId,
                    double targetCondition,
                    double estimatedCost);

    void executeTask(Artifact& artifact,
                     Restorer& restorer,
                     RestorationMaterial& material,
                     double gramsNeeded);
    double calculateActualCost(const RestorationMaterial& material,
                               double gramsUsed,
                               double laborHours) const;
    bool isCompleted() const;

    const std::string& taskId() const noexcept { return taskId_; }
    double estimatedCost() const noexcept { return estimatedCost_; }

private:
    std::string taskId_;
    std::string artifactId_;
    std::string assignedRestorerId_;
    double targetCondition_;
    double estimatedCost_;
    bool isFinished_;
};

/**
 * @class RestorationWorkshop
 * @brief Реставрационная мастерская (лаборатория) музея.
 */
class RestorationWorkshop {
public:
    RestorationWorkshop(const std::string& workshopName = "Central Conservation Lab",
                        int maxConcurrentTasks = 10,
                        double equipmentBudget = 25000.0);

    void acceptForRestoration(const Artifact& artifact, const Restorer& restorer);
    void completeTaskById(const std::string& taskId,
                          Artifact& artifact,
                          Restorer& restorer,
                          RestorationMaterial& material);
    int countPendingTasks() const;

    std::size_t totalTasks() const noexcept { return activeTasks_.size(); }

private:
    std::string workshopName_;
    int maxConcurrentTasks_;
    double equipmentBudget_;
    std::vector<RestorationTask> activeTasks_;
};

#endif // MUSEUM_RESTORATION_LAB_H
