#include <stdint.h>

#include <QString>
#include <QtCore>
#include <QFile>

#include "data/ScheduleData.hpp"
#include "data/PassengerData.hpp"
#include "data/RealtimeData.hpp"

/*
  todo:
    * create classes for each data type for loading, subset loading <potential for accelerated parseless loading>
    * create graphics context
    * plot route vs time

    ? date/time data structures ? map or other entries ?
  */

QString samplePath(const QString &basePath, bool sample) {
    return sample ? basePath + ".excerpt.csv" : basePath + ".csv";
}

int main(int argc, const char *argv[]) {
    QString directoryPath = "/home/emmett/Downloads/Urban-Data-Challenge-master/public-transportation/san-francisco/";
    bool sample = true;

    QVector<RealtimeEntry> realtimeData;
    readRealtimeData(directoryPath + samplePath("realtime-arrivals", sample), &realtimeData);

    QVector<PassengerEntry> passengerData;
    readPassengerData(directoryPath + samplePath("passenger-count", sample), &passengerData);

    QVector<ScheduleEntry> scheduleData;
    readScheduleData(directoryPath + samplePath("scheduled-arrivals", sample), &scheduleData);

    return 0;
}
