#pragma once

#include <vector>

class Cell
{
private:
    std::vector<Cell> m_Neighbors;

public:
    Cell();
    ~Cell();

    void addNeighbors(Cell neighbors[]);
    void PrepareUpdate();
    void Update();
};