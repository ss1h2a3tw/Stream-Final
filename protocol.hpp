#pragma once

enum Interval{
    ZERO    =100000,
    ONE     =150000,
    C_ZERO  =100000,
    C_ONE   =200000
};

static const size_t I[4] = {ZERO,ONE,C_ZERO,C_ONE};
static const size_t D[3] = {ONE/4,(ZERO+ONE)/2,(C_ZERO+C_ONE)/2};
