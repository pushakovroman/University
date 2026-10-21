/**
 * @file CantorSet.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Реализация методов и операторов класса CantorSet.
 * @details Содержит определения конструкторов, деструктора, теоретико-множественных
 * операций, парсера строкового представления и потоковых операторов ввода/вывода.
 */

#include "CantorSet.h"

#include <cctype>
#include <sstream>
#include <vector>

// ============================================================================
// Реализация внутренней структуры CantorSet::Element
// ============================================================================

CantorSet::Element::Element()
    : isSubset(false), atom(""), subset(nullptr) {}

CantorSet::Element::Element(const std::string& atomVal)
    : isSubset(false), atom(atomVal), subset(nullptr) {}

CantorSet::Element::Element(const CantorSet& subsetVal)
    : isSubset(true), atom(""), subset(new CantorSet(subsetVal)) {}

CantorSet::Element::Element(const Element& other)
    : isSubset(other.isSubset),
      atom(other.atom),
      subset(other.isSubset && other.subset != nullptr ? new CantorSet(*other.subset) : nullptr) {}

CantorSet::Element& CantorSet::Element::operator=(const Element& other) {
    if (this == &other) {
        return *this;
    }
    CantorSet* newSubset = nullptr;
    if (other.isSubset && other.subset != nullptr) {
        newSubset = new CantorSet(*other.subset);
    }
    delete this->subset;
    this->isSubset = other.isSubset;
    this->atom = other.atom;
    this->subset = newSubset;
    return *this;
}

CantorSet::Element::~Element() {
    delete this->subset;
    this->subset = nullptr;
}

bool CantorSet::Element::equals(const Element& other) const {
    if (this->isSubset != other.isSubset) {
        return false;
    }
    if (this->isSubset) {
        return (*this->subset) == (*other.subset);
    }
    return this->atom == other.atom;
}

std::string CantorSet::Element::toString() const {
    if (this->isSubset) {
        return this->subset->toString();
    }
    return this->atom;
}

// ============================================================================
// Конструкторы, деструктор и операторы присваивания CantorSet
// ============================================================================

CantorSet::CantorSet()
    : elements_(nullptr),
      size_(0),
      capacity_(0),
      cachedString_("{}"),
      isCacheValid_(true) {}

CantorSet::CantorSet(const std::string& str)
    : elements_(nullptr),
      size_(0),
      capacity_(0),
      cachedString_(""),
      isCacheValid_(false) {
    this->parseFromString(str);
}

CantorSet::CantorSet(const char* str)
    : elements_(nullptr),
      size_(0),
      capacity_(0),
      cachedString_(""),
      isCacheValid_(false) {
    if (str == nullptr) {
        throw std::invalid_argument("Null pointer passed to CantorSet(const char*)");
    }
    this->parseFromString(std::string(str));
}

CantorSet::CantorSet(const CantorSet& other)
    : elements_(nullptr),
      size_(other.size_),
      capacity_(other.size_),
      cachedString_(other.cachedString_),
      isCacheValid_(other.isCacheValid_) {
    if (other.size_ > 0) {
        this->elements_ = new Element[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            this->elements_[i] = other.elements_[i];
        }
    }
}

CantorSet::~CantorSet() {
    delete[] this->elements_;
    this->elements_ = nullptr;
    this->size_ = 0;
    this->capacity_ = 0;
}

CantorSet& CantorSet::operator=(const CantorSet& other) {
    if (this == &other) {
        return *this;
    }
    Element* newElements = nullptr;
    if (other.size_ > 0) {
        newElements = new Element[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            newElements[i] = other.elements_[i];
        }
    }
    delete[] this->elements_;
    this->elements_ = newElements;
    this->size_ = other.size_;
    this->capacity_ = other.size_;
    this->cachedString_ = other.cachedString_;
    this->isCacheValid_ = other.isCacheValid_;
    return *this;
}

CantorSet& CantorSet::operator=(const std::string& str) {
    CantorSet parsed(str);
    *this = parsed;
    return *this;
}

CantorSet& CantorSet::operator=(const char* str) {
    if (str == nullptr) {
        throw std::invalid_argument("Null pointer passed to CantorSet::operator=(const char*)");
    }
    CantorSet parsed(str);
    *this = parsed;
    return *this;
}

// ============================================================================
// Управление памятью и кэшем
// ============================================================================

void CantorSet::ensureCapacity(std::size_t minCapacity) {
    if (this->capacity_ >= minCapacity) {
        return;
    }
    std::size_t newCapacity = (this->capacity_ == 0) ? 4 : this->capacity_ * 2;
    if (newCapacity < minCapacity) {
        newCapacity = minCapacity;
    }
    Element* newElements = new Element[newCapacity];
    for (std::size_t i = 0; i < this->size_; ++i) {
        newElements[i] = this->elements_[i];
    }
    delete[] this->elements_;
    this->elements_ = newElements;
    this->capacity_ = newCapacity;
}

void CantorSet::invalidateCache() const {
    this->isCacheValid_ = false;
    this->cachedString_.clear();
}

void CantorSet::clear() {
    delete[] this->elements_;
    this->elements_ = nullptr;
    this->size_ = 0;
    this->capacity_ = 0;
    this->cachedString_ = "{}";
    this->isCacheValid_ = true;
}

void CantorSet::removeAt(std::size_t index) {
    for (std::size_t i = index; i + 1 < this->size_; ++i) {
        this->elements_[i] = this->elements_[i + 1];
    }
    this->elements_[this->size_ - 1] = Element();
    --this->size_;
    this->invalidateCache();
}

// ============================================================================
// Добавление, удаление и проверка принадлежности элементов
// ============================================================================

std::string CantorSet::trim(const std::string& s) {
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(start, end - start);
}

bool CantorSet::isValidAtomName(const std::string& atom) {
    if (atom.empty()) {
        return false;
    }
    for (std::size_t i = 0; i < atom.size(); ++i) {
        char c = atom[i];
        if (c == '{' || c == '}' || c == ',' || std::isspace(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool CantorSet::add(const std::string& element) {
    std::string cleaned = trim(element);
    if (!isValidAtomName(cleaned)) {
        throw std::invalid_argument("Invalid atomic element name: '" + element + "'");
    }
    if ((*this)[cleaned]) {
        return false;
    }
    this->ensureCapacity(this->size_ + 1);
    this->elements_[this->size_] = Element(cleaned);
    ++this->size_;
    this->invalidateCache();
    return true;
}

bool CantorSet::add(const char* element) {
    if (element == nullptr) {
        throw std::invalid_argument("Null pointer passed to CantorSet::add(const char*)");
    }
    return this->add(std::string(element));
}

bool CantorSet::add(const CantorSet& subset) {
    if ((*this)[subset]) {
        return false;
    }
    this->ensureCapacity(this->size_ + 1);
    this->elements_[this->size_] = Element(subset);
    ++this->size_;
    this->invalidateCache();
    return true;
}

bool CantorSet::remove(const std::string& element) {
    std::string cleaned = trim(element);
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (!this->elements_[i].isSubset && this->elements_[i].atom == cleaned) {
            this->removeAt(i);
            return true;
        }
    }
    return false;
}

bool CantorSet::remove(const char* element) {
    if (element == nullptr) {
        throw std::invalid_argument("Null pointer passed to CantorSet::remove(const char*)");
    }
    return this->remove(std::string(element));
}

bool CantorSet::remove(const CantorSet& subset) {
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->elements_[i].isSubset && (*this->elements_[i].subset) == subset) {
            this->removeAt(i);
            return true;
        }
    }
    return false;
}

bool CantorSet::operator[](const std::string& element) const {
    std::string cleaned = trim(element);
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (!this->elements_[i].isSubset && this->elements_[i].atom == cleaned) {
            return true;
        }
    }
    return false;
}

bool CantorSet::operator[](const char* element) const {
    if (element == nullptr) {
        throw std::invalid_argument("Null pointer passed to CantorSet::operator[](const char*)");
    }
    return (*this)[std::string(element)];
}

bool CantorSet::operator[](const CantorSet& subset) const {
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->elements_[i].isSubset && (*this->elements_[i].subset) == subset) {
            return true;
        }
    }
    return false;
}

bool CantorSet::contains(const std::string& element) const {
    return (*this)[element];
}

bool CantorSet::contains(const CantorSet& subset) const {
    return (*this)[subset];
}

// ============================================================================
// Теоретико-множественные операции (+, +=, *, *=, -, -=, булеан)
// ============================================================================

CantorSet& CantorSet::operator+=(const CantorSet& other) {
    if (this == &other) {
        return *this;
    }
    for (std::size_t i = 0; i < other.size_; ++i) {
        if (other.elements_[i].isSubset) {
            this->add(*other.elements_[i].subset);
        } else {
            this->add(other.elements_[i].atom);
        }
    }
    return *this;
}

CantorSet CantorSet::operator+(const CantorSet& other) const {
    CantorSet result(*this);
    result += other;
    return result;
}

CantorSet& CantorSet::operator*=(const CantorSet& other) {
    if (this == &other) {
        return *this;
    }
    CantorSet intersection;
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->elements_[i].isSubset) {
            if (other[*this->elements_[i].subset]) {
                intersection.add(*this->elements_[i].subset);
            }
        } else {
            if (other[this->elements_[i].atom]) {
                intersection.add(this->elements_[i].atom);
            }
        }
    }
    *this = intersection;
    return *this;
}

CantorSet CantorSet::operator*(const CantorSet& other) const {
    CantorSet result(*this);
    result *= other;
    return result;
}

CantorSet& CantorSet::operator-=(const CantorSet& other) {
    if (this == &other) {
        this->clear();
        return *this;
    }
    for (std::size_t i = 0; i < other.size_; ++i) {
        if (other.elements_[i].isSubset) {
            this->remove(*other.elements_[i].subset);
        } else {
            this->remove(other.elements_[i].atom);
        }
    }
    return *this;
}

CantorSet CantorSet::operator-(const CantorSet& other) const {
    CantorSet result(*this);
    result -= other;
    return result;
}

CantorSet CantorSet::powerSet() const {
    if (this->size_ >= sizeof(std::size_t) * 8 - 1) {
        throw std::overflow_error("Set cardinality is too large to compute power set");
    }
    CantorSet result;
    const std::size_t totalSubsets = static_cast<std::size_t>(1) << this->size_;
    for (std::size_t mask = 0; mask < totalSubsets; ++mask) {
        CantorSet subset;
        for (std::size_t bit = 0; bit < this->size_; ++bit) {
            if ((mask & (static_cast<std::size_t>(1) << bit)) != 0) {
                if (this->elements_[bit].isSubset) {
                    subset.add(*this->elements_[bit].subset);
                } else {
                    subset.add(this->elements_[bit].atom);
                }
            }
        }
        result.add(subset);
    }
    return result;
}

// ============================================================================
// Операторы сравнения (==, !=, <=, <, >=, >)
// ============================================================================

bool CantorSet::operator<=(const CantorSet& other) const {
    if (this->size_ > other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->elements_[i].isSubset) {
            if (!other[*this->elements_[i].subset]) {
                return false;
            }
        } else {
            if (!other[this->elements_[i].atom]) {
                return false;
            }
        }
    }
    return true;
}

bool CantorSet::operator==(const CantorSet& other) const {
    if (this == &other) {
        return true;
    }
    if (this->size_ != other.size_) {
        return false;
    }
    return (*this) <= other;
}

bool CantorSet::operator!=(const CantorSet& other) const {
    return !((*this) == other);
}

bool CantorSet::operator<(const CantorSet& other) const {
    return (this->size_ < other.size_) && ((*this) <= other);
}

bool CantorSet::operator>=(const CantorSet& other) const {
    return other <= (*this);
}

bool CantorSet::operator>(const CantorSet& other) const {
    return other < (*this);
}

// ============================================================================
// Форматирование, парсинг из строки и работа с потоками
// ============================================================================

const std::string& CantorSet::toString() const {
    if (!this->isCacheValid_) {
        std::ostringstream oss;
        oss << "{";
        for (std::size_t i = 0; i < this->size_; ++i) {
            if (i > 0) {
                oss << ", ";
            }
            oss << this->elements_[i].toString();
        }
        oss << "}";
        this->cachedString_ = oss.str();
        this->isCacheValid_ = true;
    }
    return this->cachedString_;
}

void CantorSet::parseFromString(const std::string& str) {
    this->clear();
    std::string trimmed = trim(str);
    if (trimmed.size() < 2 || trimmed.front() != '{' || trimmed.back() != '}') {
        throw std::invalid_argument("Set string must start with '{' and end with '}': " + str);
    }

    // Проверяем, что внешняя открывающая скобка закрывается именно последним символом строки
    int braceDepth = 0;
    for (std::size_t i = 0; i < trimmed.size(); ++i) {
        if (trimmed[i] == '{') {
            ++braceDepth;
        } else if (trimmed[i] == '}') {
            --braceDepth;
            if (braceDepth < 0) {
                throw std::invalid_argument("Unbalanced closing brace in set string: " + str);
            }
            if (braceDepth == 0 && i + 1 < trimmed.size()) {
                throw std::invalid_argument("Unexpected characters after closing brace in: " + str);
            }
        }
    }
    if (braceDepth != 0) {
        throw std::invalid_argument("Unbalanced braces in set string: " + str);
    }

    std::string inner = trim(trimmed.substr(1, trimmed.size() - 2));
    if (inner.empty()) {
        return; // Пустое множество {}
    }

    // Разбиваем содержимое верхнего уровня по запятым (с учётом вложенности скобок)
    std::vector<std::string> tokens;
    std::size_t tokenStart = 0;
    int depth = 0;
    for (std::size_t i = 0; i < inner.size(); ++i) {
        char c = inner[i];
        if (c == '{') {
            ++depth;
        } else if (c == '}') {
            --depth;
        } else if (c == ',' && depth == 0) {
            std::string part = trim(inner.substr(tokenStart, i - tokenStart));
            if (part.empty()) {
                throw std::invalid_argument("Empty element between commas in set string: " + str);
            }
            tokens.push_back(part);
            tokenStart = i + 1;
        }
    }

    std::string lastPart = trim(inner.substr(tokenStart));
    if (lastPart.empty()) {
        throw std::invalid_argument("Trailing comma or empty element in set string: " + str);
    }
    tokens.push_back(lastPart);

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const std::string& tok = tokens[i];
        if (tok.front() == '{') {
            CantorSet nested(tok);
            this->add(nested);
        } else {
            this->add(tok);
        }
    }
}

CantorSet CantorSet::fromString(const std::string& str) {
    return CantorSet(str);
}

CantorSet CantorSet::fromString(const char* str) {
    return CantorSet(str);
}

bool CantorSet::isValidSetString(const std::string& str) {
    try {
        CantorSet temp(str);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, const CantorSet& set) {
    os << set.toString();
    return os;
}

std::istream& operator>>(std::istream& is, CantorSet& set) {
    char ch = '\0';
    while (is.get(ch) && std::isspace(static_cast<unsigned char>(ch))) {
        // Пропускаем ведущие пробельные символы
    }
    if (!is) {
        return is;
    }
    if (ch != '{') {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::string buffer;
    buffer.push_back(ch);
    int depth = 1;
    while (depth > 0 && is.get(ch)) {
        buffer.push_back(ch);
        if (ch == '{') {
            ++depth;
        } else if (ch == '}') {
            --depth;
        }
    }

    if (depth != 0) {
        is.setstate(std::ios::failbit);
        return is;
    }

    try {
        CantorSet parsed(buffer);
        set = parsed;
    } catch (const std::invalid_argument&) {
        is.setstate(std::ios::failbit);
    }
    return is;
}
