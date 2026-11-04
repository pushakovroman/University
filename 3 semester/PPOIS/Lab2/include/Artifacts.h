/**
 * @file Artifacts.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы музейных экспонатов, авторов, исторических эпох и габаритов.
 * @details Содержит 8 классов: Dimensions, HistoricalEra, ArtistProfile, Artifact,
 * Painting, Sculpture, AncientManuscript, ArcheologicalFind.
 */

#ifndef MUSEUM_ARTIFACTS_H
#define MUSEUM_ARTIFACTS_H

#include "MuseumExceptions.h"
#include <string>

/**
 * @class Dimensions
 * @brief Физические габариты и масса музейного экспоната или витрины.
 */
class Dimensions {
public:
    Dimensions(double widthCm = 10.0, double heightCm = 10.0, double depthCm = 10.0, double weightKg = 1.0);

    double calculateVolumeCubicMeters() const;
    double calculateFootprintAreaSqMeters() const;
    bool fitsWithin(const Dimensions& maxBounds) const;

    double widthCm() const noexcept { return widthCm_; }
    double heightCm() const noexcept { return heightCm_; }
    double depthCm() const noexcept { return depthCm_; }
    double weightKg() const noexcept { return weightKg_; }

private:
    double widthCm_;
    double heightCm_;
    double depthCm_;
    double weightKg_;
};

/**
 * @class HistoricalEra
 * @brief Историческая эпоха происхождения музейного предмета.
 */
class HistoricalEra {
public:
    HistoricalEra(const std::string& name = "Antiquity",
                  int startYear = -500,
                  int endYear = 500,
                  const std::string& region = "Mediterranean");

    bool containsYear(int year) const;
    int durationYears() const;
    bool isAncient() const;

    const std::string& name() const noexcept { return name_; }
    int startYear() const noexcept { return startYear_; }
    int endYear() const noexcept { return endYear_; }
    const std::string& region() const noexcept { return region_; }

private:
    std::string name_;
    int startYear_;
    int endYear_;
    std::string region_;
};

/**
 * @class ArtistProfile
 * @brief Профиль автора (художника, скульптора, мастера).
 */
class ArtistProfile {
public:
    ArtistProfile(const std::string& fullName = "Unknown Master",
                  const std::string& nationality = "Unknown",
                  int birthYear = 1450,
                  int deathYear = 1520,
                  const std::string& artMovement = "Renaissance");

    bool wasActiveInYear(int year) const;
    int lifespanYears() const;
    bool matchesEra(const HistoricalEra& era) const;

    const std::string& fullName() const noexcept { return fullName_; }
    const std::string& nationality() const noexcept { return nationality_; }
    const std::string& artMovement() const noexcept { return artMovement_; }

private:
    std::string fullName_;
    std::string nationality_;
    int birthYear_;
    int deathYear_;
    std::string artMovement_;
};

/**
 * @class Artifact
 * @brief Базовый класс музейного экспоната.
 */
class Artifact {
public:
    Artifact(const std::string& inventoryNumber,
             const std::string& title,
             int creationYear,
             double estimatedValue,
             double conditionPercent,
             const Dimensions& dimensions,
             const HistoricalEra& era);
    virtual ~Artifact() = default;

    bool needsRestoration() const;
    void degradeCondition(double wearPoints);
    void repairCondition(double restoredPoints);
    virtual double calculateInsurancePremium(double annualRate) const;

    void setOnDisplay(bool displayed) noexcept { isOnDisplay_ = displayed; }
    const std::string& inventoryNumber() const noexcept { return inventoryNumber_; }
    const std::string& title() const noexcept { return title_; }
    int creationYear() const noexcept { return creationYear_; }
    double estimatedValue() const noexcept { return estimatedValue_; }
    double conditionPercent() const noexcept { return conditionPercent_; }
    bool isOnDisplay() const noexcept { return isOnDisplay_; }
    const Dimensions& dimensions() const noexcept { return dimensions_; }
    const HistoricalEra& era() const noexcept { return era_; }

private:
    std::string inventoryNumber_;
    std::string title_;
    int creationYear_;
    double estimatedValue_;
    double conditionPercent_;
    bool isOnDisplay_;
    Dimensions dimensions_;
    HistoricalEra era_;
};

/**
 * @class Painting
 * @brief Живописное полотно (картина).
 */
class Painting : public Artifact {
public:
    Painting(const std::string& inventoryNumber,
             const std::string& title,
             int creationYear,
             double estimatedValue,
             double conditionPercent,
             const Dimensions& dimensions,
             const HistoricalEra& era,
             const ArtistProfile& artist,
             const std::string& canvasType = "Linen",
             const std::string& paintMedium = "Oil",
             double maxLightLux = 150.0);

    bool isLightSafe(double currentLux) const;
    void installProtectiveGlass();
    double evaluateArtisticValue() const;

    bool hasProtectiveGlass() const noexcept { return hasProtectiveGlass_; }
    double maxLightLux() const noexcept { return maxLightLux_; }
    const ArtistProfile& artist() const noexcept { return artist_; }

private:
    std::string canvasType_;
    std::string paintMedium_;
    bool hasProtectiveGlass_;
    double maxLightLux_;
    ArtistProfile artist_;
};

/**
 * @class Sculpture
 * @brief Скульптурное произведение.
 */
class Sculpture : public Artifact {
public:
    Sculpture(const std::string& inventoryNumber,
              const std::string& title,
              int creationYear,
              double estimatedValue,
              double conditionPercent,
              const Dimensions& dimensions,
              const HistoricalEra& era,
              const ArtistProfile& artist,
              const std::string& material = "Marble",
              bool isOutdoorSafe = false);

    double calculateFloorLoadKgPerSqM() const;
    bool verifyFloorCapacity(double maxLoadKgPerSqM) const;
    void applyProtectiveWax();

    const std::string& material() const noexcept { return material_; }
    bool requiresReinforcedFloor() const noexcept { return requiresReinforcedFloor_; }
    bool isOutdoorSafe() const noexcept { return isOutdoorSafe_; }

private:
    std::string material_;
    bool requiresReinforcedFloor_;
    bool isOutdoorSafe_;
    ArtistProfile artist_;
};

/**
 * @class AncientManuscript
 * @brief Древняя рукопись или фолиант.
 */
class AncientManuscript : public Artifact {
public:
    AncientManuscript(const std::string& inventoryNumber,
                      const std::string& title,
                      int creationYear,
                      double estimatedValue,
                      double conditionPercent,
                      const Dimensions& dimensions,
                      const HistoricalEra& era,
                      const std::string& language = "Latin",
                      int pageCount = 120,
                      const std::string& inkType = "Iron gall",
                      double maxHumidityPercent = 55.0);

    void digitizeManuscript();
    bool isHumiditySafe(double currentHumidity) const;
    double estimateReadingHours() const;

    bool isDigitized() const noexcept { return isDigitized_; }
    int pageCount() const noexcept { return pageCount_; }
    double maxHumidityPercent() const noexcept { return maxHumidityPercent_; }

private:
    std::string language_;
    int pageCount_;
    std::string inkType_;
    bool isDigitized_;
    double maxHumidityPercent_;
};

/**
 * @class ArcheologicalFind
 * @brief Археологическая находка из раскопок.
 */
class ArcheologicalFind : public Artifact {
public:
    ArcheologicalFind(const std::string& inventoryNumber,
                      const std::string& title,
                      int creationYear,
                      double estimatedValue,
                      double conditionPercent,
                      const Dimensions& dimensions,
                      const HistoricalEra& era,
                      const std::string& excavationSite,
                      int discoveryYear,
                      double soilDepthMeters,
                      int fragilityLevel);

    bool verifyAuthenticity(int carbonDatingYear);
    double calculateTransportRisk() const;
    int yearsSinceDiscovery(int currentYear) const;

    bool isAuthenticVerified() const noexcept { return isAuthenticVerified_; }
    int fragilityLevel() const noexcept { return fragilityLevel_; }
    const std::string& excavationSite() const noexcept { return excavationSite_; }

private:
    std::string excavationSite_;
    int discoveryYear_;
    double soilDepthMeters_;
    int fragilityLevel_;
    bool isAuthenticVerified_;
};

#endif // MUSEUM_ARTIFACTS_H
