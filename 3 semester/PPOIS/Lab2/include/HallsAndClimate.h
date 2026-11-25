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
 * @details Измеряет температуру, относительную влажность и освещённость (люксы) в выставочном зале или хранилище.
 * @see ClimateControlUnit
 */
class ClimateSensor {
public:
    /**
     * @brief Конструктор класса ClimateSensor.
     * @param sensorId Уникальный идентификатор климатического датчика.
     * @param temperatureCelsius Температура воздуха в градусах Цельсия.
     * @param humidityPercent Относительная влажность воздуха в процентах.
     * @param lightLux Уровень освещённости в люксах.
     */
    ClimateSensor(const std::string& sensorId = "SNS-01",
                  double temperatureCelsius = 20.0,
                  double humidityPercent = 45.0,
                  double lightLux = 120.0);

    /**
     * @brief Обновляет показания температуры, влажности и освещённости.
     * @details Переводит датчик в статус online.
     * @param tempC Новое измеренное значение температуры (°C).
     * @param humidity Новое измеренное значение влажности (%).
     * @param lux Новое измеренное значение освещённости (лк).
     */
    void updateReadings(double tempC, double humidity, double lux);
    /**
     * @brief Проверяет, активен ли датчик и находятся ли температура, влажность и освещённость в заданных границах.
     * @param minTemp Нижняя граница допустимой температуры (°C).
     * @param maxTemp Верхняя граница допустимой температуры (°C).
     * @param maxHumidity Верхняя граница допустимой влажности (%).
     * @param maxLux Верхняя граница допустимой освещённости (лк).
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isWithinNorms(double minTemp, double maxTemp, double maxHumidity, double maxLux) const;
    /**
     * @brief Сбрасывает показания к эталонным музейным нормам (20.0 °C, 45.0%, 120.0 лк).
     */
    void calibrateSensor();

    /// @brief Возвращает идентификатор датчика.
    const std::string& sensorId() const noexcept { return sensorId_; }
    /// @brief Возвращает текущую температуру (°C).
    double temperatureCelsius() const noexcept { return temperatureCelsius_; }
    /// @brief Возвращает текущую влажность (%).
    double humidityPercent() const noexcept { return humidityPercent_; }
    /// @brief Возвращает текущую освещённость (лк).
    double lightLux() const noexcept { return lightLux_; }
    /// @brief Возвращает статус активности датчика.
    bool isOnline() const noexcept { return isOnline_; }

private:
    std::string sensorId_; ///< Уникальный идентификатор климатического датчика.
    double temperatureCelsius_; ///< Температура воздуха в градусах Цельсия.
    double humidityPercent_; ///< Относительная влажность воздуха в процентах.
    double lightLux_; ///< Уровень освещённости в люксах.
    bool isOnline_; ///< Флаг исправного онлайн-состояния датчика.
};

/**
 * @class ClimateControlUnit
 * @brief Климатическая установка для поддержания музейного микроклимата.
 * @details Поддерживает целевые параметры микроклимата и проверяет безопасность показаний датчика для рукописей и полотен.
 * @see ClimateSensor, AncientManuscript, Painting, StorageVault
 */
class ClimateControlUnit {
public:
    /**
     * @brief Конструктор класса ClimateControlUnit.
     * @param unitId Идентификатор климатической установки.
     * @param targetTempCelsius Целевая температура поддержания (°C).
     * @param targetHumidityPercent Целевая влажность поддержания (%).
     * @param maxLightLux Максимально допустимая освещённость в люксах.
     */
    ClimateControlUnit(const std::string& unitId = "CCU-01",
                       double targetTempCelsius = 20.0,
                       double targetHumidityPercent = 45.0,
                       double maxLightLux = 150.0);

    /**
     * @brief Активирует установку и приводит показания датчика к целевым значениям (освещённость = 0.8 * maxLightLux_).
     * @param sensor Датчик микроклимата, с которого считываются или корректируются показания.
     */
    void adjustToSensor(ClimateSensor& sensor);
    /**
     * @brief Проверяет допустимость влажности с датчика для древней рукописи.
     * @param manuscript Проверяемая древняя рукопись.
     * @param sensor Датчик микроклимата, с которого считываются или корректируются показания.
     * @throw ClimateOutOfRangeException При нарушении бизнес-правил или инвариантов операции.
     */
    void verifyArtifactSafety(const AncientManuscript& manuscript, const ClimateSensor& sensor) const;
    /**
     * @brief Проверяет безопасность уровня освещённости с датчика для живописного полотна.
     * @param painting Проверяемое живописное полотно.
     * @param sensor Датчик микроклимата, с которого считываются или корректируются показания.
     * @throw ClimateOutOfRangeException При нарушении бизнес-правил или инвариантов операции.
     */
    void verifyPaintingLight(const Painting& painting, const ClimateSensor& sensor) const;

    /// @brief Возвращает признак активности объекта.
    bool isActive() const noexcept { return isActive_; }
    /// @brief Возвращает целевую температуру (°C).
    double targetTempCelsius() const noexcept { return targetTempCelsius_; }
    /// @brief Возвращает целевую влажность (%).
    double targetHumidityPercent() const noexcept { return targetHumidityPercent_; }

private:
    std::string unitId_; ///< Идентификатор климатической установки.
    double targetTempCelsius_; ///< Целевая температура поддержания (°C).
    double targetHumidityPercent_; ///< Целевая влажность поддержания (%).
    double maxLightLux_; ///< Максимально допустимая освещённость в люксах.
    bool isActive_; ///< Признак активности установки или клубной карты.
};

/**
 * @class DisplayCase
 * @brief Защищённая музейная витрина для экспонирования предметов.
 * @details Агрегирует невладеющий указатель на размещённый экспонат, проверяя сохранность и габариты при монтаже.
 * @see Dimensions, Artifact, SecurityGuard
 */
class DisplayCase {
public:
    /**
     * @brief Конструктор класса DisplayCase.
     * @param caseId Уникальный код музейной витрины.
     * @param maxDimensions Максимальные внутренние габариты и грузоподъёмность витрины.
     * @param hasAlarm Наличие встроенной охранной сигнализации.
     * @param isHermetic Признак герметичности витрины.
     */
    DisplayCase(const std::string& caseId,
                const Dimensions& maxDimensions,
                bool hasAlarm = true,
                bool isHermetic = true);

    /**
     * @brief Размещает экспонат в витрине, проверяя сохранность (>= 40%) и соответствие габаритам.
     * @details Выставляет флаг isOnDisplay.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @throw RestorationRequiredException При нарушении бизнес-правил или инвариантов операции.
     * @throw InvalidArtifactDataException При нарушении бизнес-правил или инвариантов операции.
     */
    void mountArtifact(Artifact& artifact);
    /**
     * @brief Извлекает экспонат из витрины, снимает флаг isOnDisplay и разблокирует замок.
     */
    void unmountArtifact();
    /**
     * @brief Запирает защитный замок витрины (isLocked_ = true).
     */
    void lockCase();
    /**
     * @brief Возвращает true, если витрина одновременно герметична и оснащена сигнализацией.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isSuitableForFragile() const;

    /// @brief Возвращает код витрины.
    const std::string& caseId() const noexcept { return caseId_; }
    /// @brief Возвращает статус блокировки замка витрины.
    bool isLocked() const noexcept { return isLocked_; }
    /// @brief Возвращает невладеющий указатель на размещённый экспонат (или nullptr).
    const Artifact* placedArtifact() const noexcept { return placedArtifact_; }

private:
    std::string caseId_; ///< Уникальный код музейной витрины.
    Dimensions maxDimensions_; ///< Максимальные внутренние габариты и грузоподъёмность витрины.
    bool hasAlarm_; ///< Наличие встроенной охранной сигнализации.
    bool isHermetic_; ///< Признак герметичности витрины.
    bool isLocked_; ///< Флаг запертого состояния замка витрины.
    Artifact* placedArtifact_; ///< Невладеющий указатель на выставленный экспонат (nullptr, если витрина пуста).
};

/**
 * @class LightingSystem
 * @brief Интеллектуальная система освещения выставочного зала.
 * @details Управляет диммированием ламп зала (0–100%), текущей освещённостью в люксах и энергопотреблением.
 * @see ExhibitionHall
 */
class LightingSystem {
public:
    /**
     * @brief Конструктор класса LightingSystem.
     * @param systemId Идентификатор системы освещения.
     * @param lampCount Количество светильников в системе (>= 1).
     * @param currentLux Текущий уровень освещённости в люксах.
     * @param dimmingPercent Уровень диммирования в процентах [0..100].
     */
    LightingSystem(const std::string& systemId = "LGT-01",
                   int lampCount = 20,
                   double currentLux = 140.0,
                   int dimmingPercent = 100);

    /**
     * @brief Задаёт уровень яркости [0..100%] и пересчитывает освещённость (200 лк при 100%).
     * @param percent Устанавливаемый уровень яркости в процентах [0..100].
     */
    void setDimming(int percent);
    /**
     * @brief Вычисляет текущую мощность системы в Вт: lampCount_ * 18.0 * (dimmingPercent_ / 100).
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculatePowerConsumptionWatts() const;
    /**
     * @brief Переводит освещение в дежурный ночной режим (диммирование 15%, 30 лк).
     */
    void activateNightMode();

    /// @brief Возвращает текущую освещённость зала (лк).
    double currentLux() const noexcept { return currentLux_; }
    /// @brief Возвращает текущий процент диммирования.
    int dimmingPercent() const noexcept { return dimmingPercent_; }

private:
    std::string systemId_; ///< Идентификатор системы освещения.
    int lampCount_; ///< Количество светильников в системе (>= 1).
    double currentLux_; ///< Текущий уровень освещённости в люксах.
    int dimmingPercent_; ///< Уровень диммирования в процентах [0..100].
};

/**
 * @class ExhibitionHall
 * @brief Выставочный зал музея.
 * @details Контролирует текущее число посетителей, квоту вместимости и допустимую нагрузку перекрытия для скульптур.
 * @see Sculpture, Exhibition, Museum
 */
class ExhibitionHall {
public:
    /**
     * @brief Конструктор класса ExhibitionHall.
     * @param hallNumber Порядковый номер выставочного зала.
     * @param hallName Наименование выставочного зала.
     * @param floorNumber Номер этажа расположения зала.
     * @param areaSqMeters Площадь выставочного зала в м².
     * @param maxVisitors Максимально допустимое число посетителей в зале.
     * @param maxFloorLoadKgPerSqM Предельная нагрузка перекрытия зала (кг/м²).
     */
    ExhibitionHall(int hallNumber = 1,
                   const std::string& hallName = "Main Gallery",
                   int floorNumber = 1,
                   double areaSqMeters = 200.0,
                   int maxVisitors = 50,
                   double maxFloorLoadKgPerSqM = 500.0);

    /**
     * @brief Впускает count посетителей с контролем непревышения квоты maxVisitors_.
     * @param count Количество посетителей, вещей или единиц товара.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void admitVisitors(int count);
    /**
     * @brief Экстренно эвакуирует всех посетителей зала (currentVisitors_ = 0).
     */
    void evacuateVisitors();
    /**
     * @brief Проверяет, выдержит ли перекрытие зала удельную нагрузку скульптуры.
     * @param sculpture Проверяемая скульптура.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool canAccommodateSculpture(const Sculpture& sculpture) const;
    /**
     * @brief Возвращает процент заполненности зала (0%, если maxVisitors_ <= 0).
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateOccupancyPercent() const;

    /// @brief Возвращает номер выставочного зала.
    int hallNumber() const noexcept { return hallNumber_; }
    /// @brief Возвращает наименование зала.
    const std::string& hallName() const noexcept { return hallName_; }
    /// @brief Возвращает предельную вместимость зала.
    int maxVisitors() const noexcept { return maxVisitors_; }
    /// @brief Возвращает текущее число посетителей в зале.
    int currentVisitors() const noexcept { return currentVisitors_; }

private:
    int hallNumber_; ///< Порядковый номер выставочного зала.
    std::string hallName_; ///< Наименование выставочного зала.
    int floorNumber_; ///< Номер этажа расположения зала.
    double areaSqMeters_; ///< Площадь выставочного зала в м².
    int maxVisitors_; ///< Максимально допустимое число посетителей в зале.
    int currentVisitors_; ///< Текущее число посетителей в зале.
    double maxFloorLoadKgPerSqM_; ///< Предельная нагрузка перекрытия зала (кг/м²).
};

/**
 * @class StorageVault
 * @brief Защищённое фондохранилище (депозитарий) музея.
 * @details Фондохранилище (депозитарий), агрегирующее невладеющие указатели на неэкспонируемые предметы с контролем вместимости и уровня охраны.
 * @see Artifact, ClimateControlUnit, AccessBadge
 */
class StorageVault {
public:
    /**
     * @brief Конструктор класса StorageVault.
     * @param vaultId Идентификатор фондохранилища.
     * @param maxCapacity Максимальная вместимость хранилища или зала.
     * @param securityLevel Требуемый уровень секретности доступа (1–5).
     * @param climateUnit Климатическая установка фондохранилища.
     */
    StorageVault(const std::string& vaultId = "VAULT-MAIN",
                 int maxCapacity = 100,
                 int securityLevel = 3,
                 const ClimateControlUnit& climateUnit = ClimateControlUnit());

    /**
     * @brief Принимает экспонат в фондохранилище (снимает с экспозиции) при наличии свободных мест.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void depositArtifact(Artifact& artifact);
    /**
     * @brief Извлекает и возвращает указатель на экспонат по инвентарному номеру.
     * @param inventoryNumber Уникальный инвентарный номер экспоната.
     * @return Значение типа @c Artifact*, вычисленное методом.
     * @throw ArtifactNotFoundException При нарушении бизнес-правил или инвариантов операции.
     */
    Artifact* withdrawArtifact(const std::string& inventoryNumber);
    /**
     * @brief Проверяет наличие экспоната с заданным инвентарным номером в депозитарии.
     * @param inventoryNumber Уникальный инвентарный номер экспоната.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool containsArtifact(const std::string& inventoryNumber) const;
    /**
     * @brief Суммирует оценочную стоимость всех экспонатов, находящихся в хранилище.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateTotalStoredValue() const;

    /// @brief Возвращает идентификатор хранилища.
    const std::string& vaultId() const noexcept { return vaultId_; }
    /// @brief Возвращает требуемый уровень секретности хранилища.
    int securityLevel() const noexcept { return securityLevel_; }
    /// @brief Возвращает количество хранимых предметов.
    std::size_t storedCount() const noexcept { return storedArtifacts_.size(); }

private:
    std::string vaultId_; ///< Идентификатор фондохранилища.
    int maxCapacity_; ///< Максимальная вместимость хранилища или зала.
    int securityLevel_; ///< Требуемый уровень секретности доступа (1–5).
    ClimateControlUnit climateUnit_; ///< Климатическая установка фондохранилища.
    std::vector<Artifact*> storedArtifacts_; ///< Список невладеющих указателей на экспонаты в депозитарии.
};

#endif // MUSEUM_HALLS_AND_CLIMATE_H
