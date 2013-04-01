#include "DLCycle.hpp"

#include <QtCore>
#include <QRadialGradient>
#include <QHash>

struct PointParticle {
public:
    QPointF point;
    uint32_t decay;
    float scale;
}; // struct PointParticle

class ParticleManager {
public:
    void addParticle(QPointF point, float scale, uint32_t decay) {
        if (m_freelist.empty()) {
            PointParticle particle;
            particle.point = point;
            particle.decay = decay;
            particle.scale = scale;
            m_particles.push_back(particle);
        } else {
            PointParticle *particle = m_freelist.front();
            particle->decay = decay;
            particle->point = point;
            particle->scale = scale;
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

class VectorParticleManager {
public:
    VectorParticleManager() {
        // nothing
    }

    void addEvent(uint32_t carUID, const QPointF& point, uint64_t time) {
        CarData &data = m_cars[carUID];
        data.points.push_back(point);
        data.polygon = QPolygonF(data.points);
    }

    void update() {

    }

    void draw(QPainter *painter) {
        painter->setPen(QColor(255, 0, 0));
        foreach (const CarData &car, m_cars) {
            painter->drawPolyline(car.polygon);
        }
    }

protected:
    struct CarData {
        QVector<QPointF> points;
        QPolygonF polygon;
    };
    QHash<uint32_t, CarData> m_cars;
}; // class VectorParticleManager

struct DLCycle::Impl {

#if defined(USE_REALTIME_DATA)
    const QVector<RealtimeEntry> *realtimeData;
#else
    const QVector<PassengerEntry> *passengerData;
#endif
#if defined(USE_VECTOR)
    VectorParticleManager particleManager;
#else
    ParticleManager particleManager;
#endif
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
    m_impl->decayMax = 10;
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

float clamp(float value, float min, float max) {
    return value > max ? max : value < min ? min : value;
}

QColor mix(float value, const QColor &start, const QColor &stop) {
    float hue = start.hueF() * (1.0f - value) + stop.hueF() * value;
    float saturation = start.saturationF() * (1.0f - value) + stop.saturationF() * value;
    float lightness = start.lightnessF() * (1.0f - value) + stop.lightnessF() * value;
    return QColor::fromHslF(hue, saturation, lightness);
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
    float nh = 2.0f * qAbs(dh);
    uint8_t v = uint8_t(255.0f * nh);

    // sunset red - blue, sunrise yellow
    painter->setCompositionMode(QPainter::CompositionMode_SourceAtop);

    QColor nightColor(8, 29, 88);
    QColor dayColor(235, 120, 20);
    QColor timeColor = nh < 0.5f ? dayColor : nightColor;
    timeColor.setAlpha(70);
    painter->fillRect(QRect(QPoint(0, 0), renderData.pixelSize), QBrush(QColor(240, 240, 240, 255)));
    //painter->fillRect(QRect(QPoint(0, 0), renderData.pixelSize), QBrush(timeColor));
    painter->fillRect(QRect(QPoint(0, 0), renderData.pixelSize), QBrush(QColor(0, 0, 0, v)));
    QImage image(m_impl->basemap);
    painter->setCompositionMode(QPainter::CompositionMode_SourceAtop);
    //setAlpha(&image, uint8_t(v * 0.75f + 255.0f * 0.25f));
    const float imscale = 1.0;
    painter->scale(imscale, imscale);
    painter->drawImage(0, 0, image);
    painter->setCompositionMode(QPainter::CompositionMode_Darken);
    uint8_t darkness = uint8_t(170.0f * 2.0f * qAbs(dh)) + 20;
    painter->fillRect(QRect(QPoint(0, 0), renderData.pixelSize), QBrush(QColor(0, 0, 0, darkness)));

    painter->setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter->fillRect(QRect(QPoint(0, 0), renderData.pixelSize), QBrush(QColor(255, 255, 255, 255)));

    // draw all glyphs
#if 0
    painter->setCompositionMode(QPainter::CompositionMode_SourceAtop);
    {
        QSizeF size(100, 100);
        QPointF point(500, 500);
        painter->setPen(Qt::NoPen);
        QRadialGradient gradient(point, size.width() * 0.5);
        gradient.setColorAt(0, QColor(30, 255, 64, 255));
        gradient.setColorAt(1, QColor(30, 255, 64, 0));

        QBrush brush(gradient);
        painter->setBrush(brush);

        painter->drawEllipse(point, renderData.config.aspectRatio * size.width(), size.width());

    }
#endif
    float sx = renderData.pixelSize.width() * imscale;
    float sy = renderData.pixelSize.height() * imscale;
    //painter->scale(renderData.pixelSize.width(), renderData.pixelSize.height());


#if 0
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->fillRect(QRectF(QPointF(0.6, 0.5), QSizeF(0.01, 0.01)), QBrush(QColor(0, 0, 0, (255) & 0xff)));
#endif
#if defined(USE_REALTIME_DATA)
    size_t size = m_impl->realtimeData->size();
#else
    size_t size = m_impl->passengerData->size();
#endif
#if 0
    // plot them all!
    for (uint32_t index = 0; index < size; index++) {
        const RealtimeEntry& entry = m_impl->realtimeData->at(index);
        QPointF position = renderData.geoxform.apply(entry.position);

        painter->fillRect(QRectF(position, QSizeF(0.003, 0.003)), Qt::green);
    }
#else
    // update particles
    m_impl->particleManager.update();

    // add new particles
    uint32_t index = m_impl->index;
    for (; index < size; index++) {
#if defined(USE_REALTIME_DATA)
        const RealtimeEntry& entry = m_impl->realtimeData->at(index);
#else
        const PassengerEntry& entry = m_impl->passengerData->at(index);
#endif
#if 0
        if (m_impl->time > entry.arrivalTime.ms) {
            qDebug() << QString("Time not monotonic! ") << entry.arrivalTime.ms;
        }
        m_impl->time = entry.arrivalTime.ms;
#endif
        if (renderData.time.contains(entry.time)) {
            QPointF position = renderData.geoxform.apply(entry.location);
            uint32_t decay = uint32_t(m_impl->decayMax * double(renderData.time.relative(entry.time)) / double(renderData.timestepMs));
#if defined(USE_VECTOR)
            m_impl->particleManager.addEvent(qHash(entry.vehicleAssignment), position, entry.arrivalTime.ms);
#else
            if ((entry.peopleOn > 0) || (entry.peopleOff > 0)) {
                m_impl->particleManager.addParticle(position, 0.25f * sqrt(entry.load), decay);
            }
#endif
        } else {
            if (renderData.time.lt(entry.time)) {
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
    // draw low frequency, high frequency with different decays -> spatial fft
#if 1

    QColor prettyPretty(65, 182, 196);
    QColor startColor(255, 255, 217);
    QColor stopColor(8, 29, 88);
    painter->setCompositionMode(QPainter::CompositionMode_SourceAtop);
    const QVector<PointParticle> &particles = m_impl->particleManager.particles();
    foreach (const PointParticle& particle, particles) {
        if (ParticleManager::isValid(particle)) {
            float value = float(particle.decay) / m_impl->decayMax;
            uint8_t component = value >= 1.0f ? 0xff : uint8_t(255.0f * value);
            QSizeF size(sx * 0.003 * particle.scale * particle.decay, sy * 0.003 * particle.scale);
            //QPointF point((particle.point.x() - size.width() * 0.3, particle.point.y() - size.height() * 0.3);
            QPointF point(sx * particle.point.x(), sy * particle.point.y());
            painter->setPen(Qt::NoPen);
            QRadialGradient gradient(point, size.width() * 0.5f);
            QColor color = mix(clamp(particle.scale, 0.0, 1.0), stopColor, startColor);
            color = QColor(255, 255, 255);
            color = prettyPretty;
            gradient.setColorAt(0, QColor(color.red(), color.green(), color.blue(), component));
            gradient.setColorAt(1, QColor(color.red(), color.green(), color.blue(), 0));

            QBrush brush(gradient);
            painter->setBrush(brush);
            painter->drawEllipse(point, size.width(), size.width());
#if 0
            color = QColor(65, 182, 196);
            float velocity = 4.0f;
            float pointWidth = 5;
            gradient = QRadialGradient(point, pointWidth);
            gradient.setColorAt(0, QColor(color.red(), color.green(), color.blue(), clamp(value / velocity, 0.0f, 1.0f) * velocity * 255.0f));
            gradient.setColorAt(1, QColor(color.red(), color.green(), color.blue(), 0));

            painter->setBrush(gradient);
            painter->drawEllipse(point, pointWidth, pointWidth);
#endif
        }
    }
#else
    // stuff
    m_impl->particleManager.draw(painter);
#endif

    m_impl->index = index;
#endif
}

void DLCycle::accept(const RawContestData &data) {
#if defined(USE_REALTIME_DATA)
    m_impl->realtimeData = &data.realtimeData;
#else
    m_impl->passengerData = &data.passengerData;
#endif
}
