#pragma once

#include <QWidget>

#include "Renderer.hpp"

class VisWindow : public QWidget {
public:
    VisWindow(Renderer *renderer, const RenderData &renderData, QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *event);

    QImage m_buffer;
    Renderer *m_renderer;
    RenderData m_renderData;
    uint32_t m_frame;
}; // class VisWindow
