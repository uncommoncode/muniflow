#include "VisData.hpp"

void readVisData(enum VisData::Type type, VisData *data) {
    switch (type) {
        case VisData::Type_SanFrancisco: {
        //  37.7058 " " 37.8324 " " -122.539 " " -122.365
            GeoLocation bottomLeft = toGeoLocation(-122.538f, 37.704f);
            GeoLocation topRight = toGeoLocation(-122.366f, 37.833f);
            data->t0 = 1349112894000LL;
            data->area.bottomLeft = bottomLeft;
            data->area.size = toGeoLocation(topRight.longitude - bottomLeft.longitude, topRight.latitude - bottomLeft.latitude);
        } break;
    }
}
