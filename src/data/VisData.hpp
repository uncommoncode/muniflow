#pragma once

#include <data/GeoLocation.hpp>

#include <stdint.h>

struct VisData {
    enum Type {
        Type_SanFrancisco
    }; // enum Type
    GeoArea area;
    uint64_t t0;
}; // struct VisData

void readVisData(enum VisData::Type type, VisData *data);
