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
    std::vector<std::unique_ptr<Cell>> m_Neighbors;
    CellState m_CurrentState;
    CellState m_NextState;

public:
    bool alive;
    CellColor color;
    float alpha;
    float *vertices;
    static const unsigned int numVertices = 4;
    static const unsigned int positionComponentsPerVertex = 3;
    unsigned int indices[6];
    static const unsigned int numIndices = 6;

    Cell(CellPosition position, CellSize size, CellColor cellColor);
    ~Cell();

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