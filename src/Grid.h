#pragma once

#include "Cell.h"
#include <vector>
#include <memory>

class Grid
{
private:
    float m_CellWidth = 0.9f;
    float m_CellHeight = 0.9f;
    float m_CellSpacing = 0.1f;
    int m_XCells, m_YCells;
    unsigned int m_Generation;
    std::vector<std::unique_ptr<Cell>> m_Cells;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    // TODO: figure out how to attach alpha uniforms to shader per cell

public:
    Grid(int xCells, int yCells);
    ~Grid();

    void OnRender(Renderer &renderer);
    unsigned int Update();
    void Reset();
    void RandomState(float liveProbability);

private:
    void initGrid();
    void setNeighborsForCellsInGrid();
    [[nodiscard]] std::vector<std::unique_ptr<Cell>> getCellNeighbors(int x, int y) const;
    void makeAllLive();
    std::unique_ptr<VertexBuffer> createBatchVertexBuffer();
    std::unique_ptr<IndexBuffer> createBatchIndexBuffer();
};