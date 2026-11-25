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
 * @details Тематическая постоянная или временная экспозиция с контролем статуса открытия и расчётом стандартной/VIP-цены.
 * @see Artifact, ExhibitionHall, ExhibitionSchedule
 */
class Exhibition {
public:
    /**
     * @brief Конструктор класса Exhibition.
     * @param exhibitionId Уникальный идентификатор выставки.
     * @param title Наименование экспоната или выставки.
     * @param theme Тематическая категория экспозиции.
     * @param baseTicketPrice Базовая стоимость стандартного входного билета.
     * @param isTemporary Признак временной (гастрольной) выставки.
     */
    Exhibition(const std::string& exhibitionId = "EXH-01",
               const std::string& title = "Masterpieces of Antiquity",
               const std::string& theme = "History",
               double baseTicketPrice = 20.0,
               bool isTemporary = false);

    /**
     * @brief Добавляет экспонат в состав выставки после проверки сохранности и вместимости зала.
     * @param artifact Музейный экспонат, участвующий в операции.
     * @param hall Выставочный зал, задействованный в операции.
     * @throw RestorationRequiredException При нарушении бизнес-правил или инвариантов операции.
     * @throw HallOvercapacityException При нарушении бизнес-правил или инвариантов операции.
     */
    void addExhibit(const Artifact& artifact, const ExhibitionHall& hall);
    /**
     * @brief Открывает выставку для посещения; запрещено открывать пустую экспозицию.
     * @throw ExhibitionClosedException При нарушении бизнес-правил или инвариантов операции.
     */
    void openExhibition();
    /**
     * @brief Закрывает выставку для посещения (isOpen_ = false).
     */
    void closeExhibition();
    /**
     * @brief Рассчитывает цену билета: baseTicketPrice * (1.25 если временная) * (1.60 если VIP).
     * @param isVip Признак расчёта стоимости VIP-билета.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateExhibitionPrice(bool isVip) const;

    /// @brief Возвращает идентификатор выставки.
    const std::string& exhibitionId() const noexcept { return exhibitionId_; }
    /// @brief Возвращает наименование.
    const std::string& title() const noexcept { return title_; }
    /// @brief Возвращает базовую цену билета.
    double baseTicketPrice() const noexcept { return baseTicketPrice_; }
    /// @brief Возвращает true, если выставка открыта.
    bool isOpen() const noexcept { return isOpen_; }
    /// @brief Возвращает число экспонатов на выставке.
    std::size_t exhibitsCount() const noexcept { return artifactIds_.size(); }

private:
    std::string exhibitionId_; ///< Уникальный идентификатор выставки.
    std::string title_; ///< Наименование экспоната или выставки.
    std::string theme_; ///< Тематическая категория экспозиции.
    double baseTicketPrice_; ///< Базовая стоимость стандартного входного билета.
    bool isTemporary_; ///< Признак временной (гастрольной) выставки.
    bool isOpen_; ///< Признак открытого статуса выставки.
    std::vector<std::string> artifactIds_; ///< Список инвентарных номеров экспонатов выставки.
};

/**
 * @class ExcursionRoute
 * @brief Маршрут экскурсии по залам музея.
 * @details Упорядоченная последовательность залов экскурсии с расчётом длительности и ориентировочной пешеходной дистанции.
 * @see ExhibitionHall, GuidedTour
 */
class ExcursionRoute {
public:
    /**
     * @brief Конструктор класса ExcursionRoute.
     * @param routeId Идентификатор экскурсионного маршрута.
     * @param routeTitle Название экскурсионного маршрута.
     */
    ExcursionRoute(const std::string& routeId = "RT-01",
                   const std::string& routeTitle = "Grand Overview");

    /**
     * @brief Добавляет зал в последовательность маршрута и увеличивает длительность (минимум 5 мин на зал).
     * @param hall Выставочный зал, задействованный в операции.
     * @param minutesInHall Плановое время пребывания группы в зале (мин, минимум 5).
     */
    void appendHallStop(const ExhibitionHall& hall, int minutesInHall);
    /**
     * @brief Проверяет, включён ли зал hallNumber в текущий экскурсионный маршрут.
     * @param hallNumber Порядковый номер выставочного зала.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool includesHall(int hallNumber) const;
    /**
     * @brief Оценивает протяжённость маршрута из расчёта 120 м на каждую остановку в зале.
     * @return Значение типа @c double, вычисленное методом.
     */
    double estimateWalkingDistanceMeters() const;

    /// @brief Возвращает идентификатор маршрута.
    const std::string& routeId() const noexcept { return routeId_; }
    /// @brief Возвращает расчётную длительность маршрута в минутах.
    int durationMinutes() const noexcept { return durationMinutes_; }
    /// @brief Возвращает число залов-остановок в маршруте.
    std::size_t stopsCount() const noexcept { return hallSequence_.size(); }

private:
    std::string routeId_; ///< Идентификатор экскурсионного маршрута.
    std::string routeTitle_; ///< Название экскурсионного маршрута.
    int durationMinutes_; ///< Суммарная продолжительность маршрута в минутах.
    std::vector<int> hallSequence_; ///< Последовательность номеров залов маршрута.
};

/**
 * @class GuidedTour
 * @brief Групповая экскурсия с экскурсоводом.
 * @details Экскурсионная группа с квотой участников, выбранным языком и привязанным маршрутом обхода залов.
 * @see ExcursionRoute, TourGuide
 */
class GuidedTour {
public:
    /**
     * @brief Конструктор класса GuidedTour.
     * @param tourId Уникальный код групповой экскурсии.
     * @param language Язык рукописи, экскурсии или озвучки аудиогида.
     * @param maxParticipants Максимальный размер экскурсионной группы (>= 1).
     * @param route Маршрут следования экскурсии по залам.
     */
    GuidedTour(const std::string& tourId,
               const std::string& language,
               int maxParticipants,
               const ExcursionRoute& route);

    /**
     * @brief Записывает участника в группу с проверкой лимита maxParticipants_.
     * @throw TicketSoldOutException При нарушении бизнес-правил или инвариантов операции.
     */
    void enrollParticipant();
    /**
     * @brief Отменяет запись одного участника (если currentParticipants_ > 0).
     */
    void cancelParticipant();
    /**
     * @brief Проводит экскурсию и помечает её завершённой.
     * @details Требует наличия участников и остановок в маршруте.
     * @throw ExhibitionClosedException При нарушении бизнес-правил или инвариантов операции.
     */
    void conductTour();
    /**
     * @brief Возвращает число оставшихся свободных мест в экскурсионной группе.
     * @return Значение типа @c int, вычисленное методом.
     */
    int availableSpots() const;

    /// @brief Возвращает идентификатор экскурсии.
    const std::string& tourId() const noexcept { return tourId_; }
    /// @brief Возвращает язык проведения экскурсии.
    const std::string& language() const noexcept { return language_; }
    /// @brief Возвращает текущее число записанных участников.
    int currentParticipants() const noexcept { return currentParticipants_; }
    /// @brief Возвращает признак завершённости экскурсии.
    bool isCompleted() const noexcept { return isCompleted_; }
    /// @brief Возвращает маршрут экскурсии.
    const ExcursionRoute& route() const noexcept { return route_; }

private:
    std::string tourId_; ///< Уникальный код групповой экскурсии.
    std::string language_; ///< Язык рукописи, экскурсии или озвучки аудиогида.
    int maxParticipants_; ///< Максимальный размер экскурсионной группы (>= 1).
    int currentParticipants_; ///< Текущее число записанных участников экскурсии.
    bool isCompleted_; ///< Признак завершённого проведения экскурсии.
    ExcursionRoute route_; ///< Маршрут следования экскурсии по залам.
};

/**
 * @class AudioGuideDevice
 * @brief Индивидуальное устройство аудиогида для посетителей.
 * @details Персональный электронный аудиогид с учётом заряда батареи, языка озвучки и статуса аренды.
 * @see GuidedTour
 */
class AudioGuideDevice {
public:
    /**
     * @brief Конструктор класса AudioGuideDevice.
     * @param deviceId Серийный номер устройства аудиогида.
     * @param batteryPercent Уровень заряда аккумулятора [0..100%].
     * @param selectedLanguage Выбранный язык аудиодорожки.
     * @param loadedTracksCount Количество загруженных аудиотреков.
     */
    AudioGuideDevice(const std::string& deviceId = "AG-101",
                     int batteryPercent = 100,
                     const std::string& selectedLanguage = "Russian",
                     int loadedTracksCount = 35);

    /**
     * @brief Выдаёт аудиогид посетителю на выбранном языке, если устройство свободно и заряд >= 20%.
     * @param language Язык рукописи, экскурсии или озвучки аудиогида.
     * @throw StaffNotAvailableException При нарушении бизнес-правил или инвариантов операции.
     */
    void rentToVisitor(const std::string& language);
    /**
     * @brief Воспроизводит аудиотрек длительностью trackMinutes.
     * @details Разряд батареи max(1, trackMinutes / 2)%.
     * @param trackMinutes Длительность прослушиваемого аудиотрека в минутах.
     */
    void playTrack(int trackMinutes);
    /**
     * @brief Возвращает устройство на станцию и восстанавливает заряд до 100%.
     */
    void returnAndCharge();
    /**
     * @brief Возвращает true, если уровень заряда батареи ниже 20%.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool needsCharging() const;

    /// @brief Возвращает идентификатор аудиогида.
    const std::string& deviceId() const noexcept { return deviceId_; }
    /// @brief Возвращает текущий заряд батареи (%).
    int batteryPercent() const noexcept { return batteryPercent_; }
    /// @brief Возвращает true, если устройство находится у посетителя.
    bool isRented() const noexcept { return isRented_; }
    /// @brief Возвращает активный язык озвучки.
    const std::string& selectedLanguage() const noexcept { return selectedLanguage_; }

private:
    std::string deviceId_; ///< Серийный номер устройства аудиогида.
    int batteryPercent_; ///< Уровень заряда аккумулятора [0..100%].
    std::string selectedLanguage_; ///< Выбранный язык аудиодорожки.
    bool isRented_; ///< Признак выдачи аудиогида посетителю.
    int loadedTracksCount_; ///< Количество загруженных аудиотреков.
};

/**
 * @class SpecialEvent
 * @brief Специальное культурное мероприятие (лекция, Ночь музеев, мастер-класс).
 * @details Культурное мероприятие (лекция, Ночь музеев, мастер-класс) с регистрацией гостей и оценкой выручки.
 * @see ExhibitionSchedule
 */
class SpecialEvent {
public:
    /**
     * @brief Конструктор класса SpecialEvent.
     * @param eventId Идентификатор специального мероприятия.
     * @param eventName Название культурного мероприятия.
     * @param eventType Тип мероприятия (лекция, Ночь музеев, мастер-класс).
     * @param maxGuests Предельное количество гостей (>= 1).
     * @param entryFee Стоимость входного билета на мероприятие.
     */
    SpecialEvent(const std::string& eventId,
                 const std::string& eventName,
                 const std::string& eventType,
                 int maxGuests,
                 double entryFee);

    /**
     * @brief Регистрирует гостя на спецмероприятие при наличии свободных мест.
     * @throw TicketSoldOutException При нарушении бизнес-правил или инвариантов операции.
     */
    void registerGuest();
    /**
     * @brief Рассчитывает ожидаемую выручку: registeredGuests_ * entryFee_.
     * @return Значение типа @c double, вычисленное методом.
     */
    double calculateExpectedRevenue() const;
    /**
     * @brief Возвращает true, если достигнут предел maxGuests_.
     * @return Значение типа @c bool, вычисленное методом.
     */
    bool isSoldOut() const;

    /// @brief Возвращает идентификатор мероприятия.
    const std::string& eventId() const noexcept { return eventId_; }
    /// @brief Возвращает название мероприятия.
    const std::string& eventName() const noexcept { return eventName_; }
    /// @brief Возвращает число зарегистрированных гостей.
    int registeredGuests() const noexcept { return registeredGuests_; }

private:
    std::string eventId_; ///< Идентификатор специального мероприятия.
    std::string eventName_; ///< Название культурного мероприятия.
    std::string eventType_; ///< Тип мероприятия (лекция, Ночь музеев, мастер-класс).
    int maxGuests_; ///< Предельное количество гостей (>= 1).
    int registeredGuests_; ///< Число зарегистрированных гостей мероприятия.
    double entryFee_; ///< Стоимость входного билета на мероприятие.
};

/**
 * @class ExhibitionSchedule
 * @brief Годовое расписание выставок и спецмероприятий музея.
 * @details Сводное годовое расписание выставок и специальных мероприятий музея.
 * @see Exhibition, SpecialEvent
 */
class ExhibitionSchedule {
public:
    /**
     * @brief Конструктор класса ExhibitionSchedule.
     * @param calendarYear Календарный год планирования расписания.
     */
    explicit ExhibitionSchedule(int calendarYear = 2026);

    /**
     * @brief Добавляет выставку в календарное расписание года.
     * @param exhibition Выставка для добавления в расписание, организации или продажи билета.
     */
    void scheduleExhibition(const Exhibition& exhibition);
    /**
     * @brief Добавляет специальное мероприятие в календарный план.
     * @param event Специальное мероприятие для включения в расписание.
     */
    void scheduleEvent(const SpecialEvent& event);
    /**
     * @brief Подсчитывает число выставок в расписании со статусом isOpen() == true.
     * @return Значение типа @c int, вычисленное методом.
     */
    int countOpenExhibitions() const;

    /// @brief Возвращает календарный год расписания.
    int calendarYear() const noexcept { return calendarYear_; }
    /// @brief Возвращает количество запланированных выставок.
    std::size_t totalExhibitions() const noexcept { return activeExhibitions_.size(); }
    /// @brief Возвращает количество запланированных мероприятий.
    std::size_t totalEvents() const noexcept { return plannedEvents_.size(); }

private:
    int calendarYear_; ///< Календарный год планирования расписания.
    std::vector<Exhibition> activeExhibitions_; ///< Запланированные и активные выставки года.
    std::vector<SpecialEvent> plannedEvents_; ///< Запланированные специальные мероприятия.
};

#endif // MUSEUM_EXHIBITIONS_AND_EVENTS_H
