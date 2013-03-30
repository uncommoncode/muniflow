#pragma once

#include <data/Time.hpp>

class TimeController {
public:
    TimeController(int64_t t0 = 0);

    bool contains(const Time& time) const;
    int64_t absolute(const Time& time) const;
    int64_t relative(const Time& time) const;
protected:
    int64_t m_t0;
    int64_t m_tcur;
    int32_t m_tlen;
}; // class TimeController
