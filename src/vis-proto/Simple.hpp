#pragma once

#include "RawProto.hpp"

class Simple : public RawProto {
public:
    void accept(const RawContestData &data);
}; // class Simple
