#pragma once

#include <QString>
#include <QVector>

#include "GeoLocation.hpp"
#include "Time.hpp"
#include <stdint.h>

// STOP_SEQ,STOP_ID,STOP_NAME,ON,OFF,LOAD,MO,DAY,YR,ROUTE,LATITUDE,LONGITUDE,TRIP_ID,DIR,VEHNO,TIMESTOP,TIMEDOORCLOSE,TIMEPULLOUT,TRIPCODE,TRIPSTOP,DOORDWELL,WAITDWELL
// 31,6306, SACRAMENTO ST&LAGUNA ST SW-N,0,0,0,10,4,12,1,37.78761,122.42892,758,1,2801,8:14:22,8:14:22,8:14:22,1001.1 _ 2801_4684 _  75, 1031_ SACRAMENTO ST&LAGUNA ST SW-N,0:00:00,0:00:00
struct PassengerEntry {
    uint32_t stopSequence;
    uint32_t stopId;
    QString stopName;
    uint32_t peopleOn;
    uint32_t peopleOff;
    uint32_t load;
    Time time;
    QString route;
    GeoLocation location;
    uint32_t tripId;
    // direction
    uint32_t vehicleNumber;
    // TIMESTOP,TIMEDOORCLOSE,TIMEPULLOUT,TRIPCODE,TRIPSTOP,DOORDWELL,WAITDWELL
}; // struct PassengerEntry

void readPassengerData(const QString &filePath, QVector<PassengerEntry> *entries);
