#pragma once
#include <string>

struct Item {
    int         id          = 0;
    std::string name;
    std::string description;
    double      price       = 0.0;
    std::string createdAt;
};
