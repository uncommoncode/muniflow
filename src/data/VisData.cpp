#include "VisData.hpp"

void readVisData(enum VisData::Type type, VisData *data) {
    switch (type) {
        case VisData::Type_SanFrancisco: {
#if 0
        //  -122.538,37.7026,-122.3534,37.833
            GeoLocation bottomLeft = toGeoLocation(-122.538f, 37.7026f);
            GeoLocation topRight = toGeoLocation(-122.3534f, 37.833f);
            data->aspectRatio = (1788.0f / 2000.0f);
#else
        // estimate: -122.569 -122.338 37.832 37.704
            GeoLocation bottomLeft = toGeoLocation(-122.58f, 37.832f);
            GeoLocation topRight = toGeoLocation(-122.338f, 37.702f);
            data->aspectRatio = 1272.0f / 1878.0f;
#endif
            data->t0 = 1349112894000LL;
            data->area.bottomLeft = bottomLeft;
            data->area.size = toGeoLocation(topRight.longitude - bottomLeft.longitude, topRight.latitude - bottomLeft.latitude);

        } break;
    }
}
