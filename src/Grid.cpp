#include "Grid.h"

Grid::Grid(int xCells, int yCells)
    : m_XCells(xCells), m_YCells(yCells), m_Generation(0), m_Cells(std::vector<Cell>())
{
    for (int x = 0; x < m_XCells; x++) {
        for (int y = 0; y < m_YCells; y++) {
            // TODO: Figure out position and color:
            m_Cells.push_back(Cell({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f,1.0f}))
        }
    }
}

Grid::~Grid()
{
    // Is this correct ???
    delete &m_Cells;
}

void Grid::OnRender()
{
    for (Cell &cell: m_Cells) {
        cell.OnRender();
    }
}

uint64_t Grid::Update()
{
    for (Cell &cell: m_Cells) {
        cell.PrepareUpdate();
    }

    for (Cell &cell: m_Cells) {
        cell.Update();
    }

    ++m_Generation;
    return m_Generation;
}

void Grid::Reset()
{
    for (Cell &cell: m_Cells) {
        cell.MakeDead();
    }

    m_Generation = 0;
}

void Grid::RandomState(float liveProbability)
{
    // TODO: How to get random number ???
}

void Grid::setNeighborsForCellsInGrid()
{
    for (int x = 0; x < m_XCells; x++) {
        for (int y = 0; y < m_YCells; y++) {
            int idx = x + (y * m_XCells);
            m_Cells[idx].addNeighbors(getCellNeighbors(x, y));
        }
    }
}

std::vector<Cell>& Grid::getCellNeighbors(int x, int y) const
{

}

void Grid::makeAllLive()
{
    for (Cell &cell: m_Cells) {
        cell.MakeLive();
    }
}
