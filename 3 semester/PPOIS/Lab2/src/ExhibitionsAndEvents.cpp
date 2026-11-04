/**
 * @file ExhibitionsAndEvents.cpp
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Реализация классов выставок, маршрутов, экскурсий, аудиогидов и мероприятий.
 */

#include "ExhibitionsAndEvents.h"

#include <algorithm>

namespace {
constexpr double kTemporarySurcharge = 1.25;
constexpr double kVipSurcharge = 1.60;
constexpr double kMetersPerHallStop = 120.0;
constexpr int kMinRentBatteryPercent = 20;
} // namespace

Exhibition::Exhibition(const std::string& exhibitionId,
                       const std::string& title,
                       const std::string& theme,
                       double baseTicketPrice,
                       bool isTemporary)
    : exhibitionId_(exhibitionId),
      title_(title),
      theme_(theme),
      baseTicketPrice_(baseTicketPrice),
      isTemporary_(isTemporary),
      isOpen_(false),
      artifactIds_() {}

void Exhibition::addExhibit(const Artifact& artifact, const ExhibitionHall& hall) {
    if (artifact.needsRestoration()) {
        throw RestorationRequiredException(artifact.title(), artifact.conditionPercent());
    }
    if (hall.maxVisitors() <= 0) {
        throw HallOvercapacityException(hall.hallName(), hall.maxVisitors());
    }
    artifactIds_.push_back(artifact.inventoryNumber());
}

void Exhibition::openExhibition() {
    if (artifactIds_.empty()) {
        throw ExhibitionClosedException(title_ + " (cannot open empty exhibition)");
    }
    isOpen_ = true;
}

void Exhibition::closeExhibition() {
    isOpen_ = false;
}

double Exhibition::calculateExhibitionPrice(bool isVip) const {
    double price = baseTicketPrice_;
    if (isTemporary_) {
        price *= kTemporarySurcharge;
    }
    if (isVip) {
        price *= kVipSurcharge;
    }
    return price;
}

ExcursionRoute::ExcursionRoute(const std::string& routeId, const std::string& routeTitle)
    : routeId_(routeId), routeTitle_(routeTitle), durationMinutes_(0), hallSequence_() {}

void ExcursionRoute::appendHallStop(const ExhibitionHall& hall, int minutesInHall) {
    hallSequence_.push_back(hall.hallNumber());
    durationMinutes_ += std::max(5, minutesInHall);
}

bool ExcursionRoute::includesHall(int hallNumber) const {
    for (int number : hallSequence_) {
        if (number == hallNumber) {
            return true;
        }
    }
    return false;
}

double ExcursionRoute::estimateWalkingDistanceMeters() const {
    return static_cast<double>(hallSequence_.size()) * kMetersPerHallStop;
}

GuidedTour::GuidedTour(const std::string& tourId,
                       const std::string& language,
                       int maxParticipants,
                       const ExcursionRoute& route)
    : tourId_(tourId),
      language_(language),
      maxParticipants_(std::max(1, maxParticipants)),
      currentParticipants_(0),
      isCompleted_(false),
      route_(route) {}

void GuidedTour::enrollParticipant() {
    if (currentParticipants_ >= maxParticipants_) {
        throw TicketSoldOutException("GuidedTour " + tourId_);
    }
    ++currentParticipants_;
}

void GuidedTour::cancelParticipant() {
    if (currentParticipants_ > 0) {
        --currentParticipants_;
    }
}

void GuidedTour::conductTour() {
    if (currentParticipants_ == 0 || route_.stopsCount() == 0) {
        throw ExhibitionClosedException("GuidedTour " + tourId_ + " has no participants or stops");
    }
    isCompleted_ = true;
}

int GuidedTour::availableSpots() const {
    return std::max(0, maxParticipants_ - currentParticipants_);
}

AudioGuideDevice::AudioGuideDevice(const std::string& deviceId,
                                   int batteryPercent,
                                   const std::string& selectedLanguage,
                                   int loadedTracksCount)
    : deviceId_(deviceId),
      batteryPercent_(std::clamp(batteryPercent, 0, 100)),
      selectedLanguage_(selectedLanguage),
      isRented_(false),
      loadedTracksCount_(loadedTracksCount) {}

void AudioGuideDevice::rentToVisitor(const std::string& language) {
    if (needsCharging() || isRented_) {
        throw StaffNotAvailableException("AudioGuide " + deviceId_);
    }
    selectedLanguage_ = language;
    isRented_ = true;
}

void AudioGuideDevice::playTrack(int trackMinutes) {
    int drain = std::max(1, trackMinutes / 2);
    batteryPercent_ = std::max(0, batteryPercent_ - drain);
}

void AudioGuideDevice::returnAndCharge() {
    isRented_ = false;
    batteryPercent_ = 100;
}

bool AudioGuideDevice::needsCharging() const {
    return batteryPercent_ < kMinRentBatteryPercent;
}

SpecialEvent::SpecialEvent(const std::string& eventId,
                           const std::string& eventName,
                           const std::string& eventType,
                           int maxGuests,
                           double entryFee)
    : eventId_(eventId),
      eventName_(eventName),
      eventType_(eventType),
      maxGuests_(std::max(1, maxGuests)),
      registeredGuests_(0),
      entryFee_(entryFee) {}

void SpecialEvent::registerGuest() {
    if (isSoldOut()) {
        throw TicketSoldOutException(eventName_);
    }
    ++registeredGuests_;
}

double SpecialEvent::calculateExpectedRevenue() const {
    return static_cast<double>(registeredGuests_) * entryFee_;
}

bool SpecialEvent::isSoldOut() const {
    return registeredGuests_ >= maxGuests_;
}

ExhibitionSchedule::ExhibitionSchedule(int calendarYear)
    : calendarYear_(calendarYear), activeExhibitions_(), plannedEvents_() {}

void ExhibitionSchedule::scheduleExhibition(const Exhibition& exhibition) {
    activeExhibitions_.push_back(exhibition);
}

void ExhibitionSchedule::scheduleEvent(const SpecialEvent& event) {
    plannedEvents_.push_back(event);
}

int ExhibitionSchedule::countOpenExhibitions() const {
    int openCount = 0;
    for (const Exhibition& ex : activeExhibitions_) {
        if (ex.isOpen()) {
            ++openCount;
        }
    }
    return openCount;
}
