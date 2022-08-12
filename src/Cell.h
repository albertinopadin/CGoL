#pragma once

#include <vector>
#include <memory>
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexBufferLayout.h"

enum CellState
{
    Alive = 1,
    Dead = 0
};

struct CellAlpha
{
    constexpr const static float live = 1.0f;
    constexpr const static float dead = 0.0f;
};

struct CellPosition
{
    float x, y, z;
};

struct CellSize
{
    float width, height;
};

struct CellColor
{
    float r, g, b, alpha;
};

class Cell
{
private:
    float m_Alpha;
    std::vector<std::unique_ptr<Cell>> m_Neighbors;
    CellState m_CurrentState;
    CellState m_NextState;
    float *m_Vertices;
    unsigned int m_Indices[6];
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;

public:
    bool alive;

    Cell(CellPosition position, CellSize size, CellColor color);
    ~Cell();

    void OnRender();
    void addNeighbors(std::vector<std::unique_ptr<Cell>> neighbors);
    void PrepareUpdate();
    void Update();
    void SetState(CellState state);
    void MakeDead();
    void MakeLive();
    bool needsUpdate();

private:
    static float* initVertices(CellPosition position, CellSize size) ;
};