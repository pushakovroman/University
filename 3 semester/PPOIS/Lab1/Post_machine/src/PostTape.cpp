/**
 * @file PostTape.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Реализация методов и операторов класса PostTape (Лента и каретка машины Поста).
 */

#include "PostTape.h"

#include <algorithm>
#include <cctype>
#include <sstream>

PostTape::PostTape()
    : cells_(new bool[1]{false}),
      minIndex_(0),
      capacity_(1),
      carriage_(0),
      cachedRepr_("[0]"),
      isCacheValid_(true) {}

PostTape::PostTape(const std::string& stateStr, int initialCarriage)
    : cells_(nullptr),
      minIndex_(0),
      capacity_(0),
      carriage_(0),
      cachedRepr_(""),
      isCacheValid_(false) {
    this->loadFromString(stateStr, initialCarriage);
}

PostTape::PostTape(const char* stateStr, int initialCarriage)
    : cells_(nullptr),
      minIndex_(0),
      capacity_(0),
      carriage_(0),
      cachedRepr_(""),
      isCacheValid_(false) {
    if (stateStr == nullptr) {
        throw std::invalid_argument("Null pointer passed to PostTape(const char*)");
    }
    this->loadFromString(std::string(stateStr), initialCarriage);
}

PostTape::PostTape(const PostTape& other)
    : cells_(nullptr),
      minIndex_(other.minIndex_),
      capacity_(other.capacity_),
      carriage_(other.carriage_),
      cachedRepr_(other.cachedRepr_),
      isCacheValid_(other.isCacheValid_) {
    if (other.capacity_ > 0) {
        this->cells_ = new bool[other.capacity_];
        for (std::size_t i = 0; i < other.capacity_; ++i) {
            this->cells_[i] = other.cells_[i];
        }
    }
}

PostTape::~PostTape() {
    delete[] this->cells_;
    this->cells_ = nullptr;
    this->capacity_ = 0;
}

PostTape& PostTape::operator=(const PostTape& other) {
    if (this == &other) {
        return *this;
    }
    bool* newCells = nullptr;
    if (other.capacity_ > 0) {
        newCells = new bool[other.capacity_];
        for (std::size_t i = 0; i < other.capacity_; ++i) {
            newCells[i] = other.cells_[i];
        }
    }
    delete[] this->cells_;
    this->cells_ = newCells;
    this->minIndex_ = other.minIndex_;
    this->capacity_ = other.capacity_;
    this->carriage_ = other.carriage_;
    this->cachedRepr_ = other.cachedRepr_;
    this->isCacheValid_ = other.isCacheValid_;
    return *this;
}

void PostTape::invalidateCache() const {
    this->isCacheValid_ = false;
    this->cachedRepr_.clear();
}

void PostTape::ensureRange(int index) {
    if (this->capacity_ == 0) {
        this->minIndex_ = index;
        this->capacity_ = 1;
        this->cells_ = new bool[1]{false};
        return;
    }

    int maxIndex = this->minIndex_ + static_cast<int>(this->capacity_) - 1;
    if (index >= this->minIndex_ && index <= maxIndex) {
        return;
    }

    int newMin = std::min(this->minIndex_, index - 4);
    int newMax = std::max(maxIndex, index + 4);
    std::size_t newCap = static_cast<std::size_t>(newMax - newMin + 1);

    bool* newCells = new bool[newCap];
    for (std::size_t i = 0; i < newCap; ++i) {
        newCells[i] = false;
    }

    int offset = this->minIndex_ - newMin;
    for (std::size_t i = 0; i < this->capacity_; ++i) {
        newCells[static_cast<std::size_t>(offset) + i] = this->cells_[i];
    }

    delete[] this->cells_;
    this->cells_ = newCells;
    this->minIndex_ = newMin;
    this->capacity_ = newCap;
}

void PostTape::setCarriagePosition(int position) {
    this->ensureRange(position);
    this->carriage_ = position;
    this->invalidateCache();
}

bool PostTape::isMarked() const {
    return this->getCell(this->carriage_);
}

void PostTape::mark() {
    this->setCell(this->carriage_, true);
}

void PostTape::erase() {
    this->setCell(this->carriage_, false);
}

void PostTape::setCell(int index, bool value) {
    this->ensureRange(index);
    this->cells_[static_cast<std::size_t>(index - this->minIndex_)] = value;
    this->invalidateCache();
}

bool PostTape::getCell(int index) const {
    if (this->capacity_ == 0) {
        return false;
    }
    int maxIndex = this->minIndex_ + static_cast<int>(this->capacity_) - 1;
    if (index < this->minIndex_ || index > maxIndex) {
        return false;
    }
    return this->cells_[static_cast<std::size_t>(index - this->minIndex_)];
}

bool& PostTape::operator[](int index) {
    this->ensureRange(index);
    this->invalidateCache();
    return this->cells_[static_cast<std::size_t>(index - this->minIndex_)];
}

bool PostTape::operator[](int index) const {
    return this->getCell(index);
}

void PostTape::moveRight() {
    ++this->carriage_;
    this->ensureRange(this->carriage_);
    this->invalidateCache();
}

void PostTape::moveLeft() {
    --this->carriage_;
    this->ensureRange(this->carriage_);
    this->invalidateCache();
}

PostTape& PostTape::operator++() {
    this->moveRight();
    return *this;
}

PostTape PostTape::operator++(int) {
    PostTape prev(*this);
    this->moveRight();
    return prev;
}

PostTape& PostTape::operator--() {
    this->moveLeft();
    return *this;
}

PostTape PostTape::operator--(int) {
    PostTape prev(*this);
    this->moveLeft();
    return prev;
}

void PostTape::clear() {
    delete[] this->cells_;
    this->cells_ = new bool[1]{false};
    this->minIndex_ = 0;
    this->capacity_ = 1;
    this->carriage_ = 0;
    this->cachedRepr_ = "[0]";
    this->isCacheValid_ = true;
}

std::size_t PostTape::countMarks() const {
    std::size_t count = 0;
    for (std::size_t i = 0; i < this->capacity_; ++i) {
        if (this->cells_[i]) {
            ++count;
        }
    }
    return count;
}

void PostTape::loadFromString(const std::string& stateStr, int initialCarriage) {
    this->clear();
    int cellPos = 0;
    bool carriageFoundInBrackets = false;
    int bracketCarriagePos = 0;
    bool inBracket = false;

    for (std::size_t i = 0; i < stateStr.size(); ++i) {
        char c = stateStr[i];
        if (std::isspace(static_cast<unsigned char>(c))) {
            continue;
        }
        if (c == '[') {
            if (inBracket || carriageFoundInBrackets) {
                throw std::invalid_argument("Invalid or duplicate '[' in tape string: " + stateStr);
            }
            inBracket = true;
            bracketCarriagePos = cellPos;
            carriageFoundInBrackets = true;
        } else if (c == ']') {
            if (!inBracket || cellPos != bracketCarriagePos + 1) {
                throw std::invalid_argument("Invalid ']' placement in tape string: " + stateStr);
            }
            inBracket = false;
        } else if (c == '1') {
            this->setCell(cellPos, true);
            ++cellPos;
        } else if (c == '0' || c == '.') {
            this->setCell(cellPos, false);
            ++cellPos;
        } else {
            throw std::invalid_argument(std::string("Invalid character in tape string: ") + c);
        }
    }

    if (inBracket) {
        throw std::invalid_argument("Unclosed '[' in tape string: " + stateStr);
    }

    this->setCarriagePosition(carriageFoundInBrackets ? bracketCarriagePos : initialCarriage);
}

std::istream& PostTape::loadFromStream(std::istream& is) {
    std::string token;
    if (!(is >> token)) {
        return is;
    }
    try {
        this->loadFromString(token);
    } catch (const std::invalid_argument&) {
        is.setstate(std::ios::failbit);
    }
    return is;
}

const std::string& PostTape::toString() const {
    if (!this->isCacheValid_) {
        int leftBound = this->carriage_;
        int rightBound = this->carriage_;

        for (std::size_t i = 0; i < this->capacity_; ++i) {
            if (this->cells_[i]) {
                int idx = this->minIndex_ + static_cast<int>(i);
                if (idx < leftBound) leftBound = idx;
                if (idx > rightBound) rightBound = idx;
            }
        }
        if (0 < leftBound) leftBound = 0;

        std::ostringstream oss;
        for (int idx = leftBound; idx <= rightBound; ++idx) {
            char symbol = this->getCell(idx) ? '1' : '0';
            if (idx == this->carriage_) {
                oss << '[' << symbol << ']';
            } else {
                oss << symbol;
            }
        }
        this->cachedRepr_ = oss.str();
        this->isCacheValid_ = true;
    }
    return this->cachedRepr_;
}

std::string PostTape::toPlainString() const {
    int leftBound = this->carriage_;
    int rightBound = this->carriage_;
    for (std::size_t i = 0; i < this->capacity_; ++i) {
        if (this->cells_[i]) {
            int idx = this->minIndex_ + static_cast<int>(i);
            if (idx < leftBound) leftBound = idx;
            if (idx > rightBound) rightBound = idx;
        }
    }
    if (0 < leftBound) leftBound = 0;

    std::string out;
    for (int idx = leftBound; idx <= rightBound; ++idx) {
        out.push_back(this->getCell(idx) ? '1' : '0');
    }
    return out;
}

bool PostTape::operator==(const PostTape& other) const {
    if (this == &other) {
        return true;
    }
    if (this->carriage_ != other.carriage_) {
        return false;
    }
    if (this->countMarks() != other.countMarks()) {
        return false;
    }
    for (std::size_t i = 0; i < this->capacity_; ++i) {
        if (this->cells_[i]) {
            int idx = this->minIndex_ + static_cast<int>(i);
            if (!other.getCell(idx)) {
                return false;
            }
        }
    }
    return true;
}

bool PostTape::operator!=(const PostTape& other) const {
    return !((*this) == other);
}

std::ostream& operator<<(std::ostream& os, const PostTape& tape) {
    os << tape.toString();
    return os;
}

std::istream& operator>>(std::istream& is, PostTape& tape) {
    return tape.loadFromStream(is);
}
