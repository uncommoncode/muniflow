#include "Simple.hpp"

#include <data/GeoLocation.hpp>

struct Simple::SimpleData {
    const QVector<RealtimeEntry> *realtimeData;
    uint32_t index;
    float r;
}; // struct Simple::SimpleData

Simple::Simple() :
    m_impl(new Simple::SimpleData)
{
    // nothing
}

void Simple::init(const RenderData &renderData) {
    m_impl->index = 0;
    m_impl->r = 9.0f * qMin(renderData.config.area.size.latitude, renderData.config.area.size.longitude) / 500.0f;
}

void Simple::render(const RenderData &renderData, QPainter *painter) {
    // show dots sparkle over time
    uint32_t index = m_impl->index;
    size_t size = m_impl->realtimeData->size();
    for (; index < size; index++) {
        const RealtimeEntry& entry = m_impl->realtimeData->at(index);
        if (renderData.time.contains(entry.arrivalTime)) {
            renderData.time.absolute(entry.arrivalTime);
            renderData.time.relative(entry.arrivalTime);

            QPointF position = renderData.geoxform.apply(entry.position);
            painter->fillRect(QRectF(position, QSizeF(m_impl->r, m_impl->r)), Qt::green);
        } else {
            if (renderData.time.lt(entry.arrivalTime)) {
                // drop frame, in past
                index++;
            } else {
                // abort, in future
                break;
            }
        }
    }
    m_impl->index = index;
}

void Simple::accept(const RawContestData &data) {
    m_impl->realtimeData = &data.realtimeData;
}
