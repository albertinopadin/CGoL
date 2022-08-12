#pragma once

#include "Cell.h"
#include <vector>
#include <memory>

class Grid
{
private:
//    Cell m_Cells[100 * 100];
    int m_XCells, m_YCells;
    unsigned int m_Generation;
    std::vector<std::unique_ptr<Cell>> m_Cells;

public:
    Grid(int xCells, int yCells);
    ~Grid();

    void OnRender();
    unsigned int Update();
    void Reset();
    void RandomState(float liveProbability);

private:
    void setNeighborsForCellsInGrid();
    std::vector<std::unique_ptr<Cell>> getCellNeighbors(int x, int y) const;
    void makeAllLive();
};