#include "Cell.h"

Cell::Cell(CellPosition position, CellColor color)
    : m_Position(position), m_Color(color), alive(false), m_Neighbors(std::vector<Cell>()),
    m_CurrentState(CellState::Dead), m_NextState(CellState::Dead)
{

}

Cell::~Cell()
{

}

void Cell::OnRender()
{
    // TODO: Actually render cell:
}

void Cell::addNeighbors(std::vector<Cell> &neighbors)
{
    m_Neighbors = neighbors;
}

void Cell::PrepareUpdate()
{
    int liveNeighbors = 0;
    for (const Cell& neighbor : m_Neighbors) {
        if (neighbor.alive) {
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
    // Make alpha 0
}

void Cell::MakeLive() {
    SetState(CellState::Alive);
    // Make alpha 1
}

bool Cell::needsUpdate() {
    return m_CurrentState != m_NextState;
}
