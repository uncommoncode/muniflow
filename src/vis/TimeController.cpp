#include "TimeController.hpp"

TimeController::TimeController(int64_t t0) {
    m_t0 = t0;
    m_tcur = m_t0;
    m_tlen = 0;
}

bool TimeController::contains(const Time& time) const {
    return (time.ms >= m_tcur) && (time.ms < (m_tcur + m_tlen));
}

int64_t TimeController::absolute(const Time& time) const {
    return time.ms - m_t0;
}

int64_t TimeController::relative(const Time& time) const {
    return time.ms - m_tcur;
}
