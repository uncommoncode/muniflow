#pragma once

#include "TimeController.hpp"

#include <data/GeoLocation.hpp>
#include <data/VisData.hpp>

#include <QPainter>

class RenderData {
public:
    TimeController time;
    GeoCoordinateTransform geoxform;
    VisData config;
}; // class RenderData

class Renderer {
public:
    virtual ~Renderer() {};

    virtual void init(const RenderData &renderData) {};
    virtual void render(const RenderData &renderData, QPainter *painter) = 0;
}; // class Renderer
