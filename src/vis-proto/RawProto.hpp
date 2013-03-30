#pragma once

#include <vis/Renderer.hpp>
#include <data/RawContestData.hpp>

class RawProto : Renderer {
public:
    virtual void render(const RenderData& renderData, QPainter *painter) = 0;
    virtual void accept(const RawContestData& data) = 0;
}; // class RawProto
