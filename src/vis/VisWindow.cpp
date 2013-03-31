#include "VisWindow.hpp"

#include <QtCore>
#include <QtGui>

VisWindow::VisWindow(Renderer *renderer, const RenderData &renderData, QWidget *parent) :
    QWidget(parent)
{
    m_renderer = renderer;
    m_renderData = renderData;

    this->resize(1878, 1878.0f * m_renderData.config.aspectRatio);

    m_renderData.pixelSize = this->size();
    // call init
    m_renderer->init(m_renderData);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(renderData.framePeriodMs);

    m_buffer = QImage(this->width(), this->height(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&m_buffer);
    painter.fillRect(m_buffer.rect(), Qt::black);
    m_frame = 0;
}
//#define USE_BUFFER

void VisWindow::paintEvent(QPaintEvent *event) {
#if defined(USE_BUFFER)
    QPainter painter(&m_buffer);
    painter.fillRect(m_buffer.rect(), Qt::black);

    painter.save();
#if defined(USE_NORMALIZED_COORDINATES)
    painter.scale(this->width(), this->height());
#endif
    m_renderer->render(m_renderData, &painter);

    painter.restore();

    painter.setPen(Qt::white);
    painter.drawText(20, 20, QString(toQDateTime(toTime(m_renderData.time.current())).toString()));
    painter.drawText(20, 35, QString::number(m_frame));
    m_renderData.time.update();

    QPainter windowPainter(this);
    windowPainter.fillRect(this->rect(), Qt::black);
    windowPainter.drawImage(QPoint(0, 0), m_buffer);
#else
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::black);
    painter.save();
#if defined(USE_NORMALIZED_COORDINATES)
    painter.scale(this->width(), this->height());
#endif
    m_renderer->render(m_renderData, &painter);
    painter.restore();

    painter.setPen(QColor(236, 112, 120));
    painter.drawText(20, 20, QString(toQDateTime(toTime(m_renderData.time.current())).toString()));
    painter.drawText(20, 35, QString::number(m_frame));
    m_renderData.time.update();
#endif
    m_frame++;
}

