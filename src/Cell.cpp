#include "Cell.h"

#include <utility>


Cell::Cell(CellPosition position, CellSize size, CellColor cellColor)
    : alpha(CellAlpha::live), color(cellColor), alive(false),
    m_Neighbors(std::vector<std::unique_ptr<Cell>>()), m_CurrentState(CellState::Dead), m_NextState(CellState::Dead),
    vertices(initVertices(position, size)), indices { 0, 1, 2, 1, 2, 3}
{

}

float *Cell::initVertices(CellPosition position, CellSize size) {
    // Calculate cell vertices based on position and size
    float halfCellWidth = size.width / 2;
    float halfCellHeight = size.height / 2;
    return new float[numVertices * positionComponentsPerVertex] {
            position.x - halfCellWidth, position.y - halfCellHeight, position.z,
            position.x - halfCellWidth, position.y + halfCellHeight, position.z,
            position.x + halfCellWidth, position.y - halfCellHeight, position.z,
            position.x + halfCellWidth, position.y + halfCellHeight, position.z,
    };
}

Cell::~Cell()
{

}

void Cell::addNeighbors(std::vector<std::unique_ptr<Cell>> neighbors)
{
    m_Neighbors = std::move(neighbors);
}

void Cell::PrepareUpdate()
{
    int liveNeighbors = 0;
    for (std::unique_ptr<Cell> &neighbor : m_Neighbors) {
        if (neighbor->alive) {
            ++liveNeighbors;
        }
    }

    if (!(m_CurrentState == CellState::Dead && liveNeighbors < 3)) {
        m_NextState = (m_CurrentState == CellState::Alive && liveNeighbors == 2) ||
                (liveNeighbors == 3) ? CellState::Alive : CellState::Dead;
    }
}

void Cell::Update()
{
    if (needsUpdate()) {
        if (m_NextState == CellState::Alive) {
            MakeLive();
        } else {
            MakeDead();
        }
    }
}

void Cell::SetState(CellState state) {
    m_CurrentState = state;
    alive = m_CurrentState == CellState::Alive;
    m_NextState = m_CurrentState;
}

void Cell::MakeDead() {
    SetState(CellState::Dead);
    alpha = CellAlpha::dead;
}

void Cell::MakeLive() {
    SetState(CellState::Alive);
    alpha = CellAlpha::live;
}

bool Cell::needsUpdate() {
    return m_CurrentState != m_NextState;
}
