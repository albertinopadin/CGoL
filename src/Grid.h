#pragma once

#include "Cell.h"
#include <vector>

class Grid
{
private:
//    Cell m_Cells[100 * 100];
    int m_XCells, m_YCells;
    uint64_t m_Generation;
    std::vector<Cell> m_Cells;

public:
    Grid(int xCells, int yCells);
    ~Grid();

    void OnRender();
    uint64_t Update();
    void Reset();
    void RandomState(float liveProbability);

private:
    void setNeighborsForCellsInGrid();
    std::vector<Cell>& getCellNeighbors(int x, int y) const;
    void makeAllLive();
};