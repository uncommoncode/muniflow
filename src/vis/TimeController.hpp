#pragma once

#include <data/Time.hpp>

class TimeController {
public:
    TimeController(int64_t t0 = 0, int64_t tlen = 0);

    bool contains(const Time& time) const;
    bool lt(const Time& time) const;
    bool gt(const Time& time) const;
    int64_t absolute(const Time& time) const;
    int64_t relative(const Time& time) const;

    void update();
protected:
    int64_t m_t0;
    int64_t m_tcur;
    int64_t m_tlen;
}; // class TimeController
