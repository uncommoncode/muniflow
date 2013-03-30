#pragma once

#include <data/GeoLocation.hpp>

struct VisData {
    enum Type {
        Type_SanFrancisco
    }; // enum Type
    GeoArea area;
}; // struct VisData

void readVisData(enum VisData::Type type, VisData *data);
