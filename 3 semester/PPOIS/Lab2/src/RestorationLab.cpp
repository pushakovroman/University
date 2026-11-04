/**
 * @file RestorationLab.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов реставрационной мастерской, материалов и актов осмотра.
 */

#include "RestorationLab.h"

#include <algorithm>

namespace {
constexpr double kLaborHourlyRate = 45.0;
constexpr double kDefaultTaskGrams = 25.0;
constexpr double kDefaultTaskCost = 1200.0;
} // namespace

RestorationMaterial::RestorationMaterial(const std::string& materialName,
                                         double quantityGrams,
                                         double costPerGram,
                                         bool isToxic)
    : materialName_(materialName),
      quantityGrams_(std::max(0.0, quantityGrams)),
      costPerGram_(std::max(0.0, costPerGram)),
      isToxic_(isToxic) {}

void RestorationMaterial::consumeMaterial(double gramsNeeded) {
    if (gramsNeeded <= 0.0 || gramsNeeded > quantityGrams_) {
        throw InsufficientFundsException(gramsNeeded, quantityGrams_);
    }
    quantityGrams_ -= gramsNeeded;
}

void RestorationMaterial::restockMaterial(double gramsAdded) {
    if (gramsAdded > 0.0) {
        quantityGrams_ += gramsAdded;
    }
}

double RestorationMaterial::calculateStockValue() const {
    return quantityGrams_ * costPerGram_;
}

ConditionReport::ConditionReport(const std::string& reportId,
                                 const std::string& artifactId,
                                 const std::string& inspectorName,
                                 double recordedCondition,
                                 bool hasStructuralCracks)
    : reportId_(reportId),
      artifactId_(artifactId),
      inspectorName_(inspectorName),
      recordedCondition_(std::clamp(recordedCondition, 0.0, 100.0)),
      hasStructuralCracks_(hasStructuralCracks) {}

int ConditionReport::assessUrgencyLevel() const {
    if (hasStructuralCracks_ || recordedCondition_ < 25.0) {
        return 5;
    }
    if (recordedCondition_ < 50.0) {
        return 4;
    }
    if (recordedCondition_ < 75.0) {
        return 3;
    }
    return 1;
}

double ConditionReport::recommendConservationHours() const {
    double deficit = 100.0 - recordedCondition_;
    double crackExtra = hasStructuralCracks_ ? 20.0 : 0.0;
    return deficit * 0.8 + crackExtra;
}

bool ConditionReport::isFitForTransport() const {
    return !hasStructuralCracks_ && recordedCondition_ >= 65.0;
}

RestorationTask::RestorationTask(const std::string& taskId,
                                 const std::string& artifactId,
                                 const std::string& assignedRestorerId,
                                 double targetCondition,
                                 double estimatedCost)
    : taskId_(taskId),
      artifactId_(artifactId),
      assignedRestorerId_(assignedRestorerId),
      targetCondition_(std::clamp(targetCondition, 50.0, 100.0)),
      estimatedCost_(estimatedCost),
      isFinished_(false) {}

void RestorationTask::executeTask(Artifact& artifact,
                                  Restorer& restorer,
                                  RestorationMaterial& material,
                                  double gramsNeeded) {
    material.consumeMaterial(gramsNeeded);
    double pointsToRestore = std::max(10.0, targetCondition_ - artifact.conditionPercent());
    restorer.performRestoration(artifact, pointsToRestore);
    isFinished_ = true;
}

double RestorationTask::calculateActualCost(const RestorationMaterial& material,
                                            double gramsUsed,
                                            double laborHours) const {
    return gramsUsed * material.costPerGram() + laborHours * kLaborHourlyRate;
}

bool RestorationTask::isCompleted() const {
    return isFinished_;
}

RestorationWorkshop::RestorationWorkshop(const std::string& workshopName,
                                         int maxConcurrentTasks,
                                         double equipmentBudget)
    : workshopName_(workshopName),
      maxConcurrentTasks_(std::max(1, maxConcurrentTasks)),
      equipmentBudget_(equipmentBudget),
      activeTasks_() {}

void RestorationWorkshop::acceptForRestoration(const Artifact& artifact, const Restorer& restorer) {
    if (countPendingTasks() >= maxConcurrentTasks_) {
        throw HallOvercapacityException(workshopName_, maxConcurrentTasks_);
    }
    if (!restorer.isQualifiedFor(artifact)) {
        throw StaffNotAvailableException(restorer.fullName());
    }
    std::string id = "TASK-" + artifact.inventoryNumber();
    RestorationTask task(id, artifact.inventoryNumber(), restorer.employeeId(), 95.0, kDefaultTaskCost);
    activeTasks_.push_back(task);
}

void RestorationWorkshop::completeTaskById(const std::string& taskId,
                                           Artifact& artifact,
                                           Restorer& restorer,
                                           RestorationMaterial& material) {
    for (RestorationTask& task : activeTasks_) {
        if (task.taskId() == taskId && !task.isCompleted()) {
            task.executeTask(artifact, restorer, material, kDefaultTaskGrams);
            return;
        }
    }
    throw ArtifactNotFoundException(taskId);
}

int RestorationWorkshop::countPendingTasks() const {
    int pending = 0;
    for (const RestorationTask& task : activeTasks_) {
        if (!task.isCompleted()) {
            ++pending;
        }
    }
    return pending;
}
