#include "GeoLocation.hpp"

QPointF GeoCoordinateTransform::apply(const GeoLocation &location) const
{
   // linear transform
   const float dlatitude = location.latitude - m_area.bottomLeft.latitude;
   const float dlongitude = location.longitude -  m_area.bottomLeft.longitude;

   return QPointF(dlongitude / m_area.size.longitude, 1.0f - (dlatitude / m_area.size.latitude));
}
