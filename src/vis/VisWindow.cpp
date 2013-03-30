#include "VisWindow.hpp"

#include <QtCore>
#include <QtGui>

VisWindow::VisWindow(Renderer *renderer, const RenderData &renderData, QWidget *parent) :
    QWidget(parent)
{
    m_renderer = renderer;
    m_renderData = renderData;
    // call init
    m_renderer->init(m_renderData);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);

    m_buffer = QImage(this->width(), this->height(), QImage::Format_ARGB32);
    QPainter painter(&m_buffer);
    painter.fillRect(m_buffer.rect(), Qt::black);
}

void VisWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(&m_buffer);

    painter.scale(m_buffer.width(), m_buffer.height());

    m_renderer->render(m_renderData, &painter);
    m_renderData.time.update();

    QPainter windowPainter(this);
    windowPainter.drawImage(QPoint(0, 0), m_buffer);
}

