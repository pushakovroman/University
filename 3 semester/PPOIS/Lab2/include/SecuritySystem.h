/**
 * @file SecuritySystem.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы системы безопасности, видеонаблюдения, сигнализации и контроля доступа.
 * @details Содержит 4 класса: SecurityCamera, AlarmTrigger,
 * AccessBadge, SecurityControlCenter.
 */

#ifndef MUSEUM_SECURITY_SYSTEM_H
#define MUSEUM_SECURITY_SYSTEM_H

#include "HallsAndClimate.h"
#include "MuseumExceptions.h"
#include "StaffAndHR.h"
#include <string>
#include <vector>

/**
 * @class SecurityCamera
 * @brief Камера видеонаблюдения в зале музея.
 */
class SecurityCamera {
public:
    SecurityCamera(const std::string& cameraId = "CAM-01",
                   int hallNumber = 1,
                   bool hasNightVision = true);

    void startRecording();
    void recordFootage(double hours);
    void archiveAndClearStorage();

    const std::string& cameraId() const noexcept { return cameraId_; }
    bool isRecording() const noexcept { return isRecording_; }
    double storageUsedGb() const noexcept { return storageUsedGb_; }

private:
    std::string cameraId_;
    int hallNumber_;
    bool isRecording_;
    bool hasNightVision_;
    double storageUsedGb_;
};

/**
 * @class AlarmTrigger
 * @brief Извещатель охранно-пожарной сигнализации музейной зоны.
 */
class AlarmTrigger {
public:
    AlarmTrigger(const std::string& triggerId = "ALM-01",
                 int zoneHallNumber = 1);

    void armTrigger();
    void tripAlarm();
    void resetAlarm(SecurityGuard& guard);

    const std::string& triggerId() const noexcept { return triggerId_; }
    bool isArmed() const noexcept { return isArmed_; }
    bool isTripped() const noexcept { return isTripped_; }

private:
    std::string triggerId_;
    int zoneHallNumber_;
    bool isArmed_;
    bool isTripped_;
};

/**
 * @class AccessBadge
 * @brief Электронный пропуск сотрудника с уровнем секретности.
 */
class AccessBadge {
public:
    AccessBadge(const std::string& badgeCode,
                const std::string& employeeId,
                int clearanceLevel = 1);

    bool canAccessVault(const StorageVault& vault) const;
    void upgradeClearance(int newLevel);
    void revokeBadge();

    const std::string& badgeCode() const noexcept { return badgeCode_; }
    int clearanceLevel() const noexcept { return clearanceLevel_; }
    bool isRevoked() const noexcept { return isRevoked_; }

private:
    std::string badgeCode_;
    std::string employeeId_;
    int clearanceLevel_;
    bool isRevoked_;
};

/**
 * @class SecurityControlCenter
 * @brief Центральный диспетчерский пульт охраны музея.
 */
class SecurityControlCenter {
public:
    explicit SecurityControlCenter(const std::string& centerId = "SEC-HQ");

    void registerCamera(const SecurityCamera& camera);
    void registerAlarm(const AlarmTrigger& alarm);
    void triggerEmergencyLockdown(ExhibitionHall& hall);
    void liftLockdown(SecurityGuard& guard);

    bool isLockdownActive() const noexcept { return isLockdownActive_; }
    std::size_t camerasCount() const noexcept { return cameras_.size(); }
    std::size_t alarmsCount() const noexcept { return alarms_.size(); }

private:
    std::string centerId_;
    bool isLockdownActive_;
    std::vector<SecurityCamera> cameras_;
    std::vector<AlarmTrigger> alarms_;
};

#endif // MUSEUM_SECURITY_SYSTEM_H
