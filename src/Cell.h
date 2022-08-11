#pragma once

#include <vector>

enum CellState
{
    Alive = 1,
    Dead = 0
};

struct CellAlpha
{
    float live = 1.0f;
    float dead = 0.0f;
};

struct CellPosition
{
    float x, y, z;
};

struct CellColor
{
    float r, g, b, alpha;
};

class Cell
{
private:
    CellPosition m_Position;
    CellColor m_Color;
    std::vector<Cell> m_Neighbors;
    CellState m_CurrentState;
    CellState m_NextState;

public:
    bool alive;

    Cell(CellPosition position, CellColor color);
    ~Cell();

    void OnRender();
    void addNeighbors(std::vector<Cell> &neighbors);
    void PrepareUpdate();
    void Update();
    void SetState(CellState state);
    void MakeDead();
    void MakeLive();
    bool needsUpdate();
};