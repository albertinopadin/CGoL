#pragma once

#include "Cell.h"
#include <vector>
#include <memory>

struct WindowSize {
    int width;
    int height;
};

class Grid
{
private:
    WindowSize m_WindowSize;
    float m_CellWidth = 9.0f;
    float m_CellHeight = 9.0f;
    float m_CellSpacing = 1.0f;
    int m_XCells, m_YCells;
    unsigned int m_Generation;
    std::vector<std::unique_ptr<Cell>> m_Cells;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    VertexBufferLayout m_Layout;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    glm::mat4 m_Proj, m_View;

public:
    Grid(int xCells, int yCells, WindowSize windowSize);
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
    void printBatchVertexBuffer(float *batchVerticesComponents);
    static void printBatchIndices(unsigned int batchIndicesCount, unsigned int *batchIndices);
};