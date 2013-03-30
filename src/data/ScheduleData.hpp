#pragma once

#include <QString>
#include <QVector>

struct ScheduleEntry {

}; // struct ScheduleEntry

void readScheduleData(const QString &filePath, QVector<ScheduleEntry> *entries);
