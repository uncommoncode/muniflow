#include "VisData.hpp"

void readVisData(enum VisData::Type type, VisData *data) {
    switch (type) {
        case VisData::Type_SanFrancisco: {
        //  -122.538,37.7026,-122.3534,37.833
            GeoLocation bottomLeft = toGeoLocation(-122.538f, 37.7026f);
            GeoLocation topRight = toGeoLocation(-122.3534f, 37.833f);
            data->t0 = 1349112894000LL;
            data->area.bottomLeft = bottomLeft;
            data->area.size = toGeoLocation(topRight.longitude - bottomLeft.longitude, topRight.latitude - bottomLeft.latitude);
            data->aspectRatio = (1873.0f / 2096.0f);
        } break;
    }
}
