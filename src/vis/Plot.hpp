#pragma once

#include <data/ScheduleData.hpp>
#include <data/PassengerData.hpp>
#include <data/RealtimeData.hpp>

void plot(const QVector<RealtimeEntry> &entries);
void plot(const QVector<PassengerEntry> &entries);
void plot(const QVector<ScheduleEntry> &entries);
