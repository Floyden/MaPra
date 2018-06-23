
#include "text_visualizer.h"
#include "unit.h"
#include <set>

#include <cmath>
#include <limits>
#include <fstream>
#include <iterator>
#include <map>

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph {
public:
    CoordinateGraph(size_t vertcount, std::vector<NeighborT> edgy, std::vector<std::pair<double, double>> cd) :
        DistanceGraph(vertcount), mEdges(edgy), mCoordinates(cd) {};

    const NeighborT& getNeighbors( VertexT v) const override;

    CostT estimatedCost( VertexT from, VertexT to) const override;

    CostT cost( VertexT from, VertexT to) const override;

    void addEdge(VertexT from, VertexT to, CostT cost);
    void setCoordinates(VertexT v, double x, double y);

    std::vector<NeighborT> mEdges;
    std::vector<std::pair<double, double>> mCoordinates;
};

CostT CoordinateGraph::estimatedCost( VertexT from, VertexT to) const
{
    return 0;
}

CostT CoordinateGraph::cost( VertexT from, VertexT to) const
{
    auto v = getNeighbors(from);
    for(auto& e: v)
        if(e.first == to)
            return e.second;

    return infty;
}

const CoordinateGraph::NeighborT& CoordinateGraph::getNeighbors(VertexT v) const
{
    return mEdges[v];
}

class LabyrinthGraph : public DistanceGraph {
public:
    LabyrinthGraph(size_t width, size_t height, std::vector<CellType> cd) :
        DistanceGraph(width * height), cells(cd) {};

    const NeighborT& getNeighbors( VertexT v) const override;

    CostT estimatedCost( VertexT from, VertexT to) const override;

    CostT cost( VertexT from, VertexT to) const override;

    void addEdge(VertexT from, VertexT to, CostT cost);
    void setCoordinates(VertexT v, double x, double y);

    size_t width;
    size_t height;
    std::vector<CellType> cells;
};

const NeighborT& getNeighbors( VertexT v) const
{
}



void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart) {
    kostenZumStart.resize(g.numVertices());
    for(auto& e: kostenZumStart)
        e = infty;
    kostenZumStart[start] = 0;

    auto& neighbors = g.getNeighbors(start);
    for(auto neighbor: neighbors)
            kostenZumStart[neighbor.first] = neighbor.second;

    std::set<VertexT> unvisited;
    for(size_t i = 0; i < g.numVertices(); i++)
    {
        if(i == start)
            continue;
        unvisited.insert(i);
    }

    while(!unvisited.empty())
    {
        VertexT current = undefinedVertex;
        CostT best_dist = infty;

        for (auto& vert: unvisited) {
            if(best_dist > kostenZumStart[vert])
            {
                current = vert;
                best_dist = kostenZumStart[vert];
            }
        }

        unvisited.erase(current);
        if(current == undefinedVertex)
            break;

        auto& neighbors = g.getNeighbors(current);
        for(auto neighbor: neighbors)
        {
            if(kostenZumStart[neighbor.first] >
                    kostenZumStart[current] + neighbor.second)
                kostenZumStart[neighbor.first] = kostenZumStart[current] + neighbor.second;
        }

    }
}

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    // ...
    return false; // Kein Weg gefunden.
}

int main()
{
  // Frage Beispielnummer vom User ab
    int bspl = -1;
    while(!(bspl >= 1 && bspl <= 10))
    {
        std::cout << "Beispiel[1-10]: ";
        std::cin >> bspl;
        if(!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }

    std::string fileName;
    switch (bspl) {
        case 1: fileName = "Graph1.dat"; break;
        case 2: fileName = "Graph2.dat"; break;
        case 3: fileName = "Graph3.dat"; break;
        case 4: fileName = "Graph4.dat"; break;
        case 5: fileName = "Maze1.dat"; break;
        case 6: fileName = "Maze2.dat"; break;
        case 7: fileName = "Maze3.dat"; break;
        case 8: fileName = "Maze4.dat"; break;
        case 9: fileName = "Maze5.dat"; break;
        default: break;
    }
    std::ifstream stream(fileName);

    CoordinateGraph* graph;
    if(bspl <= 4)
    {

        size_t vert;
        size_t edges;
        stream >> vert;
        stream >> edges;
        std::vector<DistanceGraph::NeighborT> edgy(vert);
        std::vector<std::pair<double, double>> crd;

        for (size_t i = 0; i < edges; i++)
        {
            VertexT from;
            VertexT to;
            CostT weight;
            stream >> from >> to >> weight;
            edgy[from].emplace_back(to, weight);
        }

        for (size_t i = 0; i < vert; i++)
        {
            double x, y;
            stream >> x >> y;
            crd.emplace_back(x,y);
        }

        graph = new CoordinateGraph(vert, edgy, crd);
    }
    else if( bspl <= 9)
    {
    }



    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik

    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    if(bspl <= 4)
    {
        for (size_t i = 0; i < graph->numVertices(); i++)
        {
            std::vector<CostT> cost;
            TextVisualizer __a;
            Dijkstra(*graph, __a, i, cost);
            PruefeDijkstra(bspl, i, cost);
        }
    }

    if(graph)
        delete(graph);
    return 0;
}
