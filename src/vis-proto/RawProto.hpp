#pragma once

#include <vis/Renderer.hpp>
#include <data/RawContestData.hpp>

class RawProto : public Renderer {
public:
    virtual void render(const RenderData& renderData, QPainter *painter) = 0;
    virtual void accept(const RawContestData& data) = 0;
}; // class RawProto
