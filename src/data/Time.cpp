#include "Time.hpp"

uint64_t TimeTransform::apply(const Time &time) const {
    return time.ms - m_t0Ms;
}
