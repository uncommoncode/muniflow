#pragma once

#include <stdint.h>

#include <QString>
#include <QDateTime>
#include <QVector>

#include "GeoLocation.hpp"
#include "Time.hpp"

struct RealtimeEntry {
    QString name;
    QString vehicleAssignment;
    uint32_t tripId;
    QString blockName;
    bool doubleCar;
    uint32_t carId;
    uint32_t stopId;
    GeoLocation position;
    Time arrivalTime;
}; // struct RealtimeEntry

void readRealtimeData(const QString& filePath, QVector<RealtimeEntry> *entries);
