/**
 * @file PostRule.cpp
 * @author Student (Лабораторная работа №1, ППОИС)
 * @brief Реализация методов и операторов класса PostRule (Правило машины Поста).
 */

#include "PostRule.h"

#include <cctype>
#include <sstream>

PostRule::PostRule()
    : id_(1), action_(Action::Halt), next1_(-1), next2_(-1) {}

PostRule::PostRule(int id, Action action, int next1, int next2)
    : id_(id), action_(action), next1_(next1), next2_(next2) {
    if (this->action_ == Action::Halt) {
        this->next1_ = -1;
        this->next2_ = -1;
    } else if (this->action_ != Action::Branch) {
        this->next2_ = -1;
    }
    this->validate();
}

PostRule::PostRule(int id, char actionChar, int next1, int next2)
    : PostRule(id, charToAction(actionChar), next1, next2) {}

PostRule::PostRule(const PostRule& other)
    : id_(other.id_),
      action_(other.action_),
      next1_(other.next1_),
      next2_(other.next2_) {}

PostRule::~PostRule() = default;

PostRule& PostRule::operator=(const PostRule& other) {
    if (this == &other) {
        return *this;
    }
    this->id_ = other.id_;
    this->action_ = other.action_;
    this->next1_ = other.next1_;
    this->next2_ = other.next2_;
    return *this;
}

void PostRule::validate() const {
    if (this->id_ <= 0) {
        throw std::invalid_argument("Rule ID must be a positive integer");
    }
    if (this->action_ == Action::Halt) {
        return;
    }
    if (this->next1_ <= 0) {
        throw std::invalid_argument("Target rule next1 must be a positive integer");
    }
    if (this->action_ == Action::Branch && this->next2_ <= 0) {
        throw std::invalid_argument("Branch rule '?' requires a positive next2 target");
    }
}

PostRule::Action PostRule::charToAction(char ch) {
    switch (ch) {
        case 'V':
        case 'v':
        case '1':
            return Action::Mark;
        case 'X':
        case 'x':
        case '0':
            return Action::Erase;
        case '>':
        case 'R':
        case 'r':
            return Action::MoveRight;
        case '<':
        case 'L':
        case 'l':
            return Action::MoveLeft;
        case '?':
            return Action::Branch;
        case '!':
        case '.':
            return Action::Halt;
        default:
            throw std::invalid_argument(std::string("Unknown Post Machine action character: ") + ch);
    }
}

char PostRule::actionChar() const {
    return static_cast<char>(this->action_);
}

std::string PostRule::toString() const {
    std::ostringstream oss;
    oss << this->id_ << ": " << this->actionChar();
    if (this->action_ == Action::Branch) {
        oss << " " << this->next1_ << ", " << this->next2_;
    } else if (this->action_ != Action::Halt) {
        oss << " " << this->next1_;
    }
    return oss.str();
}

PostRule PostRule::fromString(const std::string& line) {
    std::string cleaned;
    for (std::size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '#' || (i + 1 < line.size() && line[i] == '/' && line[i + 1] == '/')) {
            break;
        }
        if (line[i] == ':' || line[i] == ',' || line[i] == ';') {
            cleaned.push_back(' ');
        } else {
            cleaned.push_back(line[i]);
        }
    }

    std::istringstream iss(cleaned);
    int id = 0;
    char actCh = '\0';
    if (!(iss >> id >> actCh)) {
        throw std::invalid_argument("Failed to parse rule ID and action from: " + line);
    }

    Action action = charToAction(actCh);
    int n1 = -1;
    int n2 = -1;

    if (action == Action::Branch) {
        if (!(iss >> n1 >> n2)) {
            throw std::invalid_argument("Branch rule '?' requires two target rule numbers: " + line);
        }
    } else if (action != Action::Halt) {
        if (!(iss >> n1)) {
            throw std::invalid_argument("Action rule requires a target rule number: " + line);
        }
    }

    std::string extra;
    if (iss >> extra) {
        throw std::invalid_argument("Unexpected extra tokens in rule string: " + line);
    }

    return PostRule(id, action, n1, n2);
}

bool PostRule::operator==(const PostRule& other) const {
    return this->id_ == other.id_ &&
           this->action_ == other.action_ &&
           this->next1_ == other.next1_ &&
           this->next2_ == other.next2_;
}

bool PostRule::operator!=(const PostRule& other) const {
    return !((*this) == other);
}

std::ostream& operator<<(std::ostream& os, const PostRule& rule) {
    os << rule.toString();
    return os;
}

std::istream& operator>>(std::istream& is, PostRule& rule) {
    std::string line;
    while (std::getline(is, line)) {
        bool hasContent = false;
        for (char c : line) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                hasContent = true;
                break;
            }
        }
        if (hasContent) {
            try {
                rule = PostRule::fromString(line);
            } catch (const std::invalid_argument&) {
                is.setstate(std::ios::failbit);
            }
            return is;
        }
    }
    is.setstate(std::ios::failbit);
    return is;
}
