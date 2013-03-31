#include "VisWindow.hpp"

#include <QtCore>
#include <QtGui>

VisWindow::VisWindow(Renderer *renderer, const RenderData &renderData, QWidget *parent) :
    QWidget(parent)
{
    m_renderer = renderer;
    m_renderData = renderData;

    this->resize(512, 512.0f * renderData.config.aspectRatio);
    // call init
    m_renderer->init(m_renderData);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(renderData.framePeriodMs);

    m_buffer = QImage(this->width(), this->height(), QImage::Format_ARGB32);
    QPainter painter(&m_buffer);
    painter.fillRect(m_buffer.rect(), Qt::black);
    m_frame = 0;
}

void VisWindow::paintEvent(QPaintEvent *event) {
#if defined(USE_BUFFER)
    QPainter painter(&m_buffer);

    painter.scale(m_buffer.width(), m_buffer.height());

    m_renderer->render(m_renderData, &painter);
    m_renderData.time.update();

    QPainter windowPainter(this);
    windowPainter.fillRect(this->rect(), Qt::black);
    windowPainter.drawImage(QPoint(0, 0), m_buffer);
#else
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::black);
    painter.setPen(Qt::white);
    painter.drawText(20, 20, QString(toQDateTime(toTime(m_renderData.time.current())).toString()));
    painter.drawText(20, 35, QString::number(m_frame));
    painter.scale(this->width(), this->height());
    m_renderer->render(m_renderData, &painter);
    m_renderData.time.update();
#endif
    m_frame++;
}

