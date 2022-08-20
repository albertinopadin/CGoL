#include "Grid.h"
#include <omp.h>
#include <iostream>
#include "imgui/imgui.h"


Grid::Grid(int xCells, int yCells, WindowSize windowSize)
    : m_WindowSize(windowSize), m_XCells(xCells), m_YCells(yCells), m_Generation(0), m_Cells(std::vector<std::unique_ptr<Cell>>()),
      m_Proj(glm::ortho(0.0f, windowSize.width *  ((yCells * (m_CellHeight + m_CellSpacing)) / m_WindowSize.height),
                        0.0f, windowSize.height * ((yCells * (m_CellHeight + m_CellSpacing)) / m_WindowSize.height),
                        -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
    initGrid();
    setNeighborsForCellsInGrid();

    m_VertexBuffer = std::move(createBatchVertexBuffer());

    m_Layout.Push<float>(Cell::positionComponentsPerVertex);
    m_Layout.Push<float>(Cell::colorComponentsPerVertex);

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexArray->AddBuffer(*m_VertexBuffer, m_Layout);

    m_IndexBuffer = createBatchIndexBuffer();

    m_Shader = std::make_unique<Shader>("../resources/shaders/Basic.shader");
    m_Shader->Bind();

    std::cout << "m_Cells size: " << m_Cells.size() << std::endl;
    std::cout << "num vertices per cell: " << Cell::numVertices << std::endl;
    std::cout << "m_Cells size * num vertices: " << m_Cells.size() * Cell::numVertices << std::endl;
    std::cout << "num indices per cell: " << Cell::numIndices << std::endl;
    std::cout << "m_Cells size * num indices: " << m_Cells.size() * Cell::numIndices << std::endl;
}

Grid::~Grid()
{
}

void Grid::initGrid()
{
    for (int x = 0; x < m_XCells; x++) {
        for (int y = 0; y < m_YCells; y++) {
            CellPosition cellPosition = {(float)x * (m_CellWidth + m_CellSpacing), (float)y * (m_CellHeight + m_CellSpacing), 0.0f};
            CellSize cellSize = {m_CellWidth, m_CellHeight};
            CellColor cellColor = {0.0f, 1.0f, 0.0f, 1.0f};
            m_Cells.emplace_back(std::make_unique<Cell>(cellPosition, cellSize, cellColor));
        }
    }
}

std::unique_ptr<VertexBuffer> Grid::createBatchVertexBuffer()
{
    unsigned int batchVerticesComponentsCount = m_Cells.size() * Cell::numVertices * Cell::componentsPerVertex;
    auto *batchVerticesComponents = new float[batchVerticesComponentsCount];

    // Can't use the latest standard parallel for_each in Clang, unfortunately
#pragma omp parallel for shared(batchVerticesComponents) default(none)
    {
        for (int ci = 0; ci < m_Cells.size(); ci++) {
            for (int vi = 0; vi < Cell::numVertices * Cell::componentsPerVertex; vi++) {
                batchVerticesComponents[ci*Cell::numVertices*Cell::componentsPerVertex + vi] = m_Cells[ci]->vertices[vi];
            }
        }
    }

//    printBatchVertexBuffer(batchVerticesComponents);

    return std::make_unique<VertexBuffer>((float *)batchVerticesComponents, batchVerticesComponentsCount * sizeof(float));
}

void Grid::printBatchVertexBuffer(float *batchVerticesComponents)
{
    std::cout << "Batch Vertices: " << std::endl;
    std::cout << "Per Cell/Vertex:" << std::endl;
    for (unsigned int c = 0; c < m_Cells.size(); c++) {
        std::cout << "Vertices for cell: " << c << std::endl;
        for (unsigned int v = 0; v < Cell::numVertices; v++) {
            std::cout << "Vertex " << v << ": " << std::ends;
            for (unsigned int vc = 0; vc < Cell::componentsPerVertex; vc++) {
                unsigned int bcvidx = c*Cell::numVertices*Cell::componentsPerVertex + v*Cell::componentsPerVertex + vc;
                std::cout << batchVerticesComponents[bcvidx] << ", " << std::ends;
            }
            std::cout << std::endl;
        }
    }
}

std::unique_ptr<IndexBuffer> Grid::createBatchIndexBuffer()
{
    int currentIndex = 0;
    int cellIdx = 0;
    unsigned int batchIndicesCount = m_Cells.size() * Cell::numIndices;
    auto *batchIndices = new unsigned int[batchIndicesCount];
    for (std::unique_ptr<Cell> &cell : m_Cells) {
        for (unsigned int index : cell->indices) {
            batchIndices[currentIndex++] = index + (cellIdx * (Cell::positionComponentsPerVertex + 1));
        }
        ++cellIdx;
    }

//    printBatchIndices(batchIndicesCount, batchIndices);

    return std::make_unique<IndexBuffer>((unsigned int *)batchIndices, batchIndicesCount);
}

void Grid::printBatchIndices(unsigned int batchIndicesCount, unsigned int *batchIndices)
{
    std::cout << "Batch Indices: " << std::endl;
    for (int idx = 0; idx < batchIndicesCount; idx++) {
        std::cout << "Index " << idx << ": " << batchIndices[idx] << std::endl;
    }
}

void Grid::OnRender(Renderer &renderer)
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    m_VertexBuffer = std::move(createBatchVertexBuffer());
    m_VertexArray->AddBuffer(*m_VertexBuffer, m_Layout);
    m_Shader->Bind();
    float scaleFactor =  ((m_YCells * (m_CellHeight + m_CellSpacing)) / m_WindowSize.height);
    float xTranslateFactor = ((m_WindowSize.width * scaleFactor)/2) - ((m_XCells * (m_CellWidth + m_CellSpacing)) / 2);
    float yTranslateFactor = ((m_WindowSize.height * scaleFactor)/2) - ((m_YCells * (m_CellHeight + m_CellSpacing)) / 2);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xTranslateFactor, yTranslateFactor, 0.0f));
    glm::mat4 mvp = m_Proj * m_View * model;
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
}

void Grid::OnImGuiRender()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

unsigned int Grid::Update()
{
#pragma omp parallel for default(none)
    {
        for (std::unique_ptr<Cell> &cell: m_Cells) {
            cell->PrepareUpdate();
        }
    }

#pragma omp parallel for default(none)
    {
        for (std::unique_ptr<Cell> &cell: m_Cells) {
            cell->Update();
        }
    }

// Similar FPS as above:
//#pragma omp parallel for default(none)
//    {
//        for (int i = 0; i < m_Cells.size(); i++) {
//            m_Cells[i]->PrepareUpdate();
//        }
//    }
//
//#pragma omp parallel for default(none)
//    {
//        for (int i = 0; i < m_Cells.size(); i++) {
//            m_Cells[i]->Update();
//        }
//    }

// Slower than above:
//#pragma omp parallel for default(none)
//    {
//        for (int x = 0; x < m_XCells; x++) {
//            for (int y = 0; y < m_YCells; y++) {
//                m_Cells[x + y*m_XCells]->PrepareUpdate();
//            }
//        }
//    }
//
//#pragma omp parallel for default(none)
//    {
//        for (int x = 0; x < m_XCells; x++) {
//            for (int y = 0; y < m_YCells; y++) {
//                m_Cells[x + y*m_XCells]->Update();
//            }
//        }
//    }

// **** Tried collapse to parallelize both loops, slightly slower than above:
//#pragma omp parallel for default(none) collapse(2)
//    {
//        for (int x = 0; x < m_XCells; x++) {
//            for (int y = 0; y < m_YCells; y++) {
//                m_Cells[x + y*m_XCells]->PrepareUpdate();
//            }
//        }
//    }
//
//#pragma omp parallel for default(none) collapse(2)
//    {
//        for (int x = 0; x < m_XCells; x++) {
//            for (int y = 0; y < m_YCells; y++) {
//                m_Cells[x + y*m_XCells]->Update();
//            }
//        }
//    }

// *** VERY SLOW
//    for (int x = 0; x < m_XCells; x++) {
//#pragma omp parallel for shared(x) default(none)
//        for (int y = 0; y < m_YCells; y++) {
//            m_Cells[x + y*m_XCells]->PrepareUpdate();
//        }
//    }
//
//    for (int x = 0; x < m_XCells; x++) {
//#pragma omp parallel for shared(x) default(none)
//        for (int y = 0; y < m_YCells; y++) {
//            m_Cells[x + y*m_XCells]->Update();
//        }
//    }

    ++m_Generation;
    return m_Generation;
}

void Grid::Reset()
{
    for (std::unique_ptr<Cell> &cell: m_Cells) {
        cell->MakeDead();
    }

    m_Generation = 0;
}

void Grid::RandomState(float liveProbability)
{
    // TODO: How to get random number ???
    if (liveProbability > 0.99f) {
        makeAllLive();
    } else {
        if (liveProbability > 0.00f) {
            for (int x = 0; x < m_XCells; x++) {
                for (int y = 0; y < m_YCells; y++) {
                    int liveP = (int)(liveProbability * 100);
                    int r = std::rand() % 100;
                    if (r <= liveP) {
                        int idx = x + (y * m_XCells);
                        m_Cells[idx]->MakeLive();
                    }
                }
            }
        }
    }
}

void Grid::setNeighborsForCellsInGrid()
{
    for (int x = 0; x < m_XCells; x++) {
        for (int y = 0; y < m_YCells; y++) {
            int idx = x + (y * m_XCells);
            m_Cells[idx]->AddNeighbors(getCellNeighbors(x, y));
        }
    }
}

std::vector<std::unique_ptr<Cell>> Grid::getCellNeighbors(int x, int y) const {
    int leftX   = x - 1;
    int rightX  = x + 1;
    int topY    = y + 1;
    int bottomY = y - 1;

    std::vector<std::unique_ptr<Cell>> neighbors = std::vector<std::unique_ptr<Cell>>();

    // Left
    if (leftX > -1) {
        int idx = leftX + y*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Upper left
    if (leftX > -1 && topY < m_YCells) {
        int idx = leftX + topY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Upper
    if (topY < m_YCells) {
        int idx = x + topY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Upper right
    if (rightX < m_XCells && topY < m_YCells) {
        int idx = rightX + topY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Right
    if (rightX < m_XCells) {
        int idx = rightX + y*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Lower right
    if (rightX < m_XCells && bottomY > -1) {
        int idx = rightX + bottomY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Lower
    if (bottomY > -1) {
        int idx = x + bottomY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    // Lower left
    if (leftX > -1 && bottomY > -1) {
        int idx = leftX + bottomY*m_XCells;
        neighbors.emplace_back(m_Cells[idx].get());
    }

    return neighbors;
}

void Grid::makeAllLive()
{
    for (std::unique_ptr<Cell> &cell: m_Cells) {
        cell->MakeLive();
    }
}
