#include "TimeController.hpp"

TimeController::TimeController(int64_t t0, int64_t tlen) {
    m_t0 = t0;
    m_tlen = tlen;
    m_tcur = m_t0;
}

bool TimeController::contains(const Time& time) const {
    return (time.ms >= m_tcur) && (time.ms < (m_tcur + m_tlen));
}

bool TimeController::gt(const Time& time) const {
    return (time.ms >= (m_tcur + m_tlen));
}

bool TimeController::lt(const Time& time) const {
    return  (time.ms < m_tcur);
}

int64_t TimeController::absolute(const Time& time) const {
    return time.ms - m_t0;
}

int64_t TimeController::relative(const Time& time) const {
    return time.ms - m_tcur;
}

int64_t TimeController::current()const {
    return m_tcur;
}

void TimeController::update() {
    m_tcur += m_tlen;
}
