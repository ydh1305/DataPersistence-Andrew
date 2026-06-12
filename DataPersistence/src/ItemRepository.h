#pragma once
#include <string>
#include <vector>
#include <optional>
#include "Item.h"

class ItemRepository {
public:
    explicit ItemRepository(const std::string& filePath);

    // Create
    Item create(const std::string& name, const std::string& description, double price);

    // Read
    std::vector<Item>    findAll()      const;
    std::optional<Item>  findById(int id) const;
    int                  count()        const;

    // Update
    bool update(int id, const std::string& name, const std::string& description, double price);

    // Delete
    bool remove(int id);

private:
    std::string        filePath_;
    std::vector<Item>  items_;

    void        persist() const;
    int         nextId()  const;
    static std::string currentTimestamp();
};
