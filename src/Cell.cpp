#include "Cell.h"

#include <utility>


Cell::Cell(CellPosition position, CellSize size, CellColor color)
    : m_Alpha(CellAlpha::dead), alive(false),
    m_Neighbors(nullptr), m_CurrentState(CellState::Dead), m_NextState(CellState::Dead),
    m_Vertices(initVertices(position, size)), m_Indices { 0, 1, 2, 1, 2, 3}
{
    m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices, 4*2*sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

    m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices, 6);

    m_Shader = std::make_unique<Shader>("filepath");
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", color.r, color.g, color.b, color.alpha);
}

float *Cell::initVertices(CellPosition position, CellSize size) {
    // Calculate cell vertices based on position and size
    float halfCellWidth = size.width / 2;
    float halfCellHeight = size.height / 2;
    return new float[8] {
            position.x - halfCellWidth, position.y - halfCellHeight,
            position.x - halfCellWidth, position.y + halfCellHeight,
            position.x + halfCellWidth, position.y - halfCellHeight,
            position.x + halfCellWidth, position.y + halfCellHeight
    };
}

Cell::~Cell()
{

}

void Cell::OnRender()
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;
    m_Shader->Bind();
    m_Shader->SetUniform1f("u_Alpha", m_Alpha);
    renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
}

void Cell::addNeighbors(std::vector<std::unique_ptr<Cell>> neighbors)
{
    m_Neighbors = std::move(neighbors);
}

void Cell::PrepareUpdate()
{
    int liveNeighbors = 0;
    for (const Cell& neighbor : *m_Neighbors) {
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
    m_Alpha = CellAlpha::dead;
}

void Cell::MakeLive() {
    SetState(CellState::Alive);
    m_Alpha = CellAlpha::live;
}

bool Cell::needsUpdate() {
    return m_CurrentState != m_NextState;
}
