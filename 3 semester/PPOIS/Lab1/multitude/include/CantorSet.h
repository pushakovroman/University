/**
 * @file CantorSet.h
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Заголовочный файл класса CantorSet (Неориентированное канторовское множество).
 * @details Содержит объявление класса CantorSet для работы с неориентированными
 * канторовскими множествами, элементами которых могут быть как атомарные строковые
 * элементы, так и другие вложенные множества произвольной глубины.
 * Реализует теоретико-множественные операции (объединение, пересечение, разность,
 * булеан), проверку принадлежности, сравнение, парсинг из строки и работу с потоками.
 */

#ifndef CANTOR_SET_H
#define CANTOR_SET_H

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @class CantorSet
 * @brief Класс «Неориентированное канторовское множество» (вариант 1.4 / 1.4.1).
 * @details Представляет математическое множество, в котором элементы не повторяются
 * и порядок элементов не имеет значения. Элементом множества может являться как
 * строковый атом (символ/слово), так и другое множество (объект класса CantorSet).
 *
 * В классе используются:
 * - динамическое выделение памяти (`new` / `delete`) для хранения элементов и вложенных множеств;
 * - правило трёх (конструктор копирования, оператор присваивания копированием, деструктор);
 * - спецификатор `mutable` для кэширования строкового представления при константных вызовах;
 * - перегрузка операторов как членов класса, так и дружественных функций (не-членов).
 */
class CantorSet {
public:
    /**
     * @brief Конструктор по умолчанию. Создаёт пустое множество `{}`.
     */
    CantorSet();

    /**
     * @brief Конструктор формирования множества из строки в стиле C++.
     * @details Парсит строковое представление вида `{a, b, c, {a, b}, {}, {a, {c}}}`.
     * Повторяющиеся элементы автоматически исключаются.
     * @param str Строковое представление множества (`const std::string&`).
     * @throw std::invalid_argument Если строка имеет некорректный синтаксис множества.
     * @see fromString
     */
    explicit CantorSet(const std::string& str);

    /**
     * @brief Конструктор формирования множества из строки в стиле C (`const char*`).
     * @details Парсит строковое представление вида `{a, b, c, {a, b}, {}, {a, {c}}}`.
     * @param str Указатель на C-строку с представлением множества.
     * @throw std::invalid_argument Если указатель равен `nullptr` или синтаксис некорректен.
     * @see CantorSet(const std::string&)
     */
    explicit CantorSet(const char* str);

    /**
     * @brief Конструктор копирования (глубокое копирование).
     * @details Создаёт полную независимую копию множества `other`, включая все
     * динамически выделенные вложенные подмножества.
     * @param other Копируемое множество.
     */
    CantorSet(const CantorSet& other);

    /**
     * @brief Деструктор класса.
     * @details Освобождает всю динамически выделенную память под элементы и вложенные множества.
     */
    ~CantorSet();

    /**
     * @brief Оператор присваивания копированием.
     * @details Выполняет проверку на самоприсваивание (`this == &other`), освобождает
     * старые ресурсы и выполняет глубокое копирование из `other`.
     * @param other Присваиваемое множество.
     * @return Ссылка на текущий объект (`*this`).
     */
    CantorSet& operator=(const CantorSet& other);

    /**
     * @brief Оператор присваивания из C++-строки.
     * @param str Строковое представление множества.
     * @return Ссылка на текущий объект (`*this`).
     * @throw std::invalid_argument При некорректном формате строки.
     */
    CantorSet& operator=(const std::string& str);

    /**
     * @brief Оператор присваивания из C-строки.
     * @param str Указатель на C-строку с представлением множества.
     * @return Ссылка на текущий объект (`*this`).
     * @throw std::invalid_argument При `nullptr` или некорректном формате строки.
     */
    CantorSet& operator=(const char* str);

    /**
     * @brief Проверяет, является ли множество пустым.
     * @details Определён внутри класса (inline) для демонстрации определения методов внутри класса.
     * @return `true`, если мощность множества равна 0, иначе `false`.
     * @see cardinality
     */
    bool isEmpty() const {
        return this->size_ == 0;
    }

    /**
     * @brief Определяет мощность (количество элементов верхнего уровня) множества.
     * @details Определён внутри класса (inline).
     * @return Количество элементов в множестве (`std::size_t`).
     * @see isEmpty
     */
    std::size_t cardinality() const {
        return this->size_;
    }

    /**
     * @brief Очищает множество, удаляя все его элементы.
     */
    void clear();

    /**
     * @brief Добавляет атомарный строковый элемент (`std::string`) в множество.
     * @details Если такой элемент уже присутствует в множестве, повторно он не добавляется.
     * Имя элемента не должно содержать фигурных скобок, запятых или быть пустым.
     * @param element Имя добавляемого атомарного элемента.
     * @return `true`, если элемент был успешно добавлен; `false`, если он уже существовал.
     * @throw std::invalid_argument Если имя элемента пустое или содержит запрещённые символы (`{`, `}`, `,`).
     */
    bool add(const std::string& element);

    /**
     * @brief Добавляет атомарный элемент из C-строки (`const char*`) в множество.
     * @param element C-строка с именем элемента.
     * @return `true`, если элемент был добавлен; `false`, если он уже существовал.
     * @throw std::invalid_argument Если `element == nullptr`, строка пуста или содержит запрещённые символы.
     */
    bool add(const char* element);

    /**
     * @brief Добавляет другое множество в качестве элемента данного множества.
     * @details Создаёт глубокую динамическую копию `subset` и добавляет её, если
     * равное подмножество ещё не является элементом данного множества.
     * @param subset Вложенное множество для добавления.
     * @return `true`, если подмножество было добавлено; `false`, если оно уже присутствовало.
     */
    bool add(const CantorSet& subset);

    /**
     * @brief Удаляет атомарный строковый элемент (`std::string`) из множества.
     * @param element Имя удаляемого атомарного элемента.
     * @return `true`, если элемент был найден и удалён; `false`, если такого элемента не было.
     */
    bool remove(const std::string& element);

    /**
     * @brief Удаляет атомарный элемент (`const char*`) из множества.
     * @param element C-строка с именем удаляемого элемента.
     * @return `true`, если элемент был найден и удалён; `false`, если не найден.
     * @throw std::invalid_argument Если `element == nullptr`.
     */
    bool remove(const char* element);

    /**
     * @brief Удаляет вложенное множество из элементов данного множества.
     * @param subset Вложенное множество, которое нужно удалить.
     * @return `true`, если подмножество было найдено и удалено; `false` в противном случае.
     */
    bool remove(const CantorSet& subset);

    /**
     * @brief Оператор `[]` проверки принадлежности атомарного элемента (`std::string`) множеству.
     * @param element Проверяемый строковый элемент.
     * @return `true`, если элемент принадлежит множеству, иначе `false`.
     */
    bool operator[](const std::string& element) const;

    /**
     * @brief Оператор `[]` проверки принадлежности атомарного элемента (`const char*`) множеству.
     * @param element Проверяемый элемент в виде C-строки.
     * @return `true`, если элемент принадлежит множеству, иначе `false`.
     * @throw std::invalid_argument Если `element == nullptr`.
     */
    bool operator[](const char* element) const;

    /**
     * @brief Оператор `[]` проверки принадлежности вложенного множества данному множеству.
     * @param subset Проверяемое множество-элемент.
     * @return `true`, если `subset` является элементом данного множества, иначе `false`.
     */
    bool operator[](const CantorSet& subset) const;

    /**
     * @brief Именованный метод проверки принадлежности атомарного элемента.
     * @param element Имя атомарного элемента.
     * @return `true`, если элемент принадлежит множеству, иначе `false`.
     * @see operator[]
     */
    bool contains(const std::string& element) const;

    /**
     * @brief Именованный метод проверки принадлежности вложенного множества.
     * @param subset Проверяемое множество-элемент.
     * @return `true`, если подмножество является элементом множества, иначе `false`.
     * @see operator[]
     */
    bool contains(const CantorSet& subset) const;

    /**
     * @brief Составной оператор объединения множеств (`+=`).
     * @details Добавляет в текущее множество все элементы из `other`, которых ещё нет в `*this`.
     * @param other Второе множество для объединения.
     * @return Ссылка на текущий объект (`*this`).
     * @see operator+
     */
    CantorSet& operator+=(const CantorSet& other);

    /**
     * @brief Оператор объединения двух множеств (`+`).
     * @param other Второе множество.
     * @return Новое множество, содержащее элементы из `*this` или `other`.
     * @see operator+=
     */
    CantorSet operator+(const CantorSet& other) const;

    /**
     * @brief Составной оператор пересечения множеств (`*=`).
     * @details Оставляет в текущем множестве только те элементы, которые также принадлежат `other`.
     * @param other Второе множество для пересечения.
     * @return Ссылка на текущий объект (`*this`).
     * @see operator*
     */
    CantorSet& operator*=(const CantorSet& other);

    /**
     * @brief Оператор пересечения двух множеств (`*`).
     * @param other Второе множество.
     * @return Новое множество, содержащее только общие элементы `*this` и `other`.
     * @see operator*=
     */
    CantorSet operator*(const CantorSet& other) const;

    /**
     * @brief Составной оператор разности множеств (`-=`).
     * @details Удаляет из текущего множества все элементы, принадлежащие `other`.
     * @param other Вычитаемое множество.
     * @return Ссылка на текущий объект (`*this`).
     * @see operator-
     */
    CantorSet& operator-=(const CantorSet& other);

    /**
     * @brief Оператор разности двух множеств (`-`).
     * @param other Вычитаемое множество.
     * @return Новое множество, содержащее элементы `*this`, не принадлежащие `other`.
     * @see operator-=
     */
    CantorSet operator-(const CantorSet& other) const;

    /**
     * @brief Построение булеана (множества всех подмножеств) данного множества.
     * @details Для множества мощности $n$ возвращает новое множество мощности $2^n$,
     * каждым элементом которого является подмножество исходного множества.
     * @return Объект `CantorSet`, представляющий булеан $P(A)$.
     */
    CantorSet powerSet() const;

    /**
     * @brief Оператор проверки двух множеств на равенство (`==`).
     * @details Два канторовских множества равны тогда и только тогда, когда их мощности
     * совпадают и каждый элемент одного множества принадлежит другому (с учётом рекурсивного
     * сравнения вложенных множеств, независимо от порядка следования элементов).
     * @param other Множество для сравнения.
     * @return `true`, если множества равны, иначе `false`.
     */
    bool operator==(const CantorSet& other) const;

    /**
     * @brief Оператор проверки двух множеств на неравенство (`!=`).
     * @param other Множество для сравнения.
     * @return `true`, если множества не равны, иначе `false`.
     */
    bool operator!=(const CantorSet& other) const;

    /**
     * @brief Оператор нестрогого включения (`<=` — является ли `*this` подмножеством `other`).
     * @param other Надмножество для проверки.
     * @return `true`, если каждый элемент `*this` принадлежит `other`.
     */
    bool operator<=(const CantorSet& other) const;

    /**
     * @brief Оператор строгого включения (`<` — является ли `*this` собственным подмножеством `other`).
     * @param other Надмножество для проверки.
     * @return `true`, если `*this <= other` и `*this != other`.
     */
    bool operator<(const CantorSet& other) const;

    /**
     * @brief Оператор нестрогого включения надмножества (`>=`).
     * @param other Подмножество для проверки.
     * @return `true`, если `other <= *this`.
     */
    bool operator>=(const CantorSet& other) const;

    /**
     * @brief Оператор строгого включения надмножества (`>`).
     * @param other Подмножество для проверки.
     * @return `true`, если `other < *this`.
     */
    bool operator>(const CantorSet& other) const;

    /**
     * @brief Возвращает строковое представление множества в формате `{a, b, {c}}`.
     * @details Использует кэширование через `mutable`-поля `cachedString_` и `isCacheValid_`,
     * чтобы не пересобирать строку при повторных константных обращениях.
     * @return Константная ссылка на строковое представление множества.
     */
    const std::string& toString() const;

    /**
     * @brief Статический метод создания множества из C++-строки.
     * @param str Строка вида `{a, b, {c}}`.
     * @return Сформированный объект `CantorSet`.
     * @throw std::invalid_argument При синтаксической ошибке в строке.
     */
    static CantorSet fromString(const std::string& str);

    /**
     * @brief Статический метод создания множества из C-строки.
     * @param str Указатель на C-строку вида `{a, b, {c}}`.
     * @return Сформированный объект `CantorSet`.
     * @throw std::invalid_argument При `nullptr` или синтаксической ошибке в строке.
     */
    static CantorSet fromString(const char* str);

    /**
     * @brief Статический метод проверки корректности строковой записи множества.
     * @param str Проверяемая строка.
     * @return `true`, если строка является синтаксически корректной записью множества, иначе `false`.
     */
    static bool isValidSetString(const std::string& str);

    /**
     * @brief Перегрузка потокового оператора вывода (`<<`) как функции не-члена (friend).
     * @param os Выходной поток (`std::ostream&`).
     * @param set Выводимое множество (`const CantorSet&`).
     * @return Ссылка на выходной поток `os`.
     */
    friend std::ostream& operator<<(std::ostream& os, const CantorSet& set);

    /**
     * @brief Перегрузка потокового оператора ввода (`>>`) как функции не-члена (friend).
     * @details Считывает из входного потока строковое представление множества со сбалансированными
     * фигурными скобками `{...}` и формирует объект `set`. При ошибке формата устанавливает
     * флаг `std::ios::failbit` у потока.
     * @param is Входной поток (`std::istream&`).
     * @param set Множество, в которое записывается результат (`CantorSet&`).
     * @return Ссылка на входной поток `is`.
     */
    friend std::istream& operator>>(std::istream& is, CantorSet& set);

private:
    /**
     * @struct Element
     * @brief Внутренняя структура для представления одного элемента канторовского множества.
     * @details Элемент может быть либо атомарной строкой (`isSubset == false`),
     * либо указателем на динамически выделенное вложенное множество (`isSubset == true`).
     */
    struct Element {
        bool isSubset;          ///< Флаг: `true`, если элемент — вложенное множество, `false` — если атом.
        std::string atom;       ///< Значение атомарного элемента (используется при `isSubset == false`).
        CantorSet* subset;      ///< Указатель на вложенное множество (при `isSubset == true`).

        Element();
        explicit Element(const std::string& atomVal);
        explicit Element(const CantorSet& subsetVal);
        Element(const Element& other);
        Element& operator=(const Element& other);
        ~Element();

        bool equals(const Element& other) const;
        std::string toString() const;
    };

    Element* elements_;                 ///< Динамический массив элементов множества.
    std::size_t size_;                  ///< Текущее количество элементов (мощность).
    std::size_t capacity_;              ///< Текущая ёмкость динамического массива.

    mutable std::string cachedString_;  ///< Кэшированное строковое представление (`mutable`).
    mutable bool isCacheValid_;         ///< Флаг актуальности строкового кэша (`mutable`).

    /**
     * @brief Увеличивает ёмкость динамического массива элементов при необходимости.
     * @param minCapacity Минимально требуемая ёмкость.
     */
    void ensureCapacity(std::size_t minCapacity);

    /**
     * @brief Сбрасывает флаг актуальности кэша строкового представления.
     */
    void invalidateCache() const;

    /**
     * @brief Удаляет элемент по индексу со сдвигом остальных элементов.
     * @param index Индекс удаляемого элемента.
     */
    void removeAt(std::size_t index);

    /**
     * @brief Внутренний парсер строкового представления множества.
     * @param str Входная строка.
     * @throw std::invalid_argument При нарушении синтаксиса.
     */
    void parseFromString(const std::string& str);

    /**
     * @brief Вспомогательный метод удаления пробельных символов по краям строки.
     * @param s Исходная строка.
     * @return Обрезанная строка без начальных и конечных пробелов.
     */
    static std::string trim(const std::string& s);

    /**
     * @brief Проверяет допустимость имени атомарного элемента.
     * @param atom Проверяемое имя.
     * @return `true`, если имя не пустое и не содержит `{`, `}`, `,`.
     */
    static bool isValidAtomName(const std::string& atom);
};

#endif // CANTOR_SET_H
