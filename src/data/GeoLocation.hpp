#pragma once

#include <QString>
#include <QPointF>

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
    GeoCoordinateTransform(const GeoArea& area = GeoArea()) :
        m_area(area)
    {
        // do nothing
    }

    // transform from lat long to pixel coordinates, normalized to [0, 1]
    QPointF apply(const GeoLocation &location) const;
protected:
    GeoArea m_area;
}; // class GeoCoordinateTransform
