/**
 * @file Artifacts.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов музейных экспонатов, эпох и авторов.
 */

#include "Artifacts.h"

#include <algorithm>
#include <cmath>

namespace {
constexpr double kCmToMeters = 0.01;
constexpr double kCubicCmToCubicMeters = 1e-6;
constexpr int kAncientBoundaryYear = 500;
constexpr int kMinAdultArtistAge = 14;
constexpr double kCriticalConditionThreshold = 40.0;
constexpr double kMaxConditionPercent = 100.0;
constexpr double kAncientInsuranceMultiplier = 1.35;
constexpr double kHeavySculptureThresholdKg = 300.0;
constexpr double kWaxConditionBonus = 5.0;
constexpr double kDigitizationBonus = 3.0;
constexpr double kPagesPerHour = 15.0;
constexpr int kCarbonDatingToleranceYears = 150;
} // namespace

Dimensions::Dimensions(double widthCm, double heightCm, double depthCm, double weightKg)
    : widthCm_(widthCm), heightCm_(heightCm), depthCm_(depthCm), weightKg_(weightKg) {
    if (widthCm <= 0.0 || heightCm <= 0.0 || depthCm <= 0.0 || weightKg <= 0.0) {
        throw InvalidArtifactDataException("Dimensions and weight must be positive");
    }
}

double Dimensions::calculateVolumeCubicMeters() const {
    return widthCm_ * heightCm_ * depthCm_ * kCubicCmToCubicMeters;
}

double Dimensions::calculateFootprintAreaSqMeters() const {
    return (widthCm_ * kCmToMeters) * (depthCm_ * kCmToMeters);
}

bool Dimensions::fitsWithin(const Dimensions& maxBounds) const {
    return widthCm_ <= maxBounds.widthCm_ &&
           heightCm_ <= maxBounds.heightCm_ &&
           depthCm_ <= maxBounds.depthCm_ &&
           weightKg_ <= maxBounds.weightKg_;
}

HistoricalEra::HistoricalEra(const std::string& name,
                             int startYear,
                             int endYear,
                             const std::string& region)
    : name_(name), startYear_(startYear), endYear_(endYear), region_(region) {
    if (name.empty() || endYear < startYear) {
        throw InvalidArtifactDataException("Invalid historical era boundaries");
    }
}

bool HistoricalEra::containsYear(int year) const {
    return year >= startYear_ && year <= endYear_;
}

int HistoricalEra::durationYears() const {
    return endYear_ - startYear_;
}

bool HistoricalEra::isAncient() const {
    return endYear_ <= kAncientBoundaryYear;
}

ArtistProfile::ArtistProfile(const std::string& fullName,
                             const std::string& nationality,
                             int birthYear,
                             int deathYear,
                             const std::string& artMovement)
    : fullName_(fullName),
      nationality_(nationality),
      birthYear_(birthYear),
      deathYear_(deathYear),
      artMovement_(artMovement) {
    if (fullName.empty() || deathYear < birthYear) {
        throw InvalidArtifactDataException("Invalid artist profile data");
    }
}

bool ArtistProfile::wasActiveInYear(int year) const {
    return year >= (birthYear_ + kMinAdultArtistAge) && year <= deathYear_;
}

int ArtistProfile::lifespanYears() const {
    return deathYear_ - birthYear_;
}

bool ArtistProfile::matchesEra(const HistoricalEra& era) const {
    return birthYear_ <= era.endYear() && deathYear_ >= era.startYear();
}

Artifact::Artifact(const std::string& inventoryNumber,
                   const std::string& title,
                   int creationYear,
                   double estimatedValue,
                   double conditionPercent,
                   const Dimensions& dimensions,
                   const HistoricalEra& era)
    : inventoryNumber_(inventoryNumber),
      title_(title),
      creationYear_(creationYear),
      estimatedValue_(estimatedValue),
      conditionPercent_(conditionPercent),
      isOnDisplay_(false),
      dimensions_(dimensions),
      era_(era) {
    if (inventoryNumber.empty() || title.empty() || estimatedValue < 0.0 ||
        conditionPercent < 0.0 || conditionPercent > kMaxConditionPercent) {
        throw InvalidArtifactDataException("Invalid artifact core fields");
    }
}

bool Artifact::needsRestoration() const {
    return conditionPercent_ < kCriticalConditionThreshold;
}

void Artifact::degradeCondition(double wearPoints) {
    if (wearPoints < 0.0) {
        throw InvalidArtifactDataException("Wear points cannot be negative");
    }
    conditionPercent_ = std::max(0.0, conditionPercent_ - wearPoints);
}

void Artifact::repairCondition(double restoredPoints) {
    if (restoredPoints < 0.0) {
        throw InvalidArtifactDataException("Restored points cannot be negative");
    }
    conditionPercent_ = std::min(kMaxConditionPercent, conditionPercent_ + restoredPoints);
}

double Artifact::calculateInsurancePremium(double annualRate) const {
    if (annualRate < 0.0) {
        throw InvalidArtifactDataException("Insurance rate cannot be negative");
    }
    double multiplier = era_.isAncient() ? kAncientInsuranceMultiplier : 1.0;
    return estimatedValue_ * annualRate * multiplier;
}

Painting::Painting(const std::string& inventoryNumber,
                   const std::string& title,
                   int creationYear,
                   double estimatedValue,
                   double conditionPercent,
                   const Dimensions& dimensions,
                   const HistoricalEra& era,
                   const ArtistProfile& artist,
                   const std::string& canvasType,
                   const std::string& paintMedium,
                   double maxLightLux)
    : Artifact(inventoryNumber, title, creationYear, estimatedValue, conditionPercent, dimensions, era),
      canvasType_(canvasType),
      paintMedium_(paintMedium),
      hasProtectiveGlass_(false),
      maxLightLux_(maxLightLux),
      artist_(artist) {}

bool Painting::isLightSafe(double currentLux) const {
    double effectiveLimit = hasProtectiveGlass_ ? maxLightLux_ * 1.25 : maxLightLux_;
    return currentLux <= effectiveLimit;
}

void Painting::installProtectiveGlass() {
    hasProtectiveGlass_ = true;
}

double Painting::evaluateArtisticValue() const {
    double mediumBonus = (paintMedium_ == "Oil") ? 1.15 : 1.05;
    double conditionFactor = conditionPercent() / kMaxConditionPercent;
    return estimatedValue() * mediumBonus * (0.5 + 0.5 * conditionFactor);
}

Sculpture::Sculpture(const std::string& inventoryNumber,
                     const std::string& title,
                     int creationYear,
                     double estimatedValue,
                     double conditionPercent,
                     const Dimensions& dimensions,
                     const HistoricalEra& era,
                     const ArtistProfile& artist,
                     const std::string& material,
                     bool isOutdoorSafe)
    : Artifact(inventoryNumber, title, creationYear, estimatedValue, conditionPercent, dimensions, era),
      material_(material),
      requiresReinforcedFloor_(dimensions.weightKg() >= kHeavySculptureThresholdKg),
      isOutdoorSafe_(isOutdoorSafe),
      artist_(artist) {}

double Sculpture::calculateFloorLoadKgPerSqM() const {
    double area = dimensions().calculateFootprintAreaSqMeters();
    return dimensions().weightKg() / std::max(0.01, area);
}

bool Sculpture::verifyFloorCapacity(double maxLoadKgPerSqM) const {
    return calculateFloorLoadKgPerSqM() <= maxLoadKgPerSqM;
}

void Sculpture::applyProtectiveWax() {
    isOutdoorSafe_ = true;
    repairCondition(kWaxConditionBonus);
}

AncientManuscript::AncientManuscript(const std::string& inventoryNumber,
                                     const std::string& title,
                                     int creationYear,
                                     double estimatedValue,
                                     double conditionPercent,
                                     const Dimensions& dimensions,
                                     const HistoricalEra& era,
                                     const std::string& language,
                                     int pageCount,
                                     const std::string& inkType,
                                     double maxHumidityPercent)
    : Artifact(inventoryNumber, title, creationYear, estimatedValue, conditionPercent, dimensions, era),
      language_(language),
      pageCount_(pageCount),
      inkType_(inkType),
      isDigitized_(false),
      maxHumidityPercent_(maxHumidityPercent) {
    if (pageCount <= 0) {
        throw InvalidArtifactDataException("Manuscript page count must be positive");
    }
}

void AncientManuscript::digitizeManuscript() {
    isDigitized_ = true;
    repairCondition(kDigitizationBonus);
}

bool AncientManuscript::isHumiditySafe(double currentHumidity) const {
    return currentHumidity >= 30.0 && currentHumidity <= maxHumidityPercent_;
}

double AncientManuscript::estimateReadingHours() const {
    return static_cast<double>(pageCount_) / kPagesPerHour;
}

ArcheologicalFind::ArcheologicalFind(const std::string& inventoryNumber,
                                     const std::string& title,
                                     int creationYear,
                                     double estimatedValue,
                                     double conditionPercent,
                                     const Dimensions& dimensions,
                                     const HistoricalEra& era,
                                     const std::string& excavationSite,
                                     int discoveryYear,
                                     double soilDepthMeters,
                                     int fragilityLevel)
    : Artifact(inventoryNumber, title, creationYear, estimatedValue, conditionPercent, dimensions, era),
      excavationSite_(excavationSite),
      discoveryYear_(discoveryYear),
      soilDepthMeters_(soilDepthMeters),
      fragilityLevel_(std::clamp(fragilityLevel, 1, 10)),
      isAuthenticVerified_(false) {}

bool ArcheologicalFind::verifyAuthenticity(int carbonDatingYear) {
    isAuthenticVerified_ = (std::abs(carbonDatingYear - creationYear()) <= kCarbonDatingToleranceYears);
    return isAuthenticVerified_;
}

double ArcheologicalFind::calculateTransportRisk() const {
    double fragilityFactor = static_cast<double>(fragilityLevel_) / 10.0;
    double conditionPenalty = (kMaxConditionPercent - conditionPercent()) / kMaxConditionPercent;
    return std::min(1.0, 0.6 * fragilityFactor + 0.4 * conditionPenalty);
}

int ArcheologicalFind::yearsSinceDiscovery(int currentYear) const {
    return std::max(0, currentYear - discoveryYear_);
}
