#pragma once

#include "RawProto.hpp"

class DLCycle : public RawProto {
public:
    DLCycle(const QString &imageDirectoryPath);

    void init(const RenderData &renderData);
    void render(const RenderData &renderData, QPainter *painter);
    void accept(const RawContestData &data);
protected:
    struct Impl;
    QSharedPointer<Impl> m_impl;
}; // class Simple
