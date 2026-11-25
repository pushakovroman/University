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
 * @details Камера видеонаблюдения с ночным режимом и учётом объёма видеоархива (2.5 ГБ/ч, лимит 100 ГБ).
 * @see SecurityControlCenter
 */
class SecurityCamera {
public:
    /**
     * @brief Конструктор класса SecurityCamera.
     * @param cameraId Идентификатор камеры видеонаблюдения.
     * @param hallNumber Порядковый номер выставочного зала.
     * @param hasNightVision Наличие инфракрасного ночного видения.
     */
    SecurityCamera(const std::string& cameraId = "CAM-01",
                   int hallNumber = 1,
                   bool hasNightVision = true);

    /**
     * @brief Включает режим видеозаписи камеры (isRecording_ = true).
     */
    void startRecording();
    /**
     * @brief Записывает hours часов видео (2.5 ГБ/ч) с контролем лимита диска 100 ГБ.
     * @param hours Длительность записываемого видеофрагмента в часах.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void recordFootage(double hours);
    /**
     * @brief Выгружает архив и обнуляет занятый объём локального накопителя камеры.
     */
    void archiveAndClearStorage();

    /// @brief Возвращает идентификатор камеры.
    const std::string& cameraId() const noexcept { return cameraId_; }
    /// @brief Возвращает true, если ведётся видеозапись.
    bool isRecording() const noexcept { return isRecording_; }
    /// @brief Возвращает занятый объём архива в гигабайтах.
    double storageUsedGb() const noexcept { return storageUsedGb_; }

private:
    std::string cameraId_; ///< Идентификатор камеры видеонаблюдения.
    int hallNumber_; ///< Порядковый номер выставочного зала.
    bool isRecording_; ///< Признак активной видеозаписи камеры.
    bool hasNightVision_; ///< Наличие инфракрасного ночного видения.
    double storageUsedGb_; ///< Занятый объём видеоархива камеры (ГБ).
};

/**
 * @class AlarmTrigger
 * @brief Извещатель охранно-пожарной сигнализации музейной зоны.
 * @details Адресный датчик охранно-пожарной сигнализации музейной зоны с постановкой на охрану и сбросом инцидента.
 * @see SecurityGuard, SecurityControlCenter
 */
class AlarmTrigger {
public:
    /**
     * @brief Конструктор класса AlarmTrigger.
     * @param triggerId Идентификатор охранного извещателя.
     * @param zoneHallNumber Номер контролируемого зала.
     */
    AlarmTrigger(const std::string& triggerId = "ALM-01",
                 int zoneHallNumber = 1);

    /**
     * @brief Ставит охранный извещатель на охрану (isArmed_ = true).
     */
    void armTrigger();
    /**
     * @brief Фиксирует срабатывание датчика.
     * @details Если зона снята с охраны, генерирует нарушение безопасности.
     * @throw SecurityBreachException При нарушении бизнес-правил или инвариантов операции.
     */
    void tripAlarm();
    /**
     * @brief Сбрасывает тревогу после подтверждённого реагирования дежурного охранника.
     * @param guard Дежурный охранник, отрабатывающий инцидент.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void resetAlarm(SecurityGuard& guard);

    /// @brief Возвращает идентификатор извещателя.
    const std::string& triggerId() const noexcept { return triggerId_; }
    /// @brief Возвращает true, если датчик стоит на охране.
    bool isArmed() const noexcept { return isArmed_; }
    /// @brief Возвращает true, если зафиксирована тревога.
    bool isTripped() const noexcept { return isTripped_; }

private:
    std::string triggerId_; ///< Идентификатор охранного извещателя.
    int zoneHallNumber_; ///< Номер контролируемого зала.
    bool isArmed_; ///< Признак постановки датчика на охрану.
    bool isTripped_; ///< Признак активной тревоги датчика.
};

/**
 * @class AccessBadge
 * @brief Электронный пропуск сотрудника с уровнем секретности.
 * @details Электронный пропуск сотрудника с уровнем секретности 1–5 и возможностью отзыва.
 * @see StorageVault
 */
class AccessBadge {
public:
    /**
     * @brief Конструктор класса AccessBadge.
     * @param badgeCode Уникальный код электронного пропуска.
     * @param employeeId Табельный идентификатор сотрудника.
     * @param clearanceLevel Уровень допуска сотрудника [1..5].
     */
    AccessBadge(const std::string& badgeCode,
                const std::string& employeeId,
                int clearanceLevel = 1);

    /**
     * @brief Проверяет допуск в хранилище: пропуск не отозван и clearanceLevel_ >= vault.securityLevel().
     * @param vault Проверяемое фондохранилище.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool canAccessVault(const StorageVault& vault) const;
    /**
     * @brief Повышает уровень допуска (до 5-го); запрещено для отозванных пропусков.
     * @param newLevel Новый целевой уровень допуска [clearanceLevel..5].
     * @throw SecurityBreachException При нарушении бизнес-правил или инвариантов операции.
     */
    void upgradeClearance(int newLevel);
    /**
     * @brief Аннулирует электронный пропуск (isRevoked_ = true).
     */
    void revokeBadge();

    /// @brief Возвращает код электронного пропуска.
    const std::string& badgeCode() const noexcept { return badgeCode_; }
    /// @brief Возвращает уровень допуска пропуска (1–5).
    int clearanceLevel() const noexcept { return clearanceLevel_; }
    /// @brief Возвращает true, если пропуск аннулирован.
    bool isRevoked() const noexcept { return isRevoked_; }

private:
    std::string badgeCode_; ///< Уникальный код электронного пропуска.
    std::string employeeId_; ///< Табельный идентификатор сотрудника.
    int clearanceLevel_; ///< Уровень допуска сотрудника [1..5].
    bool isRevoked_; ///< Признак аннулирования электронного пропуска.
};

/**
 * @class SecurityControlCenter
 * @brief Центральный диспетчерский пульт охраны музея.
 * @details Центральный пульт охраны: регистрирует камеры и датчики, управляет экстренной блокировкой (Lockdown) и эвакуацией.
 * @see SecurityCamera, AlarmTrigger, ExhibitionHall, SecurityGuard, Museum
 */
class SecurityControlCenter {
public:
    /**
     * @brief Конструктор класса SecurityControlCenter.
     * @param centerId Идентификатор диспетчерского пульта охраны.
     */
    explicit SecurityControlCenter(const std::string& centerId = "SEC-HQ");

    /**
     * @brief Регистрирует камеру видеонаблюдения на диспетчерском пульте.
     * @param camera Регистрируемая камера видеонаблюдения.
     */
    void registerCamera(const SecurityCamera& camera);
    /**
     * @brief Подключает охранно-пожарный извещатель к пульту охраны.
     * @param alarm Регистрируемый охранно-пожарный датчик.
     */
    void registerAlarm(const AlarmTrigger& alarm);
    /**
     * @brief Активирует экстренную блокировку комплекса и немедленно эвакуирует посетителей зала.
     * @param hall Выставочный зал, задействованный в операции.
     */
    void triggerEmergencyLockdown(ExhibitionHall& hall);
    /**
     * @brief Снимает режим блокировки после отработки инцидента дежурным охранником.
     * @param guard Дежурный охранник, отрабатывающий инцидент.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void liftLockdown(SecurityGuard& guard);

    /// @brief Возвращает статус режима экстренной блокировки.
    bool isLockdownActive() const noexcept { return isLockdownActive_; }
    /// @brief Возвращает число зарегистрированных камер.
    std::size_t camerasCount() const noexcept { return cameras_.size(); }
    /// @brief Возвращает число зарегистрированных датчиков.
    std::size_t alarmsCount() const noexcept { return alarms_.size(); }

private:
    std::string centerId_; ///< Идентификатор диспетчерского пульта охраны.
    bool isLockdownActive_; ///< Признак активного режима экстренной блокировки.
    std::vector<SecurityCamera> cameras_; ///< Зарегистрированные камеры видеонаблюдения.
    std::vector<AlarmTrigger> alarms_; ///< Зарегистрированные охранно-пожарные извещатели.
};

#endif // MUSEUM_SECURITY_SYSTEM_H
