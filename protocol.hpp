#pragma once

enum Interval{
    ZERO=10,
    ONE=40,
    C_ZERO=80,
    C_ONE=110
};
static const int D[3] = {(ZERO+ONE)/2,(ONE+C_ZERO)/2,(C_ZERO+C_ONE)/2};
