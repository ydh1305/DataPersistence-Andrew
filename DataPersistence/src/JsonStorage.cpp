#include "JsonStorage.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iomanip>
#include <filesystem>

// ──────────────────────────────────────────────
// 파서 내부 헬퍼
// ──────────────────────────────────────────────

static size_t skipWs(const std::string& s, size_t pos) {
    while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos])))
        ++pos;
    return pos;
}

static std::string parseString(const std::string& s, size_t& pos) {
    if (pos >= s.size() || s[pos] != '"')
        throw std::runtime_error("JSON: '\"' 필요, pos=" + std::to_string(pos));
    ++pos;
    std::string result;
    while (pos < s.size() && s[pos] != '"') {
        if (s[pos] == '\\' && pos + 1 < s.size()) {
            ++pos;
            switch (s[pos]) {
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                case '/':  result += '/';  break;
                case 'b':  result += '\b'; break;
                case 'f':  result += '\f'; break;
                case 'n':  result += '\n'; break;
                case 'r':  result += '\r'; break;
                case 't':  result += '\t'; break;
                default:   result += s[pos]; break;
            }
        } else {
            result += s[pos];
        }
        ++pos;
    }
    if (pos >= s.size())
        throw std::runtime_error("JSON: 문자열 미종료");
    ++pos; // 닫는 '"' 소비
    return result;
}

static Item parseItem(const std::string& s, size_t& pos) {
    pos = skipWs(s, pos);
    if (pos >= s.size() || s[pos] != '{')
        throw std::runtime_error("JSON: '{' 필요, pos=" + std::to_string(pos));
    ++pos;

    Item item;
    bool first = true;

    while (true) {
        pos = skipWs(s, pos);
        if (pos >= s.size())
            throw std::runtime_error("JSON: 객체 미종료");
        if (s[pos] == '}') { ++pos; break; }

        if (!first) {
            if (s[pos] != ',')
                throw std::runtime_error("JSON: ',' 또는 '}' 필요, pos=" + std::to_string(pos));
            ++pos;
            pos = skipWs(s, pos);
        }
        first = false;

        std::string key = parseString(s, pos);
        pos = skipWs(s, pos);
        if (pos >= s.size() || s[pos] != ':')
            throw std::runtime_error("JSON: ':' 필요, pos=" + std::to_string(pos));
        ++pos;
        pos = skipWs(s, pos);

        if (pos < s.size() && s[pos] == '"') {
            std::string val = parseString(s, pos);
            if      (key == "name")        item.name        = val;
            else if (key == "description") item.description = val;
            else if (key == "createdAt")   item.createdAt   = val;
        } else {
            // 숫자 파싱
            size_t start = pos;
            if (pos < s.size() && s[pos] == '-') ++pos;
            while (pos < s.size() &&
                   (std::isdigit(static_cast<unsigned char>(s[pos])) ||
                    s[pos] == '.' || s[pos] == 'e' || s[pos] == 'E' ||
                    s[pos] == '+' || s[pos] == '-')) {
                ++pos;
            }
            if (start == pos)
                throw std::runtime_error("JSON: 값 파싱 실패, pos=" + std::to_string(pos));
            std::string num = s.substr(start, pos - start);
            if      (key == "id")    item.id    = std::stoi(num);
            else if (key == "price") item.price = std::stod(num);
        }
    }
    return item;
}

// ──────────────────────────────────────────────
// 공개 인터페이스
// ──────────────────────────────────────────────

std::vector<Item> JsonStorage::load(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return {};

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();
    if (content.empty()) return {};

    std::vector<Item> items;
    size_t pos = skipWs(content, 0);
    if (pos >= content.size() || content[pos] != '[') return {};
    ++pos;

    bool first = true;
    while (true) {
        pos = skipWs(content, pos);
        if (pos >= content.size() || content[pos] == ']') break;

        if (!first) {
            if (content[pos] != ',') break;
            ++pos;
            pos = skipWs(content, pos);
        }
        first = false;

        if (content[pos] != '{') break;
        try {
            items.push_back(parseItem(content, pos));
        } catch (const std::exception&) {
            break;
        }
    }
    return items;
}

std::string JsonStorage::escapeString(const std::string& s) {
    std::string result;
    result.reserve(s.size() + 4);
    for (char c : s) {
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n";  break;
            case '\r': result += "\\r";  break;
            case '\t': result += "\\t";  break;
            default:   result += c;      break;
        }
    }
    return result;
}

bool JsonStorage::save(const std::string& filePath, const std::vector<Item>& items) {
    std::filesystem::path p(filePath);
    if (p.has_parent_path()) {
        std::error_code ec;
        std::filesystem::create_directories(p.parent_path(), ec);
    }

    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    file << "[\n";
    for (size_t i = 0; i < items.size(); ++i) {
        const auto& item = items[i];

        // price는 classic locale 소수점 형식으로 직렬화
        std::ostringstream priceStr;
        priceStr.imbue(std::locale::classic());
        priceStr << std::fixed << std::setprecision(2) << item.price;

        file << "  {\n"
             << "    \"id\": "          << item.id                          << ",\n"
             << "    \"name\": \""      << escapeString(item.name)          << "\",\n"
             << "    \"description\": \""<< escapeString(item.description)  << "\",\n"
             << "    \"price\": "       << priceStr.str()                   << ",\n"
             << "    \"createdAt\": \"" << escapeString(item.createdAt)     << "\"\n"
             << "  }";
        if (i + 1 < items.size()) file << ",";
        file << "\n";
    }
    file << "]\n";
    return file.good();
}
