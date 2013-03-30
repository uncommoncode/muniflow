#pragma once

#include "RawProto.hpp"

class Simple : public RawProto {
public:
    Simple();

    void init(const RenderData &renderData);
    void render(const RenderData &renderData, QPainter *painter);
    void accept(const RawContestData &data);
protected:
    struct SimpleData;
    QSharedPointer<SimpleData> m_impl;
}; // class Simple
