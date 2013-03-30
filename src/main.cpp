#include <stdint.h>

#include <QString>
#include <QtCore>
#include <QFile>

#include "data/RawContestData.hpp"
#include "data/VisData.hpp"

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
    bool sample = false;

    RawContestData contestData;
    readRealtimeData(directoryPath + samplePath("realtime-arrivals", sample), &contestData.realtimeData);
#if defined(PLOT_ALL)
    QVector<PassengerEntry> passengerData;
    readPassengerData(directoryPath + samplePath("passenger-count", sample), &contestData.passengerData);

    QVector<ScheduleEntry> scheduleData;
    readScheduleData(directoryPath + samplePath("scheduled-arrivals", sample), &contestData.scheduleData);
#endif

    VisData visData;
    readVisData(VisData::Type_SanFrancisco, &visData);

    QApplication app(argc, argv);
    plot(visData, contestData.realtimeData);
#if defined(PLOT_ALL)
    plot(visData, contestData.scheduleData);
    plot(visData, contestData.passengerData);

    QSharedPointer<RawProto> proto(new Simple());
    proto->accept(contestData);
#endif

    return 0;
}
