#pragma once

#include "ScheduleData.hpp"
#include "PassengerData.hpp"
#include "RealtimeData.hpp"

struct RawContestData {
    QVector<RealtimeEntry> realtimeData;
    QVector<PassengerEntry> passengerData;
    QVector<ScheduleEntry> scheduleData;
}; // struct RawContestData
