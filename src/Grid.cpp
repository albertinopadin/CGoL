#include "Grid.h"


Grid::Grid(int xCells, int yCells)
    : m_XCells(xCells), m_YCells(yCells), m_Generation(0), m_Cells(std::vector<std::unique_ptr<Cell>>())
{
    for (int x = 0; x < m_XCells; x++) {
        for (int y = 0; y < m_YCells; y++) {
            CellPosition cellPosition = {(float)x * (m_CellWidth + m_CellSpacing), (float)y * (m_CellHeight + m_CellSpacing), 0.0f};
            CellSize cellSize = {m_CellWidth, m_CellHeight};
            CellColor cellColor = {0.0f, 1.0f, 0.0f, 1.0f};
            m_Cells.emplace_back(std::make_unique<Cell>(cellPosition, cellSize, cellColor));
        }
    }

    setNeighborsForCellsInGrid();
}

Grid::~Grid()
{
    // Is this correct ???
    delete &m_Cells;
}

void Grid::OnRender()
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    // TODO: move draw call here and make a batch draw call
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
    if (liveProbability > 0.99f) {
        makeAllLive();
    } else {
        if (liveProbability > 0.00f) {
            for (int x = 0; x < m_XCells; x++) {
                for (int y = 0; y < m_YCells; y++) {
                    int liveP = (int)(liveProbability * 100);
                    int r = std::rand() % 100;
                    if (r <= liveP) {
                        int idx = x + (y * m_XCells);
                        m_Cells[idx]->MakeLive();
                    }
                }
            }
        }
    }
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
    int leftX   = x - 1;
    int rightX  = x + 1;
    int topY    = y + 1;
    int bottomY = y - 1;

    std::vector<std::unique_ptr<Cell>> neighbors = std::vector<std::unique_ptr<Cell>>();

    // Left
    if (leftX > -1) {
        int idx = leftX + y*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Upper left
    if (leftX > -1 && topY < m_YCells) {
        int idx = leftX + topY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Upper
    if (topY < m_YCells) {
        int idx = x + topY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Upper right
    if (rightX < m_XCells && topY < m_YCells) {
        int idx = rightX + topY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Right
    if (rightX < m_XCells) {
        int idx = rightX + y*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Lower right
    if (rightX < m_XCells && bottomY > -1) {
        int idx = rightX + bottomY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Lower
    if (bottomY > -1) {
        int idx = x + bottomY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Lower left
    if (leftX > -1 && bottomY > -1) {
        int idx = leftX + bottomY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    return neighbors;
}

void Grid::makeAllLive()
{
    for (std::unique_ptr<Cell> &cell: m_Cells) {
        cell->MakeLive();
    }
}
