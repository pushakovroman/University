/**
 * @file ExhibitionsAndEvents.h
 * @author Roman Pushakov (Лабораторная работа №2, ППОИС)
 * @brief Классы выставок, экскурсионных маршрутов, туров, аудиогидов и спецмероприятий.
 * @details Содержит 6 классов: Exhibition, ExcursionRoute, GuidedTour,
 * AudioGuideDevice, SpecialEvent, ExhibitionSchedule.
 */

#ifndef MUSEUM_EXHIBITIONS_AND_EVENTS_H
#define MUSEUM_EXHIBITIONS_AND_EVENTS_H

#include "Artifacts.h"
#include "HallsAndClimate.h"
#include "MuseumExceptions.h"
#include <string>
#include <vector>

/**
 * @class Exhibition
 * @brief Постоянная или временная музейная выставка.
 */
class Exhibition {
public:
    Exhibition(const std::string& exhibitionId = "EXH-01",
               const std::string& title = "Masterpieces of Antiquity",
               const std::string& theme = "History",
               double baseTicketPrice = 20.0,
               bool isTemporary = false);

    void addExhibit(const Artifact& artifact, const ExhibitionHall& hall);
    void openExhibition();
    void closeExhibition();
    double calculateExhibitionPrice(bool isVip) const;

    const std::string& exhibitionId() const noexcept { return exhibitionId_; }
    const std::string& title() const noexcept { return title_; }
    double baseTicketPrice() const noexcept { return baseTicketPrice_; }
    bool isOpen() const noexcept { return isOpen_; }
    std::size_t exhibitsCount() const noexcept { return artifactIds_.size(); }

private:
    std::string exhibitionId_;
    std::string title_;
    std::string theme_;
    double baseTicketPrice_;
    bool isTemporary_;
    bool isOpen_;
    std::vector<std::string> artifactIds_;
};

/**
 * @class ExcursionRoute
 * @brief Маршрут экскурсии по залам музея.
 */
class ExcursionRoute {
public:
    ExcursionRoute(const std::string& routeId = "RT-01",
                   const std::string& routeTitle = "Grand Overview");

    void appendHallStop(const ExhibitionHall& hall, int minutesInHall);
    bool includesHall(int hallNumber) const;
    double estimateWalkingDistanceMeters() const;

    const std::string& routeId() const noexcept { return routeId_; }
    int durationMinutes() const noexcept { return durationMinutes_; }
    std::size_t stopsCount() const noexcept { return hallSequence_.size(); }

private:
    std::string routeId_;
    std::string routeTitle_;
    int durationMinutes_;
    std::vector<int> hallSequence_;
};

/**
 * @class GuidedTour
 * @brief Групповая экскурсия с экскурсоводом.
 */
class GuidedTour {
public:
    GuidedTour(const std::string& tourId,
               const std::string& language,
               int maxParticipants,
               const ExcursionRoute& route);

    void enrollParticipant();
    void cancelParticipant();
    void conductTour();
    int availableSpots() const;

    const std::string& tourId() const noexcept { return tourId_; }
    const std::string& language() const noexcept { return language_; }
    int currentParticipants() const noexcept { return currentParticipants_; }
    bool isCompleted() const noexcept { return isCompleted_; }
    const ExcursionRoute& route() const noexcept { return route_; }

private:
    std::string tourId_;
    std::string language_;
    int maxParticipants_;
    int currentParticipants_;
    bool isCompleted_;
    ExcursionRoute route_;
};

/**
 * @class AudioGuideDevice
 * @brief Индивидуальное устройство аудиогида для посетителей.
 */
class AudioGuideDevice {
public:
    AudioGuideDevice(const std::string& deviceId = "AG-101",
                     int batteryPercent = 100,
                     const std::string& selectedLanguage = "Russian",
                     int loadedTracksCount = 35);

    void rentToVisitor(const std::string& language);
    void playTrack(int trackMinutes);
    void returnAndCharge();
    bool needsCharging() const;

    const std::string& deviceId() const noexcept { return deviceId_; }
    int batteryPercent() const noexcept { return batteryPercent_; }
    bool isRented() const noexcept { return isRented_; }
    const std::string& selectedLanguage() const noexcept { return selectedLanguage_; }

private:
    std::string deviceId_;
    int batteryPercent_;
    std::string selectedLanguage_;
    bool isRented_;
    int loadedTracksCount_;
};

/**
 * @class SpecialEvent
 * @brief Специальное культурное мероприятие (лекция, Ночь музеев, мастер-класс).
 */
class SpecialEvent {
public:
    SpecialEvent(const std::string& eventId,
                 const std::string& eventName,
                 const std::string& eventType,
                 int maxGuests,
                 double entryFee);

    void registerGuest();
    double calculateExpectedRevenue() const;
    bool isSoldOut() const;

    const std::string& eventId() const noexcept { return eventId_; }
    const std::string& eventName() const noexcept { return eventName_; }
    int registeredGuests() const noexcept { return registeredGuests_; }

private:
    std::string eventId_;
    std::string eventName_;
    std::string eventType_;
    int maxGuests_;
    int registeredGuests_;
    double entryFee_;
};

/**
 * @class ExhibitionSchedule
 * @brief Годовое расписание выставок и спецмероприятий музея.
 */
class ExhibitionSchedule {
public:
    explicit ExhibitionSchedule(int calendarYear = 2026);

    void scheduleExhibition(const Exhibition& exhibition);
    void scheduleEvent(const SpecialEvent& event);
    int countOpenExhibitions() const;

    int calendarYear() const noexcept { return calendarYear_; }
    std::size_t totalExhibitions() const noexcept { return activeExhibitions_.size(); }
    std::size_t totalEvents() const noexcept { return plannedEvents_.size(); }

private:
    int calendarYear_;
    std::vector<Exhibition> activeExhibitions_;
    std::vector<SpecialEvent> plannedEvents_;
};

#endif // MUSEUM_EXHIBITIONS_AND_EVENTS_H
