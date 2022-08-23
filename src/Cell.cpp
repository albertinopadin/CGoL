#include "Cell.h"

#include <utility>


Cell::Cell(CellPosition position, CellSize size, CellColor cellColor)
    : alpha(CellAlpha::dead), color(cellColor), alive(false),
      m_Neighbors(std::vector<std::unique_ptr<Cell>>()), m_CurrentState(false), m_NextState(false),
      vertices(InitVertices(position, size, cellColor)), indices {0, 1, 2, 1, 2, 3}
{
    SetAlphaInVertices();
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

    if (!(!m_CurrentState && liveNeighbors < 3)) {
        m_NextState = (m_CurrentState && liveNeighbors == 2) || (liveNeighbors == 3);
    }
}

void Cell::Update()
{
    if (needsUpdate()) {
        if (m_NextState) {
            MakeLive();
        } else {
            MakeDead();
        }
    }
}

void Cell::SetState(bool state)
{
    m_CurrentState = state;
    alive = state;
    m_NextState = state;
}

void Cell::MakeLive()
{
    SetState(true);
    alpha = CellAlpha::live;
    SetAlphaInVertices();
}

void Cell::MakeDead()
{
    SetState(false);
    alpha = CellAlpha::dead;
    SetAlphaInVertices();
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
