#include <stdint.h>

#include <QString>
#include <QtCore>
#include <QFile>

#include "data/RawContestData.hpp"

#include "vis/Plot.hpp"

#include "vis-proto/RawProto.hpp"
#include "vis-proto/Simple.hpp"

#include <QtCore>
#include <QtGui>

/*
  todo:
    * create classes for each data type for loading, subset loading <potential for accelerated parseless loading>
    * create graphics context
    * plot route vs time

    ? map or other entries ?
  */

QString samplePath(const QString &basePath, bool sample) {
    return sample ? basePath + ".excerpt.csv" : basePath + ".csv";
}

int main(int argc, char *argv[]) {
    QString directoryPath = "/home/emmett/Downloads/Urban-Data-Challenge-master/public-transportation/san-francisco/";
    bool sample = true;

    RawContestData contestData;
    readRealtimeData(directoryPath + samplePath("realtime-arrivals", sample), &contestData.realtimeData);

    QVector<PassengerEntry> passengerData;
    readPassengerData(directoryPath + samplePath("passenger-count", sample), &contestData.passengerData);

    QVector<ScheduleEntry> scheduleData;
    readScheduleData(directoryPath + samplePath("scheduled-arrivals", sample), &contestData.scheduleData);

    QApplication app(argc, argv);
    plot(contestData.realtimeData);
    plot(contestData.scheduleData);
    plot(contestData.passengerData);

    QSharedPointer<RawProto> proto(new Simple());
    proto->accept(contestData);

    return 0;
}
