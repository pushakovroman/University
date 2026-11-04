/**
 * @file HallsAndClimate.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов залов, витрин, освещения, микроклимата и фондохранилища.
 */

#include "HallsAndClimate.h"

#include <algorithm>

namespace {
constexpr double kDefaultTempC = 20.0;
constexpr double kDefaultHumidity = 45.0;
constexpr double kDefaultLux = 120.0;
constexpr double kWattsPerLampAtFull = 18.0;
constexpr int kNightModeDimmingPercent = 15;
constexpr double kFullLuxAt100Percent = 200.0;
} // namespace

ClimateSensor::ClimateSensor(const std::string& sensorId,
                             double temperatureCelsius,
                             double humidityPercent,
                             double lightLux)
    : sensorId_(sensorId),
      temperatureCelsius_(temperatureCelsius),
      humidityPercent_(humidityPercent),
      lightLux_(lightLux),
      isOnline_(true) {}

void ClimateSensor::updateReadings(double tempC, double humidity, double lux) {
    temperatureCelsius_ = tempC;
    humidityPercent_ = humidity;
    lightLux_ = lux;
    isOnline_ = true;
}

bool ClimateSensor::isWithinNorms(double minTemp, double maxTemp, double maxHumidity, double maxLux) const {
    return isOnline_ &&
           temperatureCelsius_ >= minTemp &&
           temperatureCelsius_ <= maxTemp &&
           humidityPercent_ <= maxHumidity &&
           lightLux_ <= maxLux;
}

void ClimateSensor::calibrateSensor() {
    temperatureCelsius_ = kDefaultTempC;
    humidityPercent_ = kDefaultHumidity;
    lightLux_ = kDefaultLux;
    isOnline_ = true;
}

ClimateControlUnit::ClimateControlUnit(const std::string& unitId,
                                       double targetTempCelsius,
                                       double targetHumidityPercent,
                                       double maxLightLux)
    : unitId_(unitId),
      targetTempCelsius_(targetTempCelsius),
      targetHumidityPercent_(targetHumidityPercent),
      maxLightLux_(maxLightLux),
      isActive_(true) {}

void ClimateControlUnit::adjustToSensor(ClimateSensor& sensor) {
    isActive_ = true;
    sensor.updateReadings(targetTempCelsius_, targetHumidityPercent_, maxLightLux_ * 0.8);
}

void ClimateControlUnit::verifyArtifactSafety(const AncientManuscript& manuscript,
                                              const ClimateSensor& sensor) const {
    if (!manuscript.isHumiditySafe(sensor.humidityPercent())) {
        throw ClimateOutOfRangeException("Humidity", sensor.humidityPercent(), manuscript.maxHumidityPercent());
    }
}

void ClimateControlUnit::verifyPaintingLight(const Painting& painting, const ClimateSensor& sensor) const {
    if (!painting.isLightSafe(sensor.lightLux())) {
        throw ClimateOutOfRangeException("LightLux", sensor.lightLux(), painting.maxLightLux());
    }
}

DisplayCase::DisplayCase(const std::string& caseId,
                         const Dimensions& maxDimensions,
                         bool hasAlarm,
                         bool isHermetic)
    : caseId_(caseId),
      maxDimensions_(maxDimensions),
      hasAlarm_(hasAlarm),
      isHermetic_(isHermetic),
      isLocked_(false),
      placedArtifact_(nullptr) {}

void DisplayCase::mountArtifact(Artifact& artifact) {
    if (artifact.needsRestoration()) {
        throw RestorationRequiredException(artifact.title(), artifact.conditionPercent());
    }
    if (!artifact.dimensions().fitsWithin(maxDimensions_)) {
        throw InvalidArtifactDataException("Artifact exceeds display case dimensions");
    }
    placedArtifact_ = &artifact;
    artifact.setOnDisplay(true);
}

void DisplayCase::unmountArtifact() {
    if (placedArtifact_ != nullptr) {
        placedArtifact_->setOnDisplay(false);
        placedArtifact_ = nullptr;
    }
    isLocked_ = false;
}

void DisplayCase::lockCase() {
    isLocked_ = true;
}

bool DisplayCase::isSuitableForFragile() const {
    return hasAlarm_ && isHermetic_;
}

LightingSystem::LightingSystem(const std::string& systemId,
                               int lampCount,
                               double currentLux,
                               int dimmingPercent)
    : systemId_(systemId),
      lampCount_(std::max(1, lampCount)),
      currentLux_(currentLux),
      dimmingPercent_(std::clamp(dimmingPercent, 0, 100)) {}

void LightingSystem::setDimming(int percent) {
    dimmingPercent_ = std::clamp(percent, 0, 100);
    currentLux_ = kFullLuxAt100Percent * (static_cast<double>(dimmingPercent_) / 100.0);
}

double LightingSystem::calculatePowerConsumptionWatts() const {
    return static_cast<double>(lampCount_) * kWattsPerLampAtFull * (static_cast<double>(dimmingPercent_) / 100.0);
}

void LightingSystem::activateNightMode() {
    setDimming(kNightModeDimmingPercent);
}

ExhibitionHall::ExhibitionHall(int hallNumber,
                               const std::string& hallName,
                               int floorNumber,
                               double areaSqMeters,
                               int maxVisitors,
                               double maxFloorLoadKgPerSqM)
    : hallNumber_(hallNumber),
      hallName_(hallName),
      floorNumber_(floorNumber),
      areaSqMeters_(areaSqMeters),
      maxVisitors_(maxVisitors),
      currentVisitors_(0),
      maxFloorLoadKgPerSqM_(maxFloorLoadKgPerSqM) {}

void ExhibitionHall::admitVisitors(int count) {
    if (count < 0 || currentVisitors_ + count > maxVisitors_) {
        throw HallOvercapacityException(hallName_, maxVisitors_);
    }
    currentVisitors_ += count;
}

void ExhibitionHall::evacuateVisitors() {
    currentVisitors_ = 0;
}

bool ExhibitionHall::canAccommodateSculpture(const Sculpture& sculpture) const {
    return sculpture.verifyFloorCapacity(maxFloorLoadKgPerSqM_);
}

double ExhibitionHall::calculateOccupancyPercent() const {
    if (maxVisitors_ <= 0) {
        return 0.0;
    }
    return (static_cast<double>(currentVisitors_) / static_cast<double>(maxVisitors_)) * 100.0;
}

StorageVault::StorageVault(const std::string& vaultId,
                           int maxCapacity,
                           int securityLevel,
                           const ClimateControlUnit& climateUnit)
    : vaultId_(vaultId),
      maxCapacity_(maxCapacity),
      securityLevel_(securityLevel),
      climateUnit_(climateUnit),
      storedArtifacts_() {}

void StorageVault::depositArtifact(Artifact& artifact) {
    if (static_cast<int>(storedArtifacts_.size()) >= maxCapacity_) {
        throw HallOvercapacityException(vaultId_, maxCapacity_);
    }
    artifact.setOnDisplay(false);
    storedArtifacts_.push_back(&artifact);
}

Artifact* StorageVault::withdrawArtifact(const std::string& inventoryNumber) {
    for (auto it = storedArtifacts_.begin(); it != storedArtifacts_.end(); ++it) {
        if ((*it)->inventoryNumber() == inventoryNumber) {
            Artifact* found = *it;
            storedArtifacts_.erase(it);
            return found;
        }
    }
    throw ArtifactNotFoundException(inventoryNumber);
}

bool StorageVault::containsArtifact(const std::string& inventoryNumber) const {
    for (const Artifact* item : storedArtifacts_) {
        if (item->inventoryNumber() == inventoryNumber) {
            return true;
        }
    }
    return false;
}

double StorageVault::calculateTotalStoredValue() const {
    double sum = 0.0;
    for (const Artifact* item : storedArtifacts_) {
        sum += item->estimatedValue();
    }
    return sum;
}
