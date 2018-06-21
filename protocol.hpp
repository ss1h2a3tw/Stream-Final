#pragma once

enum Interval{
    ZERO    =200000,
    ONE     =300000,
    C_ZERO  =400000,
    C_ONE   =500000
};

static const size_t I[4] = {ZERO,ONE,C_ZERO,C_ONE};
static const size_t D[4] = {ONE/2,(ZERO+ONE)/2,(ONE+C_ZERO)/2,(C_ZERO+C_ONE)/2};
