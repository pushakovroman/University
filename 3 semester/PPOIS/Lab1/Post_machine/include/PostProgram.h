/**
 * @file PostProgram.h
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Заголовочный файл класса PostProgram (Программа из правил для машины Поста).
 * @details Реализует хранение, добавление, удаление, поиск и просмотр правил машины Поста,
 * а также чтение программы из потока и вывод в поток.
 */

#ifndef POST_PROGRAM_H
#define POST_PROGRAM_H

#include "PostRule.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @class PostProgram
 * @brief Класс, управляющий набором правил (программой) машины Поста.
 * @details Хранит правила в динамически выделяемом массиве (`new PostRule[]` / `delete[]`),
 * упорядоченном по номеру правила (`id`). Поддерживает конструктор копирования,
 * оператор присваивания, деструктор, операторы `+=`, `-=`, `[]`, `==`, `!=`, `<<`, `>>`
 * и кэширование строкового представления через `mutable`.
 */
class PostProgram {
public:
    /**
     * @brief Конструктор по умолчанию. Создаёт пустую программу.
     */
    PostProgram();

    /**
     * @brief Конструктор копирования (глубокое копирование динамического массива правил).
     * @param other Копируемая программа.
     */
    PostProgram(const PostProgram& other);

    /**
     * @brief Деструктор класса. Освобождает динамический массив правил (`delete[]`).
     */
    ~PostProgram();

    /**
     * @brief Оператор присваивания копированием.
     * @param other Присваиваемая программа.
     * @return Ссылка на текущий объект (`*this`).
     */
    PostProgram& operator=(const PostProgram& other);

    /**
     * @brief Проверяет, пуста ли программа (inline-метод).
     * @return `true`, если в программе нет ни одного правила.
     */
    bool isEmpty() const {
        return this->size_ == 0;
    }

    /**
     * @brief Возвращает количество правил в программе (inline-метод).
     * @return Число правил (`std::size_t`).
     */
    std::size_t size() const {
        return this->size_;
    }

    /**
     * @brief Очищает программу, удаляя все правила.
     */
    void clear();

    /**
     * @brief Добавляет новое правило или заменяет существующее правило с тем же номером `id`.
     * @param rule Добавляемое правило.
     */
    void addRule(const PostRule& rule);

    /**
     * @brief Добавляет правило, заданное строкой (например, `"1: > 2"`).
     * @param ruleStr Строковое описание правила.
     * @throw std::invalid_argument При некорректном формате правила.
     */
    void addRule(const std::string& ruleStr);

    /**
     * @brief Оператор `+=` для добавления правила в программу.
     * @param rule Добавляемое правило.
     * @return Ссылка на текущий объект (`*this`).
     */
    PostProgram& operator+=(const PostRule& rule);

    /**
     * @brief Удаляет правило по его номеру `ruleId`.
     * @param ruleId Номер удаляемого правила.
     * @return `true`, если правило было найдено и удалено; `false`, если такого правила не было.
     */
    bool removeRule(int ruleId);

    /**
     * @brief Оператор `-=` для удаления правила по его номеру.
     * @param ruleId Номер удаляемого правила.
     * @return Ссылка на текущий объект (`*this`).
     */
    PostProgram& operator-=(int ruleId);

    /**
     * @brief Проверяет наличие правила с указанным номером `ruleId`.
     * @param ruleId Номер искомого правила.
     * @return `true`, если правило присутствует в программе, иначе `false`.
     */
    bool hasRule(int ruleId) const;

    /**
     * @brief Возвращает правило по его уникальному номеру `ruleId`.
     * @param ruleId Номер правила.
     * @return Константная ссылка на найденное правило `PostRule`.
     * @throw std::out_of_range Если правило с номером `ruleId` не найдено.
     */
    const PostRule& getRule(int ruleId) const;

    /**
     * @brief Оператор `[]` получения правила по его номеру `ruleId`.
     * @param ruleId Номер правила.
     * @return Константная ссылка на правило `PostRule`.
     * @throw std::out_of_range Если правило с номером `ruleId` отсутствует.
     */
    const PostRule& operator[](int ruleId) const;

    /**
     * @brief Возвращает правило по порядковому индексу в массиве `[0 .. size() - 1]`.
     * @param index Порядковый индекс.
     * @return Константная ссылка на правило.
     * @throw std::out_of_range Если `index >= size()`.
     */
    const PostRule& ruleAtIndex(std::size_t index) const;

    /**
     * @brief Возвращает минимальный номер правила в программе (стартовое правило по умолчанию).
     * @return Номер первого правила или `-1`, если программа пуста.
     */
    int firstRuleId() const;

    /**
     * @brief Загружает программу из входного потока (построчно).
     * @details Пустые строки и строки-комментарии (начинающиеся с `#` или `//`) пропускаются.
     * @param is Входной поток.
     * @return Ссылка на входной поток.
     * @throw std::invalid_argument При обнаружении синтаксически некорректного правила.
     */
    std::istream& loadFromStream(std::istream& is);

    /**
     * @brief Возвращает многострочное текстовое представление всех правил программы.
     * @details Использует `mutable`-кэш для ускорения повторных константных вызовов.
     * @return Константная ссылка на текстовое представление программы.
     */
    const std::string& toString() const;

    /**
     * @brief Оператор сравнения двух программ на равенство (`==`).
     * @param other Другая программа.
     * @return `true`, если наборы правил полностью совпадают.
     */
    bool operator==(const PostProgram& other) const;

    /**
     * @brief Оператор сравнения двух программ на неравенство (`!=`).
     * @param other Другая программа.
     * @return `true`, если программы различаются.
     */
    bool operator!=(const PostProgram& other) const;

    /**
     * @brief Потоковый оператор вывода программы (`<<`).
     * @param os Выходной поток.
     * @param program Выводимая программа.
     * @return Ссылка на выходной поток.
     */
    friend std::ostream& operator<<(std::ostream& os, const PostProgram& program);

    /**
     * @brief Потоковый оператор ввода программы (`>>`).
     * @param is Входной поток.
     * @param program Заполняемая программа.
     * @return Ссылка на входной поток.
     */
    friend std::istream& operator>>(std::istream& is, PostProgram& program);

private:
    PostRule* rules_;                   ///< Динамический массив правил.
    std::size_t size_;                  ///< Количество правил в программе.
    std::size_t capacity_;              ///< Ёмкость динамического массива.

    mutable std::string cachedText_;    ///< Кэшированное текстовое представление (`mutable`).
    mutable bool isCacheValid_;         ///< Флаг актуальности кэша (`mutable`).

    /**
     * @brief Увеличивает ёмкость динамического массива правил при необходимости.
     * @param minCapacity Минимально требуемая ёмкость.
     */
    void ensureCapacity(std::size_t minCapacity);

    /**
     * @brief Сбрасывает кэш текстового представления.
     */
    void invalidateCache() const;
};

#endif // POST_PROGRAM_H
