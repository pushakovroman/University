/**
 * @file SecuritySystem.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов системы безопасности, камер, сигнализации и пропусков.
 */

#include "SecuritySystem.h"

#include <algorithm>

namespace {
constexpr double kGbPerRecordingHour = 2.5;
constexpr double kMaxCameraStorageGb = 100.0;
constexpr int kMaxClearanceLevel = 5;
} // namespace

SecurityCamera::SecurityCamera(const std::string& cameraId, int hallNumber, bool hasNightVision)
    : cameraId_(cameraId),
      hallNumber_(hallNumber),
      isRecording_(true),
      hasNightVision_(hasNightVision),
      storageUsedGb_(0.0) {}

void SecurityCamera::startRecording() {
    isRecording_ = true;
}

void SecurityCamera::recordFootage(double hours) {
    if (!isRecording_ || hours <= 0.0) {
        return;
    }
    double addedGb = hours * kGbPerRecordingHour;
    if (storageUsedGb_ + addedGb > kMaxCameraStorageGb) {
        throw HallOvercapacityException("CameraStorage " + cameraId_, static_cast<int>(kMaxCameraStorageGb));
    }
    storageUsedGb_ += addedGb;
}

void SecurityCamera::archiveAndClearStorage() {
    storageUsedGb_ = 0.0;
}

AlarmTrigger::AlarmTrigger(const std::string& triggerId, int zoneHallNumber)
    : triggerId_(triggerId),
      zoneHallNumber_(zoneHallNumber),
      isArmed_(true),
      isTripped_(false) {}

void AlarmTrigger::armTrigger() {
    isArmed_ = true;
}

void AlarmTrigger::tripAlarm() {
    if (!isArmed_) {
        throw SecurityBreachException("Unarmed zone " + std::to_string(zoneHallNumber_));
    }
    isTripped_ = true;
}

void AlarmTrigger::resetAlarm(SecurityGuard& guard) {
    guard.respondToIncident();
    isTripped_ = false;
}

AccessBadge::AccessBadge(const std::string& badgeCode,
                         const std::string& employeeId,
                         int clearanceLevel)
    : badgeCode_(badgeCode),
      employeeId_(employeeId),
      clearanceLevel_(std::clamp(clearanceLevel, 1, kMaxClearanceLevel)),
      isRevoked_(false) {}

bool AccessBadge::canAccessVault(const StorageVault& vault) const {
    return !isRevoked_ && clearanceLevel_ >= vault.securityLevel();
}

void AccessBadge::upgradeClearance(int newLevel) {
    if (isRevoked_) {
        throw SecurityBreachException("Cannot upgrade revoked badge " + badgeCode_);
    }
    clearanceLevel_ = std::clamp(newLevel, clearanceLevel_, kMaxClearanceLevel);
}

void AccessBadge::revokeBadge() {
    isRevoked_ = true;
}

SecurityControlCenter::SecurityControlCenter(const std::string& centerId)
    : centerId_(centerId), isLockdownActive_(false), cameras_(), alarms_() {}

void SecurityControlCenter::registerCamera(const SecurityCamera& camera) {
    cameras_.push_back(camera);
}

void SecurityControlCenter::registerAlarm(const AlarmTrigger& alarm) {
    alarms_.push_back(alarm);
}

void SecurityControlCenter::triggerEmergencyLockdown(ExhibitionHall& hall) {
    isLockdownActive_ = true;
    hall.evacuateVisitors();
}

void SecurityControlCenter::liftLockdown(SecurityGuard& guard) {
    guard.respondToIncident();
    isLockdownActive_ = false;
}
