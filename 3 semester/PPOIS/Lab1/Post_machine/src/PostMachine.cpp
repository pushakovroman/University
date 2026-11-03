/**
 * @file PostMachine.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Реализация методов и операторов класса PostMachine (Абстрактная машина Поста).
 */

#include "PostMachine.h"

#include <cctype>
#include <fstream>
#include <sstream>

namespace {

std::string trimString(const std::string& s) {
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

} // namespace

PostMachine::PostMachine()
    : tape_(),
      program_(),
      currentRuleId_(-1),
      stepCount_(0),
      halted_(false) {}

PostMachine::PostMachine(const PostTape& tape, const PostProgram& program)
    : tape_(tape),
      program_(program),
      currentRuleId_(program.firstRuleId()),
      stepCount_(0),
      halted_(false) {}

PostMachine::PostMachine(const PostMachine& other)
    : tape_(other.tape_),
      program_(other.program_),
      currentRuleId_(other.currentRuleId_),
      stepCount_(other.stepCount_),
      halted_(other.halted_) {}

PostMachine::~PostMachine() = default;

PostMachine& PostMachine::operator=(const PostMachine& other) {
    if (this == &other) {
        return *this;
    }
    this->tape_ = other.tape_;
    this->program_ = other.program_;
    this->currentRuleId_ = other.currentRuleId_;
    this->stepCount_ = other.stepCount_;
    this->halted_ = other.halted_;
    return *this;
}

void PostMachine::setCurrentRuleId(int ruleId) {
    this->currentRuleId_ = ruleId;
    this->halted_ = false;
}

void PostMachine::resetExecution() {
    this->currentRuleId_ = this->program_.firstRuleId();
    this->stepCount_ = 0;
    this->halted_ = false;
}

bool PostMachine::step() {
    if (this->halted_) {
        return false;
    }
    if (this->program_.isEmpty()) {
        throw std::runtime_error("Cannot execute step: PostMachine program is empty");
    }
    if (this->currentRuleId_ <= 0) {
        this->currentRuleId_ = this->program_.firstRuleId();
    }
    if (!this->program_.hasRule(this->currentRuleId_)) {
        throw std::runtime_error("Rule ID " + std::to_string(this->currentRuleId_) +
                                 " does not exist in PostProgram");
    }

    const PostRule& rule = this->program_.getRule(this->currentRuleId_);
    ++this->stepCount_;

    switch (rule.action()) {
        case PostRule::Action::Mark:
            this->tape_.mark();
            this->currentRuleId_ = rule.next1();
            return true;

        case PostRule::Action::Erase:
            this->tape_.erase();
            this->currentRuleId_ = rule.next1();
            return true;

        case PostRule::Action::MoveRight:
            ++this->tape_; // используем перегруженный оператор инкремента каретки
            this->currentRuleId_ = rule.next1();
            return true;

        case PostRule::Action::MoveLeft:
            --this->tape_; // используем перегруженный оператор декремента каретки
            this->currentRuleId_ = rule.next1();
            return true;

        case PostRule::Action::Branch:
            if (this->tape_.isMarked()) {
                this->currentRuleId_ = rule.next2();
            } else {
                this->currentRuleId_ = rule.next1();
            }
            return true;

        case PostRule::Action::Halt:
            this->halted_ = true;
            return false;
    }
    return false;
}

PostMachine& PostMachine::operator++() {
    this->step();
    return *this;
}

PostMachine PostMachine::operator++(int) {
    PostMachine copy(*this);
    this->step();
    return copy;
}

std::size_t PostMachine::run(bool logSteps, std::ostream& logStream, std::size_t maxSteps) {
    if (this->currentRuleId_ <= 0 && !this->program_.isEmpty()) {
        this->currentRuleId_ = this->program_.firstRuleId();
    }

    std::size_t executedInRun = 0;
    while (!this->halted_) {
        if (executedInRun >= maxSteps) {
            throw std::runtime_error("Execution exceeded maximum step limit (" +
                                     std::to_string(maxSteps) + ")");
        }
        int executedRuleId = this->currentRuleId_;
        this->step();
        ++executedInRun;

        if (logSteps) {
            logStream << "[Шаг " << this->stepCount_
                      << " | Выполнено правило #" << executedRuleId << "] "
                      << this->formatState() << "\n";
        }
    }
    return executedInRun;
}

std::istream& PostMachine::loadFromStream(std::istream& is) {
    this->tape_.clear();
    this->program_.clear();
    this->stepCount_ = 0;
    this->halted_ = false;
    this->currentRuleId_ = -1;

    bool tapeLoaded = false;
    std::string line;

    while (std::getline(is, line)) {
        // Отрезаем комментарии (# или //)
        for (std::size_t i = 0; i < line.size(); ++i) {
            if (line[i] == '#' || (i + 1 < line.size() && line[i] == '/' && line[i + 1] == '/')) {
                line = line.substr(0, i);
                break;
            }
        }
        std::string trimmed = trimString(line);
        if (trimmed.empty()) {
            continue;
        }

        if (trimmed.rfind("TAPE:", 0) == 0 || trimmed.rfind("tape:", 0) == 0) {
            std::string tapeVal = trimString(trimmed.substr(5));
            this->tape_.loadFromString(tapeVal);
            tapeLoaded = true;
            continue;
        }

        if (trimmed.rfind("CARRIAGE:", 0) == 0 || trimmed.rfind("carriage:", 0) == 0) {
            std::string posStr = trimString(trimmed.substr(9));
            this->tape_.setCarriagePosition(std::stoi(posStr));
            continue;
        }

        // Если лента ещё не загружена и строка состоит только из 0, 1, ., [, ]
        if (!tapeLoaded) {
            bool looksLikeTape = true;
            for (char c : trimmed) {
                if (c != '0' && c != '1' && c != '.' && c != '[' && c != ']') {
                    looksLikeTape = false;
                    break;
                }
            }
            if (looksLikeTape) {
                this->tape_.loadFromString(trimmed);
                tapeLoaded = true;
                continue;
            }
        }

        this->program_.addRule(PostRule::fromString(trimmed));
    }

    if (is.eof() && !is.bad()) {
        is.clear(std::ios::eofbit);
    }

    this->currentRuleId_ = this->program_.firstRuleId();
    return is;
}

void PostMachine::loadFromFile(const std::string& filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open Post Machine input file: " + filePath);
    }
    this->loadFromStream(ifs);
}

std::string PostMachine::formatState() const {
    std::ostringstream oss;
    oss << "Каретка: " << this->tape_.carriagePosition()
        << " | След. правило: " << (this->halted_ ? std::string("HALT (!)") : std::to_string(this->currentRuleId_))
        << " | Шагов: " << this->stepCount_
        << " | Лента: " << this->tape_.toString();
    return oss.str();
}

bool PostMachine::operator==(const PostMachine& other) const {
    return this->tape_ == other.tape_ &&
           this->program_ == other.program_ &&
           this->currentRuleId_ == other.currentRuleId_ &&
           this->stepCount_ == other.stepCount_ &&
           this->halted_ == other.halted_;
}

bool PostMachine::operator!=(const PostMachine& other) const {
    return !((*this) == other);
}

std::ostream& operator<<(std::ostream& os, const PostMachine& machine) {
    os << "TAPE: " << machine.tape_.toString() << "\n";
    os << "CARRIAGE: " << machine.tape_.carriagePosition() << "\n";
    os << machine.program_.toString();
    return os;
}

std::istream& operator>>(std::istream& is, PostMachine& machine) {
    try {
        machine.loadFromStream(is);
    } catch (const std::exception&) {
        is.setstate(std::ios::failbit);
    }
    return is;
}
