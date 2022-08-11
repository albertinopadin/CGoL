#pragma once

#include "Cell.h"
#include <vector>

class Grid
{
private:
//    Cell m_Cells[100 * 100];
    std::vector<Cell> m_Cells;

public:
    Grid();
    ~Grid();

    uint64_t Update();
    void Reset();
    void RandomState(float liveProbability);
};