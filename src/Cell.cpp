#include "Cell.h"

#include <utility>


Cell::Cell(CellPosition position, CellSize size, CellColor cellColor)
    : alpha(CellAlpha::dead), color(cellColor), alive(false),
      m_Neighbors(std::vector<std::unique_ptr<Cell>>()), m_CurrentState(CellState::Dead), m_NextState(CellState::Dead),
      vertices(InitVertices(position, size, cellColor)), indices {0, 1, 2, 1, 2, 3}
{

}

float *Cell::InitVertices(CellPosition position, CellSize size, CellColor color) {
    // Calculate cell vertices based on position and size
    float halfCellWidth = size.width / 2;
    float halfCellHeight = size.height / 2;
    float x = position.x;
    float y = position.y;
    float z = position.z;
    return new float[numVertices * componentsPerVertex] {
            x - halfCellWidth, y - halfCellHeight, z, color.r, color.g, color.b, color.alpha,
            x - halfCellWidth, y + halfCellHeight, z, color.r, color.g, color.b, color.alpha,
            x + halfCellWidth, y - halfCellHeight, z, color.r, color.g, color.b, color.alpha,
            x + halfCellWidth, y + halfCellHeight, z, color.r, color.g, color.b, color.alpha
    };
}

Cell::~Cell()
{

}

void Cell::AddNeighbors(std::vector<std::unique_ptr<Cell>> neighbors)
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

    SetAlphaInVertices();
}

void Cell::SetState(CellState state)
{
    m_CurrentState = state;
    alive = m_CurrentState == CellState::Alive;
    m_NextState = m_CurrentState;
}

void Cell::MakeDead()
{
    SetState(CellState::Dead);
    alpha = CellAlpha::dead;
}

void Cell::MakeLive()
{
    SetState(CellState::Alive);
    alpha = CellAlpha::live;
}

void Cell::SetAlphaInVertices() const
{
    vertices[6] = alpha;
    vertices[13] = alpha;
    vertices[20] = alpha;
    vertices[27] = alpha;
}

bool Cell::needsUpdate()
{
    return m_CurrentState != m_NextState;
}
