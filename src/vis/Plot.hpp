#pragma once

#include <data/ScheduleData.hpp>
#include <data/PassengerData.hpp>
#include <data/RealtimeData.hpp>

#include <data/VisData.hpp>

void plot(const VisData &visData, const QVector<RealtimeEntry> &entries);
void plot(const VisData &visData, const QVector<PassengerEntry> &entries);
void plot(const VisData &visData, const QVector<ScheduleEntry> &entries);
