#include "Particle.hpp"

#include <QtCore>

struct PointParticle {
public:
    QPointF point;
    uint32_t decay;
}; // struct PointParticle

class ParticleManager {
public:
    void addParticle(QPointF point, uint32_t decay) {
        if (m_freelist.empty()) {
            PointParticle particle;
            particle.point = point;
            particle.decay = decay;
            m_particles.push_back(particle);
        } else {
            PointParticle *particle = m_freelist.front();
            particle->decay = decay;
            particle->point = point;
            m_freelist.pop_front();
        }
    }

    void update() {
        for (QVector<PointParticle>::iterator itr = m_particles.begin(); itr != m_particles.end(); itr++) {
            PointParticle &particle = *itr;
            if (!isValid(particle)) {
                // skip
            } else {
                // decay
                particle.decay--;
                if (!isValid(particle)) {
                    m_freelist.push_back(&particle);
                }
            }
        }
        // if all empty, reduce size
        if (m_freelist.size() == m_particles.size()) {
            m_freelist.clear();
            m_particles.clear();
        }
    }

    const QVector<PointParticle>& particles() const {
        return m_particles;
    }

    inline static bool isValid(const PointParticle &particle) {
        return particle.decay != 0;
    }

protected:
    QList<PointParticle*> m_freelist;
    QVector<PointParticle> m_particles;
}; // class ParticleManager

struct Particle::Impl {
    const QVector<RealtimeEntry> *realtimeData;
    ParticleManager particleManager;
    uint32_t index;
    float decayMax;
    float r;
    uint64_t time;
}; // struct Particle::Impl

Particle::Particle() :
    m_impl(new Particle::Impl)
{
    // nothing
}

void Particle::init(const RenderData &renderData) {
    m_impl->index = 0;
    m_impl->r = 100.0f * qMin(renderData.config.area.size.latitude, renderData.config.area.size.longitude) / 500.0f;
    m_impl->decayMax = 100.0f * float(renderData.framePeriodMs);
    m_impl->time = 0;
}

void Particle::render(const RenderData &renderData, QPainter *painter) {
    // show dots sparkle over time

    // update particles
    m_impl->particleManager.update();

    // add new particles
    uint32_t index = m_impl->index;
    size_t size = m_impl->realtimeData->size();
    for (; index < size; index++) {
        const RealtimeEntry& entry = m_impl->realtimeData->at(index);
        if (m_impl->time > entry.arrivalTime.ms) {
            qDebug() << QString("Time not monotonic! ") << entry.arrivalTime.ms;
        }
        m_impl->time = entry.arrivalTime.ms;
        if (renderData.time.contains(entry.arrivalTime)) {
            QPointF position = renderData.geoxform.apply(entry.position);
            uint32_t decay = uint32_t(m_impl->decayMax * double(renderData.time.relative(entry.arrivalTime)) / double(renderData.timestepMs));
            m_impl->particleManager.addParticle(position, decay);
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

    if (index == size) {
        qDebug() << "Finished!";
    }

    // draw particles
    const QVector<PointParticle> &particles = m_impl->particleManager.particles();
    foreach (const PointParticle& particle, particles) {
        if (ParticleManager::isValid(particle)) {
            float value = particle.decay / m_impl->decayMax;
            uint8_t green = value >= 1.0f ? 0xff : uint8_t(255.0f * value);
            QColor color = qRgb(0, green, 0);
            painter->fillRect(QRectF(particle.point, QSizeF(m_impl->r, m_impl->r)), color);
        }
    }

    m_impl->index = index;
}

void Particle::accept(const RawContestData &data) {
    m_impl->realtimeData = &data.realtimeData;
}
