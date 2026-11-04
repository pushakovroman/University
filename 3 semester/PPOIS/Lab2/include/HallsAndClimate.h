/**
 * @file HallsAndClimate.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы залов, витрин, освещения, микроклимата и фондохранилища музея.
 * @details Содержит 6 классов: ClimateSensor, ClimateControlUnit, DisplayCase,
 * LightingSystem, ExhibitionHall, StorageVault.
 */

#ifndef MUSEUM_HALLS_AND_CLIMATE_H
#define MUSEUM_HALLS_AND_CLIMATE_H

#include "Artifacts.h"
#include "MuseumExceptions.h"
#include <string>
#include <vector>

/**
 * @class ClimateSensor
 * @brief Датчик контроля температуры, влажности и освещённости в зале.
 */
class ClimateSensor {
public:
    ClimateSensor(const std::string& sensorId = "SNS-01",
                  double temperatureCelsius = 20.0,
                  double humidityPercent = 45.0,
                  double lightLux = 120.0);

    void updateReadings(double tempC, double humidity, double lux);
    bool isWithinNorms(double minTemp, double maxTemp, double maxHumidity, double maxLux) const;
    void calibrateSensor();

    const std::string& sensorId() const noexcept { return sensorId_; }
    double temperatureCelsius() const noexcept { return temperatureCelsius_; }
    double humidityPercent() const noexcept { return humidityPercent_; }
    double lightLux() const noexcept { return lightLux_; }
    bool isOnline() const noexcept { return isOnline_; }

private:
    std::string sensorId_;
    double temperatureCelsius_;
    double humidityPercent_;
    double lightLux_;
    bool isOnline_;
};

/**
 * @class ClimateControlUnit
 * @brief Климатическая установка для поддержания музейного микроклимата.
 */
class ClimateControlUnit {
public:
    ClimateControlUnit(const std::string& unitId = "CCU-01",
                       double targetTempCelsius = 20.0,
                       double targetHumidityPercent = 45.0,
                       double maxLightLux = 150.0);

    void adjustToSensor(ClimateSensor& sensor);
    void verifyArtifactSafety(const AncientManuscript& manuscript, const ClimateSensor& sensor) const;
    void verifyPaintingLight(const Painting& painting, const ClimateSensor& sensor) const;

    bool isActive() const noexcept { return isActive_; }
    double targetTempCelsius() const noexcept { return targetTempCelsius_; }
    double targetHumidityPercent() const noexcept { return targetHumidityPercent_; }

private:
    std::string unitId_;
    double targetTempCelsius_;
    double targetHumidityPercent_;
    double maxLightLux_;
    bool isActive_;
};

/**
 * @class DisplayCase
 * @brief Защищённая музейная витрина для экспонирования предметов.
 */
class DisplayCase {
public:
    DisplayCase(const std::string& caseId,
                const Dimensions& maxDimensions,
                bool hasAlarm = true,
                bool isHermetic = true);

    void mountArtifact(Artifact& artifact);
    void unmountArtifact();
    void lockCase();
    bool isSuitableForFragile() const;

    const std::string& caseId() const noexcept { return caseId_; }
    bool isLocked() const noexcept { return isLocked_; }
    const Artifact* placedArtifact() const noexcept { return placedArtifact_; }

private:
    std::string caseId_;
    Dimensions maxDimensions_;
    bool hasAlarm_;
    bool isHermetic_;
    bool isLocked_;
    Artifact* placedArtifact_;
};

/**
 * @class LightingSystem
 * @brief Интеллектуальная система освещения выставочного зала.
 */
class LightingSystem {
public:
    LightingSystem(const std::string& systemId = "LGT-01",
                   int lampCount = 20,
                   double currentLux = 140.0,
                   int dimmingPercent = 100);

    void setDimming(int percent);
    double calculatePowerConsumptionWatts() const;
    void activateNightMode();

    double currentLux() const noexcept { return currentLux_; }
    int dimmingPercent() const noexcept { return dimmingPercent_; }

private:
    std::string systemId_;
    int lampCount_;
    double currentLux_;
    int dimmingPercent_;
};

/**
 * @class ExhibitionHall
 * @brief Выставочный зал музея.
 */
class ExhibitionHall {
public:
    ExhibitionHall(int hallNumber = 1,
                   const std::string& hallName = "Main Gallery",
                   int floorNumber = 1,
                   double areaSqMeters = 200.0,
                   int maxVisitors = 50,
                   double maxFloorLoadKgPerSqM = 500.0);

    void admitVisitors(int count);
    void evacuateVisitors();
    bool canAccommodateSculpture(const Sculpture& sculpture) const;
    double calculateOccupancyPercent() const;

    int hallNumber() const noexcept { return hallNumber_; }
    const std::string& hallName() const noexcept { return hallName_; }
    int maxVisitors() const noexcept { return maxVisitors_; }
    int currentVisitors() const noexcept { return currentVisitors_; }

private:
    int hallNumber_;
    std::string hallName_;
    int floorNumber_;
    double areaSqMeters_;
    int maxVisitors_;
    int currentVisitors_;
    double maxFloorLoadKgPerSqM_;
};

/**
 * @class StorageVault
 * @brief Защищённое фондохранилище (депозитарий) музея.
 */
class StorageVault {
public:
    StorageVault(const std::string& vaultId = "VAULT-MAIN",
                 int maxCapacity = 100,
                 int securityLevel = 3,
                 const ClimateControlUnit& climateUnit = ClimateControlUnit());

    void depositArtifact(Artifact& artifact);
    Artifact* withdrawArtifact(const std::string& inventoryNumber);
    bool containsArtifact(const std::string& inventoryNumber) const;
    double calculateTotalStoredValue() const;

    const std::string& vaultId() const noexcept { return vaultId_; }
    int securityLevel() const noexcept { return securityLevel_; }
    std::size_t storedCount() const noexcept { return storedArtifacts_.size(); }

private:
    std::string vaultId_;
    int maxCapacity_;
    int securityLevel_;
    ClimateControlUnit climateUnit_;
    std::vector<Artifact*> storedArtifacts_;
};

#endif // MUSEUM_HALLS_AND_CLIMATE_H
