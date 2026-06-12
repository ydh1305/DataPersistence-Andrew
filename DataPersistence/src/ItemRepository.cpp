#include "ItemRepository.h"
#include "JsonStorage.h"
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>

ItemRepository::ItemRepository(const std::string& filePath)
    : filePath_(filePath) {
    items_ = JsonStorage::load(filePath_);
}

int ItemRepository::nextId() const {
    int maxId = 0;
    for (const auto& item : items_)
        if (item.id > maxId) maxId = item.id;
    return maxId + 1;
}

std::string ItemRepository::currentTimestamp() {
    auto now  = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &time);
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void ItemRepository::persist() const {
    JsonStorage::save(filePath_, items_);
}

// ── Create ────────────────────────────────────

Item ItemRepository::create(const std::string& name,
                             const std::string& description,
                             double price) {
    Item item;
    item.id          = nextId();
    item.name        = name;
    item.description = description;
    item.price       = price;
    item.createdAt   = currentTimestamp();
    items_.push_back(item);
    persist();
    return item;
}

// ── Read ──────────────────────────────────────

std::vector<Item> ItemRepository::findAll() const {
    return items_;
}

std::optional<Item> ItemRepository::findById(int id) const {
    for (const auto& item : items_)
        if (item.id == id) return item;
    return std::nullopt;
}

int ItemRepository::count() const {
    return static_cast<int>(items_.size());
}

// ── Update ────────────────────────────────────

bool ItemRepository::update(int id,
                             const std::string& name,
                             const std::string& description,
                             double price) {
    for (auto& item : items_) {
        if (item.id == id) {
            item.name        = name;
            item.description = description;
            item.price       = price;
            persist();
            return true;
        }
    }
    return false;
}

// ── Delete ────────────────────────────────────

bool ItemRepository::remove(int id) {
    auto it = std::remove_if(items_.begin(), items_.end(),
        [id](const Item& item) { return item.id == id; });
    if (it == items_.end()) return false;
    items_.erase(it, items_.end());
    persist();
    return true;
}
