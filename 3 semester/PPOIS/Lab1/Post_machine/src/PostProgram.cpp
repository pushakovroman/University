/**
 * @file PostProgram.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Реализация методов и операторов класса PostProgram (Программа машины Поста).
 */

#include "PostProgram.h"

#include <cctype>
#include <sstream>

PostProgram::PostProgram()
    : rules_(nullptr),
      size_(0),
      capacity_(0),
      cachedText_(""),
      isCacheValid_(true) {}

PostProgram::PostProgram(const PostProgram& other)
    : rules_(nullptr),
      size_(other.size_),
      capacity_(other.size_),
      cachedText_(other.cachedText_),
      isCacheValid_(other.isCacheValid_) {
    if (other.size_ > 0) {
        this->rules_ = new PostRule[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            this->rules_[i] = other.rules_[i];
        }
    }
}

PostProgram::~PostProgram() {
    delete[] this->rules_;
    this->rules_ = nullptr;
    this->size_ = 0;
    this->capacity_ = 0;
}

PostProgram& PostProgram::operator=(const PostProgram& other) {
    if (this == &other) {
        return *this;
    }
    PostRule* newRules = nullptr;
    if (other.size_ > 0) {
        newRules = new PostRule[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            newRules[i] = other.rules_[i];
        }
    }
    delete[] this->rules_;
    this->rules_ = newRules;
    this->size_ = other.size_;
    this->capacity_ = other.size_;
    this->cachedText_ = other.cachedText_;
    this->isCacheValid_ = other.isCacheValid_;
    return *this;
}

void PostProgram::invalidateCache() const {
    this->isCacheValid_ = false;
    this->cachedText_.clear();
}

void PostProgram::ensureCapacity(std::size_t minCapacity) {
    if (this->capacity_ >= minCapacity) {
        return;
    }
    std::size_t newCap = (this->capacity_ == 0) ? 4 : this->capacity_ * 2;
    if (newCap < minCapacity) {
        newCap = minCapacity;
    }
    PostRule* newRules = new PostRule[newCap];
    for (std::size_t i = 0; i < this->size_; ++i) {
        newRules[i] = this->rules_[i];
    }
    delete[] this->rules_;
    this->rules_ = newRules;
    this->capacity_ = newCap;
}

void PostProgram::clear() {
    delete[] this->rules_;
    this->rules_ = nullptr;
    this->size_ = 0;
    this->capacity_ = 0;
    this->cachedText_.clear();
    this->isCacheValid_ = true;
}

void PostProgram::addRule(const PostRule& rule) {
    // Если правило с таким id уже есть — заменяем его
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->rules_[i].id() == rule.id()) {
            this->rules_[i] = rule;
            this->invalidateCache();
            return;
        }
    }

    // Вставляем с сохранением сортировки по возрастанию id
    this->ensureCapacity(this->size_ + 1);
    std::size_t insertPos = this->size_;
    while (insertPos > 0 && this->rules_[insertPos - 1].id() > rule.id()) {
        this->rules_[insertPos] = this->rules_[insertPos - 1];
        --insertPos;
    }
    this->rules_[insertPos] = rule;
    ++this->size_;
    this->invalidateCache();
}

void PostProgram::addRule(const std::string& ruleStr) {
    this->addRule(PostRule::fromString(ruleStr));
}

PostProgram& PostProgram::operator+=(const PostRule& rule) {
    this->addRule(rule);
    return *this;
}

bool PostProgram::removeRule(int ruleId) {
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->rules_[i].id() == ruleId) {
            for (std::size_t j = i; j + 1 < this->size_; ++j) {
                this->rules_[j] = this->rules_[j + 1];
            }
            --this->size_;
            this->invalidateCache();
            return true;
        }
    }
    return false;
}

PostProgram& PostProgram::operator-=(int ruleId) {
    this->removeRule(ruleId);
    return *this;
}

bool PostProgram::hasRule(int ruleId) const {
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->rules_[i].id() == ruleId) {
            return true;
        }
    }
    return false;
}

const PostRule& PostProgram::getRule(int ruleId) const {
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->rules_[i].id() == ruleId) {
            return this->rules_[i];
        }
    }
    throw std::out_of_range("Rule ID " + std::to_string(ruleId) + " not found in PostProgram");
}

const PostRule& PostProgram::operator[](int ruleId) const {
    return this->getRule(ruleId);
}

const PostRule& PostProgram::ruleAtIndex(std::size_t index) const {
    if (index >= this->size_) {
        throw std::out_of_range("Rule index out of range in PostProgram::ruleAtIndex");
    }
    return this->rules_[index];
}

int PostProgram::firstRuleId() const {
    if (this->size_ == 0) {
        return -1;
    }
    return this->rules_[0].id();
}

std::istream& PostProgram::loadFromStream(std::istream& is) {
    this->clear();
    std::string line;
    while (std::getline(is, line)) {
        // Убираем комментарии
        std::string stripped = line;
        for (std::size_t i = 0; i < stripped.size(); ++i) {
            if (stripped[i] == '#' ||
                (i + 1 < stripped.size() && stripped[i] == '/' && stripped[i + 1] == '/')) {
                stripped = stripped.substr(0, i);
                break;
            }
        }
        bool hasNonSpace = false;
        for (char c : stripped) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                hasNonSpace = true;
                break;
            }
        }
        if (!hasNonSpace) {
            continue;
        }
        this->addRule(PostRule::fromString(stripped));
    }
    if (is.eof() && !is.bad()) {
        is.clear(std::ios::eofbit);
    }
    return is;
}

const std::string& PostProgram::toString() const {
    if (!this->isCacheValid_) {
        std::ostringstream oss;
        for (std::size_t i = 0; i < this->size_; ++i) {
            if (i > 0) {
                oss << "\n";
            }
            oss << this->rules_[i].toString();
        }
        this->cachedText_ = oss.str();
        this->isCacheValid_ = true;
    }
    return this->cachedText_;
}

bool PostProgram::operator==(const PostProgram& other) const {
    if (this == &other) {
        return true;
    }
    if (this->size_ != other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < this->size_; ++i) {
        if (this->rules_[i] != other.rules_[i]) {
            return false;
        }
    }
    return true;
}

bool PostProgram::operator!=(const PostProgram& other) const {
    return !((*this) == other);
}

std::ostream& operator<<(std::ostream& os, const PostProgram& program) {
    os << program.toString();
    return os;
}

std::istream& operator>>(std::istream& is, PostProgram& program) {
    try {
        program.loadFromStream(is);
    } catch (const std::invalid_argument&) {
        is.setstate(std::ios::failbit);
    }
    return is;
}
