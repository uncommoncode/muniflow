#include "GeoLocation.hpp"

#include <QtCore>

static double long2tilex(double lon, int z = 12)
{
    return (((lon + 180.0) / 360.0 * pow(2.0, z)));
}

static double lat2tiley(double lat, int z = 12)
{
    return (((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z)));
}

static GeoLocation geoToTile(const GeoLocation &location) {
    return toGeoLocation(long2tilex(location.longitude), lat2tiley(location.latitude));
}

GeoCoordinateTransform::GeoCoordinateTransform(const GeoArea &area)
{
    GeoLocation topRight = area.bottomLeft;
    topRight.latitude += area.size.latitude;
    topRight.longitude += area.size.longitude;

    GeoLocation topRightTile = geoToTile(topRight);
    GeoLocation bottomLeftTile = geoToTile(area.bottomLeft);

    GeoLocation size = topRightTile;
    size.latitude -= bottomLeftTile.latitude;
    size.longitude -= bottomLeftTile.longitude;

    m_area.bottomLeft = bottomLeftTile;
    m_area.size = size;
}


QPointF GeoCoordinateTransform::apply(const GeoLocation &location) const
{
    // nonlinear transform from lat,lon to spherical mercator
    GeoLocation tileLocation = geoToTile(location);

    const float dlatitude = tileLocation.latitude - m_area.bottomLeft.latitude;
    const float dlongitude = tileLocation.longitude - m_area.bottomLeft.longitude;

    return QPointF(dlongitude / m_area.size.longitude, (dlatitude / m_area.size.latitude));
}
