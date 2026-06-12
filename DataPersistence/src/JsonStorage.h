#pragma once
#include <string>
#include <vector>
#include "Item.h"

// JSON 파일 기반 I/O — 외부 라이브러리 없이 자체 파서/직렬화 구현
class JsonStorage {
public:
    static std::vector<Item> load(const std::string& filePath);
    static bool              save(const std::string& filePath, const std::vector<Item>& items);

private:
    static std::string escapeString(const std::string& s);
};
