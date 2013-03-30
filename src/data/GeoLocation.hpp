#pragma once

#include <QString>

struct GeoLocation {
    float latitude, longitude;
}; // struct GeoLocation

static GeoLocation toGeoLocation(const QString &longitude, const QString &latitude) {
    GeoLocation location;
    location.latitude = latitude.toFloat();
    location.longitude = longitude.toFloat();
    return location;
}
