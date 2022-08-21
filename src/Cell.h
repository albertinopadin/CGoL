#pragma once

#include <vector>
#include <memory>
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexBufferLayout.h"

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
    bool m_CurrentState;
    bool m_NextState;

public:
    static const unsigned int numVertices = 4;
    static const unsigned int positionComponentsPerVertex = 3;
    static const unsigned int colorComponentsPerVertex = 4;
    static const unsigned int componentsPerVertex = positionComponentsPerVertex + colorComponentsPerVertex;
    static const unsigned int numIndices = 6;

    bool alive;
    CellColor color;
    float alpha;
    float *vertices;
    unsigned int indices[6];

    Cell(CellPosition position, CellSize size, CellColor cellColor);
    ~Cell();

    void AddNeighbors(std::vector<std::unique_ptr<Cell>> neighbors);
    void PrepareUpdate();
    void Update();
    void SetState(bool state);
    void MakeDead();
    void MakeLive();
    bool needsUpdate();

private:
    static float* InitVertices(CellPosition position, CellSize size, CellColor color);
    void SetAlphaInVertices() const;
};