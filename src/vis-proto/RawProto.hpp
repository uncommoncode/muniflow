#pragma once

#include <data/RawContestData.hpp>

class RawProto {
public:
    virtual ~RawProto() {};
    virtual void accept(const RawContestData& data) = 0;
}; // class RawProto
