#pragma once

#include <QString>
#include <QVector>

struct PassengerEntry {

}; // struct PassengerEntry

void readPassengerData(const QString &filePath, QVector<PassengerEntry> *entries);
