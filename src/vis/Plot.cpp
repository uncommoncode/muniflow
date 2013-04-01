#include "Plot.hpp"

#include <QtCore>
#include <QtGui>

#include <data/GeoLocation.hpp>

float min(float a, float b) {
    return a < b ? a : b;
}

float max(float a, float b) {
    return a > b ? a : b;
}

template <typename ENTRY_TYPE>
void printDimensions(const QVector<ENTRY_TYPE> &entries) {
    const ENTRY_TYPE &entry = entries.at(0);
    float minx = entry.position.longitude, maxx = entry.position.longitude, miny = entry.position.latitude, maxy = entry.position.latitude;
    foreach (const ENTRY_TYPE &entry, entries) {
        minx = min(minx, entry.position.latitude);
        maxx = max(maxx, entry.position.latitude);
        miny = min(miny, entry.position.longitude);
        maxy = max(maxy, entry.position.longitude);
    }
    qDebug() << QString("Range: ") << minx << QString(" ") << maxx << QString(" ") << miny << QString(" ") << maxy;
}

void plot(const VisData &visData, const QVector<RealtimeEntry> &entries) {
    QImage image(512, 512, QImage::Format_ARGB32);
    QPainter painter(&image);
    GeoCoordinateTransform xform(visData.area);
    painter.fillRect(image.rect(), Qt::black);
    painter.setPen(Qt::green);
    painter.scale(image.width(), image.height());
    float r = 9.0f * min(visData.area.size.latitude, visData.area.size.longitude) / image.width();
    foreach (const RealtimeEntry &entry, entries) {
        QPointF position = xform.apply(entry.position);

        qDebug() << toString(entry.position);
        painter.fillRect(QRectF(position, QSizeF(r, r)), Qt::green);
    }
    image.save("images/plot/realtime-data.png");
}

void plot(const VisData &visData, const QVector<PassengerEntry> &entries) {
    QImage image(512, 512, QImage::Format_ARGB32);
    QPainter painter(&image);
    GeoCoordinateTransform xform(visData.area);
    painter.fillRect(image.rect(), Qt::black);
    painter.setPen(Qt::green);
    painter.scale(image.width(), image.height());
    float r = 9.0f * min(visData.area.size.latitude, visData.area.size.longitude) / image.width();
    foreach (const PassengerEntry &entry, entries) {
        QPointF position = xform.apply(entry.location);
        qDebug() << toString(entry.location);
        painter.fillRect(QRectF(position, QSizeF(r, r)), Qt::green);
    }
    image.save("images/plot/passenger-data.png");
}

void plot(const VisData &visData, const QVector<ScheduleEntry> &entries) {

}
