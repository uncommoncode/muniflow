#include "DLCycle.hpp"

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

struct DLCycle::Impl {
    const QVector<RealtimeEntry> *realtimeData;
    ParticleManager particleManager;
    uint32_t index;
    float decayMax;
    float r;
    uint64_t time;
    QImage basemap;
}; // struct Particle::Impl

DLCycle::DLCycle(const QString& imageDirectoryPath) :
    m_impl(new DLCycle::Impl)
{
    m_impl->basemap = QImage(imageDirectoryPath + "base_map.png");
}

void DLCycle::init(const RenderData &renderData) {
    m_impl->index = 0;
    m_impl->r = 100.0f * qMin(renderData.config.area.size.latitude, renderData.config.area.size.longitude) / 500.0f;
    m_impl->decayMax = 100.0f * float(renderData.framePeriodMs);
    m_impl->time = 0;
}

void setAlpha(QImage *image, uint8_t alpha) {
    for (int y = 0; y < image->height(); y++) {
        QRgb *scanline = (QRgb*)image->scanLine(y);
        for (int x = 0; x < image->width(); x++) {
            uint8_t a = ((uint8_t*)&scanline[x])[3];
            if (a > 10) {
                ((uint8_t*)&scanline[x])[3] = alpha;
            }
        }
    }
}

double long2tilex(double lon, int z = 12)
{
    return (((lon + 180.0) / 360.0 * pow(2.0, z)));
}

double lat2tiley(double lat, int z = 12)
{
    return (((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z)));
}

QPointF geoToTile(const GeoLocation &location) {
    return QPointF(long2tilex(location.longitude), lat2tiley(location.latitude));
}

void DLCycle::render(const RenderData &renderData, QPainter *painter) {
    // show dots sparkle over time
    QDateTime dateTime;
    dateTime.setMSecsSinceEpoch(renderData.time.current());
    int hour = dateTime.time().hour();
    int minute = dateTime.time().minute();
    int second = dateTime.time().second();
    //painter->fillRect(QRect(QPoint(0, 0), renderData.pixelSize), QBrush(QColor(0, 40, 200)));

    // noon peak, +/- 6 hours of sun
    float h = (float(hour) + float(minute) / 60.0f + float(second) / 3600.0f) / 24.0f;
    float dh = 0.5f - h;
    uint8_t v = uint8_t(255.0f * 2.0f * qAbs(dh));
    painter->fillRect(QRect(QPoint(0, 0), renderData.pixelSize), QBrush(QColor(240, 240, 240, 255)));
    painter->fillRect(QRect(QPoint(0, 0), renderData.pixelSize), QBrush(QColor(0, 0, 0, v)));
    QImage image(m_impl->basemap);
    painter->setCompositionMode(QPainter::CompositionMode_SourceAtop);
    //setAlpha(&image, uint8_t(v * 0.75f + 255.0f * 0.25f));
    const float imscale = 1.0;
    painter->scale(imscale, imscale);
    painter->drawImage(0, 0, image);

    // draw all glyphs
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->scale(renderData.pixelSize.width(), renderData.pixelSize.height());

    qDebug() << renderData.pixelSize;
    size_t size = m_impl->realtimeData->size();
#if 1
    GeoLocation topRight;
    topRight.latitude = renderData.config.area.bottomLeft.latitude + renderData.config.area.size.latitude;
    topRight.longitude = renderData.config.area.bottomLeft.longitude + renderData.config.area.size.longitude;
    QPointF trp = geoToTile(topRight);
    qDebug() << trp;
    QPointF blp = geoToTile(renderData.config.area.bottomLeft);
    qDebug() << blp;

    QPointF sp(trp.x() - blp.x(), (trp.y() - blp.y()));

    qDebug() << sp;

    for (uint32_t index = 0; index < size; index++) {
        const RealtimeEntry& entry = m_impl->realtimeData->at(index);
        QPointF position0 = geoToTile(entry.position);
        position0.setX((position0.x() - blp.x()) / sp.x());
        position0.setY((position0.y() - blp.y()) / sp.y());
        QPointF position = renderData.geoxform.apply(entry.position);

        qDebug() << position0 << position;

        painter->fillRect(QRectF(position, QSizeF(0.005, 0.005)), Qt::green);
    }
#else
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
#endif
}

void DLCycle::accept(const RawContestData &data) {
    m_impl->realtimeData = &data.realtimeData;
}
