#include <stdint.h>

#include <QString>
#include <QtCore>
#include <QFile>

#include "data/RawContestData.hpp"
#include "data/VisData.hpp"

#include "vis/Plot.hpp"
#include "vis/VisWindow.hpp"

#include "vis-proto/Simple.hpp"
#include "vis-proto/Particle.hpp"
#include "vis-proto/DLCycle.hpp"

#include <QtCore>
#include <QtGui>

#include <stdio.h>

/*
  todo:
    * create classes for each data type for loading, subset loading <potential for accelerated parseless loading>
    * create graphics context
    * plot route vs time

    ? map or other entries ?
  */

bool realtimeArrivalCompare(const RealtimeEntry &a, const RealtimeEntry &b) {
    return a.arrivalTime.ms < b.arrivalTime.ms;
}

bool passengerDepartureCompare(const PassengerEntry &a, const PassengerEntry &b) {
    return a.time.ms < b.time.ms;
}

QString samplePath(const QString &basePath, bool sample) {
    return sample ? basePath + ".excerpt.csv" : basePath + ".csv";
}

int main(int argc, char *argv[]) {
    QString directoryPath = "/home/emmett/Downloads/Urban-Data-Challenge-master/public-transportation/san-francisco/";
    QString imageDirectoryPath = "/home/emmett/code/udcproto/data/shapes/";
    bool sample = false;

    RawContestData contestData;
#if defined(USE_REALTIME_DATA)
    readRealtimeData(directoryPath + samplePath("realtime-arrivals", sample), &contestData.realtimeData);
    qSort(contestData.realtimeData.begin(), contestData.realtimeData.end(), realtimeArrivalCompare);
#endif

#ifndef USE_REALTIME_DATA
    readPassengerData(directoryPath + samplePath("passenger-count", sample), &contestData.passengerData);
    qSort(contestData.passengerData.begin(), contestData.passengerData.end(), passengerDepartureCompare);
#endif

#if defined(PLOT_ALL)
    readScheduleData(directoryPath + samplePath("scheduled-arrivals", sample), &contestData.scheduleData);
#endif

    VisData visData;
    readVisData(VisData::Type_SanFrancisco, &visData);

    QApplication app(argc, argv);
    plot(visData, contestData.passengerData);

#if defined(PLOT_ALL)
    plot(visData, contestData.realtimeData);
    plot(visData, contestData.scheduleData);
    plot(visData, contestData.passengerData);
#endif

    QSharedPointer<RawProto> proto(new DLCycle(imageDirectoryPath));
    proto->accept(contestData);

    RenderData renderData;
    renderData.config = visData;
#if defined(USE_REALTIME_DATA)
    renderData.config.t0 = contestData.realtimeData.front().time;
#else
    renderData.config.t0 = contestData.passengerData.front().time.ms;
#endif
    renderData.config.scale = 0.5f;
    renderData.geoxform = GeoCoordinateTransform(renderData.config.area);
    renderData.timestepMs = 60000LL;
    renderData.framePeriodMs = 40;
    renderData.time = TimeController(renderData.config.t0 - renderData.timestepMs, renderData.timestepMs);
#if 0
    VisWindow window(proto.data(), renderData);
    window.show();
#else
    RawProto *renderer = proto.data();
    QImage outputImage(1878.0f * renderData.config.scale, 1878.0f * renderData.config.aspectRatio * renderData.config.scale, QImage::Format_ARGB32_Premultiplied);
    renderData.pixelSize = outputImage.size();

    // call init
    renderer->init(renderData);

    QPainter painter(&outputImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    for (uint32_t frame = 0; ; frame++) {
        painter.fillRect(outputImage.rect(), Qt::black);
        painter.save();
        renderer->render(renderData, &painter);
        painter.restore();
        painter.fillRect(0, 0, outputImage.width(), 2 * 40 * renderData.config.scale + 20, QBrush(QColor(0, 0, 0, 100)));

        painter.setPen(QColor(255, 255, 255));
        QFont font;
        font.setFamily("Droid Sans Mono");
        font.setPixelSize(40 * renderData.config.scale);
        painter.setFont(font);
        QString text = QString("San Francisco " + QString(toQDateTime(toTime(renderData.time.current())).toString()));
        painter.drawText(20, 40 * renderData.config.scale + 20, text);
        renderData.time.update();
        static char filename[1024];
        sprintf(filename, "output/frame_%06d.png", frame);
        outputImage.save(filename);
    }
#endif

    return app.exec();
}
