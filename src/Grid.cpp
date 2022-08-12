#include "Grid.h"

Grid::Grid(int xCells, int yCells)
    : m_XCells(xCells), m_YCells(yCells), m_Generation(0), m_Cells(std::vector<std::unique_ptr<Cell>>())
{
    for (int x = 0; x < m_XCells; x++) {
        for (int y = 0; y < m_YCells; y++) {
            // TODO: Figure out position and color:
            CellPosition cellPosition = {0.0f, 0.0f, 0.0f};
            CellSize cellSize = {0.9, 0.9};
            CellColor cellColor = {0.0f, 1.0f, 0.0f, 1.0f};
            m_Cells.emplace_back(std::make_unique<Cell>(cellPosition, cellSize, cellColor));
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
    for (std::unique_ptr<Cell> &cell: m_Cells) {
        cell->OnRender();
    }
}

unsigned int Grid::Update()
{
    for (std::unique_ptr<Cell> &cell: m_Cells) {
        cell->PrepareUpdate();
    }

    for (std::unique_ptr<Cell> &cell: m_Cells) {
        cell->Update();
    }

    ++m_Generation;
    return m_Generation;
}

void Grid::Reset()
{
    for (std::unique_ptr<Cell> &cell: m_Cells) {
        cell->MakeDead();
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
            m_Cells[idx]->addNeighbors(getCellNeighbors(x, y));
        }
    }
}

std::vector<std::unique_ptr<Cell>> Grid::getCellNeighbors(int x, int y) const {

}

void Grid::makeAllLive()
{
    for (std::unique_ptr<Cell> &cell: m_Cells) {
        cell->MakeLive();
    }
}
