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

#if 0
void dimensions() {

    float minx = 1000.0f, maxx = -1000.0f, miny = 1000.0f, maxy = -1000.0f;
    minx = min(minx, entry.position.latitude);
    maxx = max(maxx, entry.position.latitude);
    miny = min(miny, entry.position.longitude);
    maxy = max(maxy, entry.position.longitude);
    qDebug() << QString("Range: ") << minx << QString(" ") << maxx << QString(" ") << miny << QString(" ") << maxy;

}
#endif

void plot(const VisData &visData, const QVector<RealtimeEntry> &entries) {
    QImage image(512, 512, QImage::Format_ARGB32);
    QPainter painter(&image);
    GeoCoordinateTransform xform(visData.area);
    painter.fillRect(image.rect(), Qt::black);
    painter.setPen(Qt::green);
    QTransform imageScale;
    painter.scale(image.width(), image.height());
    float r = 9.0f * min(visData.area.size.latitude, visData.area.size.longitude) / image.width();
    foreach (const RealtimeEntry &entry, entries) {
        QPointF position = xform.apply(entry.position);
        painter.fillRect(QRectF(position, QSizeF(r, r)), Qt::green);
    }
    image.save("images/plot/realtime-data.png");
}

void plot(const VisData &visData, const QVector<PassengerEntry> &entries) {

}

void plot(const VisData &visData, const QVector<ScheduleEntry> &entries) {

}
