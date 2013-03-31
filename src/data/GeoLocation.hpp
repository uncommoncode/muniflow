#pragma once

#include <QString>
#include <QPointF>

#include <cmath>

struct GeoLocation {
    float latitude, longitude;
}; // struct GeoLocation

static GeoLocation toGeoLocation(const QString &longitude, const QString &latitude) {
    GeoLocation location;
    location.latitude = latitude.toFloat();
    location.longitude = longitude.toFloat();
    return location;
}

static GeoLocation toGeoLocation(float longitude, float latitude) {
    GeoLocation location;
    location.latitude = latitude;
    location.longitude = longitude;
    return location;
}

static QString toString(const GeoLocation &location) {
    return QString("GeoLocation(") + QString::number(location.longitude) + ", " + QString::number(location.latitude) + ")";
}

struct GeoArea {
    GeoLocation bottomLeft;
    GeoLocation size;
}; // struct GeoArea

class GeoCoordinateTransform {
public:
    GeoCoordinateTransform(const GeoArea& area = GeoArea());

    // transform from lat long to pixel coordinates, normalized to [0, 1]
    QPointF apply(const GeoLocation &location) const;
protected:
    GeoArea m_area;
}; // class GeoCoordinateTransform

static QPointF WGS84toSphericalMercator(const GeoLocation &location) {
    const double s = 100.0;
    double x = location.longitude * s / 180.0;
    double y = log(tan((90.0 + location.latitude) * M_PI / 360.0)) / (M_PI / 180.0);
    y = y * s / 180.0;
    return QPointF(x, y);
}
