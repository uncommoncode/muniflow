#pragma once

#include <stdint.h>

#include <QDateTime>

struct Time {
    int64_t ms;
}; // struct Time

static Time toTime(int64_t ms) {
    Time time;
    time.ms = ms;
    return time;
}

static Time toTime(const QDateTime &dateTime) {
    Time time;
    time.ms = dateTime.toMSecsSinceEpoch();
    return time;
}

static QDateTime toQDateTime(const Time &time) {
    QDateTime dateTime;
    dateTime.setMSecsSinceEpoch(time.ms);
    return dateTime;
}

class TimeTransform {
public:
    TimeTransform(uint64_t t0Ms) :
        m_t0Ms(t0Ms)
    {}

    uint64_t apply(const Time &time) const;
protected:
    uint64_t m_t0Ms;
}; // class TimeTransform
