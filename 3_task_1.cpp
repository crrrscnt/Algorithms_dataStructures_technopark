#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <queue>
#include <deque>
#include <unordered_set>

struct IGraph
{
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

// Список смежности
struct ListGraph : public IGraph
{
public:
    ListGraph(int size)
        : adjacencyLists(size)
    {
    }

    ListGraph(const IGraph &graph)
    {
        adjacencyLists.resize(graph.VerticesCount());
        for (int i = 0; i < graph.VerticesCount(); ++i)
        {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph()
    {
    }

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

    int VerticesCount() const override
    {
        return (int)adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); ++from)
        {
            for (int to : adjacencyLists[from])
            {
                if (to == vertex)
                    prevVertices.push_back(from);
            }
        }
        return prevVertices;
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};

// Матрица смежности
struct MatrixGraph : public IGraph
{
public:
    MatrixGraph(int size)
        : adjMatrix(size) {}

    MatrixGraph(const IGraph &graph) // Конструктор копирования
    {
        adjMatrix.resize(graph.VerticesCount());
        for (int i = 0; i < graph.VerticesCount(); ++i)
        {
            adjMatrix[i] = graph.GetNextVertices(i);
        }
    }

    ~MatrixGraph()
    {
    }

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjMatrix.size());
        assert(0 <= to && to < adjMatrix.size());
        adjMatrix[from][to] = 1;
    };

    int VerticesCount() const override
    {
        // std::cout <<"\nРАЗМЕР МАТРИЦЫ СМЕЖНОСТИ\n" <<  adjMatrix.size() << "\n";
        return (int)adjMatrix.size();
    };

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjMatrix.size());
        // std::vector<int> next;
        // for (const auto &edge : adjMatrix) {
        //     if (edge.first == vertex) {
        //         next.push_back(edge.second);
        //     }
        // }
        return adjMatrix[vertex];
    };
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjMatrix.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjMatrix.size(); ++from)
        {
            for (int to : adjMatrix[from])
            {
                if (to == vertex)
                    prevVertices.push_back(from);
            }
        }
        return prevVertices;
    };

private:
    std::vector<std::vector<int>> adjMatrix;
};

// Массив ребер
struct ArcGraph : public IGraph
{
public:
    ArcGraph(int size)
        : arrVert(size), verticesCount(size) {};

    ArcGraph(const IGraph &graph)
    {
        for (int from = 0; from < graph.VerticesCount(); ++from)
        {
            for (int to : graph.GetNextVertices(from))
            {
                arrVert.emplace_back(from, to);
            }
        }
        verticesCount = graph.VerticesCount();
    }

    ~ArcGraph()
    {
    }

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < arrVert.size());
        assert(0 <= to && to < arrVert.size());
        arrVert.emplace_back(from, to);
    };

    int VerticesCount() const override
    {
        // std::cout <<"\nРАЗМЕР массива ребер\n" <<  arrVert.size() << "\n";
        return verticesCount;
    };

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < VerticesCount());
        std::vector<int> next;
        for (const auto &edge : arrVert)
        {
            if (edge.first == vertex)
            {
                next.push_back(edge.second);
            }
        }
        return next;
    };
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < arrVert.size());
        std::vector<int> prev;
        for (const auto &edge : arrVert)
        {
            if (edge.second == vertex)
            {
                prev.push_back(edge.first);
            }
        }
        return prev;
    };

private:
    std::vector<std::pair<int, int>> arrVert;
    int verticesCount;
};

// Массив хэш-таблиц
struct SetGraph : public IGraph
{
public:
    SetGraph(int size)
        : adjSet(size) {};

    SetGraph(const IGraph &graph)
    {
        vertexCount = graph.VerticesCount();
        adjSet.resize(vertexCount);
        for (int from = 0; from < vertexCount; ++from)
        {
            for (int to : graph.GetNextVertices(from))
            {
                adjSet[from].insert(to);
            }
        }
    }

    ~SetGraph()
    {
    }

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < VerticesCount());
        assert(0 <= to && to < VerticesCount());
        adjSet[from].insert(to);
    };

    int VerticesCount() const override
    {
        // std::cout <<"\nРАЗМЕР массива хэш-таблиц\n" <<  adjSet.size() << "\n";
        return vertexCount;
    };

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjSet.size());
        return std::vector<int>(adjSet[vertex].begin(), adjSet[vertex].end());
    };
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < VerticesCount());
        std::vector<int> prev;
        for (int from = 0; from < adjSet.size(); ++from)
        {
            if (adjSet[from].count(vertex) > 0)
            {
                prev.push_back(from);
            }
        }
        return prev;
    };

private:
    std::vector<std::unordered_set<int>> adjSet;
    int vertexCount;
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);

        for (int nextVertex : graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            BFS(graph, i, visited, func);
        }
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex : graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
        {
            DFS(graph, nextVertex, visited, func);
        }
    }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            DFS(graph, i, visited, func);
        }
    }
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;

    for (int nextVertex : graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
        {
            topologicalSortInternal(graph, nextVertex, visited, sorted);
        }
    }

    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            topologicalSortInternal(graph, i, visited, sorted);
        }
    }

    return sorted;
}

int main(int argc, const char *argv[])
{
    ListGraph listGraph(7);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);

    std::cout << "Список смежности (BFS, DFS):\n";

    mainBFS(listGraph, [](int vertex)
            { std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(listGraph, [](int vertex)
            { std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "\nТопологическая сортировка:\n";
    for (int vertex : topologicalSort(listGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    // Нужно продемонстрировать работу конструктора копирования, проинициализировав
    // графы разных классов друг от друга. Далее, показать, что вершины и ребра
    // успешно скопированы: число вершин осталось прежним, произвести BFS и DFS.
    MatrixGraph matrixGraph(listGraph);

    // Проверка BFS/DFS
    std::cout << "\nМатрица смежности (BFS, DFS)\n";
    mainBFS(matrixGraph, [](int vertex)
            { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(matrixGraph, [](int vertex)
            { std::cout << vertex << " "; });
    std::cout << std::endl;

    ArcGraph arcGraph(matrixGraph);

    std::cout << "\nМассив ребер (BFS, DFS)\n";
    mainBFS(arcGraph, [](int vertex)
            { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(arcGraph, [](int vertex)
            { std::cout << vertex << " "; });
    std::cout << std::endl;

    SetGraph setGraph(arcGraph);
    std::cout << "\nМассив хэш-таблиц (BFS, DFS)\n";
    mainBFS(arcGraph, [](int vertex)
            { std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(arcGraph, [](int vertex)
            { std::cout << vertex << " "; });
    std::cout << std::endl;

    return 0;
}