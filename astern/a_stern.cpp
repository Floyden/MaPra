
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
        DistanceGraph(vertcount), mEdges(edgy), mCoordinates(cd), maxDist(-1.0)
    {
        for(auto& cd: mCoordinates)
        {
            double distance = std::sqrt(cd.first * cd.first + cd.second * cd.second);
            if(distance > maxDist)
                maxDist = distance;
        }
    };

    const NeighborT& getNeighbors( VertexT v) const override;

    CostT estimatedCost( VertexT from, VertexT to) const override;

    CostT cost( VertexT from, VertexT to) const override;

    void addEdge(VertexT from, VertexT to, CostT cost);
    void setCoordinates(VertexT v, double x, double y);

    std::vector<NeighborT> mEdges;
    std::vector<std::pair<double, double>> mCoordinates;
    double maxDist;
};

CostT CoordinateGraph::estimatedCost( VertexT from, VertexT to) const
{
    auto fromCoord = mCoordinates[from];
    auto toCoord = mCoordinates[to];

    double distance = std::sqrt(
        (fromCoord.first - toCoord.first) *  (fromCoord.first - toCoord.first) +
        (fromCoord.second - toCoord.second) *  (fromCoord.second - toCoord.second)
    ) / maxDist;
    return distance;
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
        DistanceGraph(width * height), width(width), height(height)
    {
        for (size_t v = 0; v < cd.size(); v++)
        {
            edges.push_back({cd[v], NeighborT()});
            if(cd[v] == CellType::Wall)
                continue;

            if(v >= width && cd[v - width] != CellType::Wall)
                edges.back().second.push_back({v - width, 1});
            if(v % width && cd[v - 1] != CellType::Wall)
                edges.back().second.push_back({v - 1, 1});
            if((v + 1) % width && cd[v + 1] != CellType::Wall)
                edges.back().second.push_back({v + 1, 1});
            if(v + width < width * height && cd[v + width] != CellType::Wall)
                edges.back().second.push_back({v + width, 1});

        }
    };

    const NeighborT& getNeighbors( VertexT v) const override;

    CostT estimatedCost( VertexT from, VertexT to) const override;

    CostT cost( VertexT from, VertexT to) const override;

    void addEdge(VertexT from, VertexT to, CostT cost);
    void setCoordinates(VertexT v, double x, double y);

    size_t width;
    size_t height;
    std::vector<std::pair<CellType, NeighborT>> edges;
};

const LabyrinthGraph::NeighborT& LabyrinthGraph::getNeighbors(VertexT v) const
{
    return edges[v].second;
}


CostT LabyrinthGraph::estimatedCost( VertexT from, VertexT to) const
{
    double x = abs((int)(from % width - to % width));
    double y = abs((int)(from / width - to / width));
    return x + y;
}

CostT LabyrinthGraph::cost( VertexT from, VertexT to) const
{
    auto v = getNeighbors(from);
    for(auto& e: v)
        if(e.first == to)
            return e.second;

    return infty;
}


std::istream& operator>>(std::istream& is, CellType& ct)
{
    char ch;
    is >> ch;
    switch (ch) {
        case '#': ct = CellType::Wall; break;
        case '.': ct = CellType::Ground; break;
        default: break;
    }
    return is;
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
    std::set<VertexT> known = {start};
    std::set<VertexT> finished;

    std::vector<double> score(g.numVertices(), infty);
    score[start] = 0.0;

    std::vector<double> est(g.numVertices(), infty);
    score[start] = g.estimatedCost(start, ziel);

    std::vector<VertexT> path(g.numVertices(), undefinedVertex);

    while (!known.empty())
    {
        VertexT current = *known.begin();
        for(auto& x: known)
            if(est[x] < est[current])
                current = x;
        if(current == ziel)
        {
            //done
            weg.clear();
            VertexT tmp = current;
            while(tmp != start)
            {
                weg.push_front(tmp);
                tmp = path[tmp];
            }
            weg.push_front(start);

            return true;
        }

        known.erase(current);
        finished.insert(current);

        auto neighbors = g.getNeighbors(current);
        for(auto& _nb : neighbors)
        {
            // std::cout << current << " NB " <<_nb.first << '\n';
            auto nb = _nb.first;
            if(finished.find(nb) != finished.end())
                continue;
            known.insert(nb);
            if(score[current] + g.cost(current, nb) >= score[nb])
                continue;

            path[nb] = current;
            score[nb] = score[current] + g.cost(current, nb);
            est[nb] = score[nb] + g.estimatedCost(nb, ziel);
        }
    }
    weg.clear();
    weg.push_front(start);
    return false; // Kein Weg gefunden.
}

void printG(std::vector<CellType>& g, size_t w, size_t h)
{
    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < w; j++)
        {
            switch (g[i * w + j]) {
                case CellType::Wall: std::cout << '#'; break;
                case CellType::Ground: std::cout << '.'; break;
                case CellType::Start: std::cout << 'S'; break;
                case CellType::Destination: std::cout << 'D'; break;
            }
        }
        std::cout << '\n';
    }
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
        case 1: fileName = "daten/Graph1.dat"; break;
        case 2: fileName = "daten/Graph2.dat"; break;
        case 3: fileName = "daten/Graph3.dat"; break;
        case 4: fileName = "daten/Graph4.dat"; break;
        case 5: fileName = "daten/Maze1.dat"; break;
        case 6: fileName = "daten/Maze2.dat"; break;
        case 7: fileName = "daten/Maze3.dat"; break;
        case 8: fileName = "daten/Maze4.dat"; break;
        case 9: fileName = "daten/Maze5.dat"; break;
        default: break;
    }
    std::ifstream stream(fileName);


    DistanceGraph* graph;
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
        size_t width;
        size_t height;
        stream >> width;
        stream >> height;
        std::vector<CellType> edgy;

        for (size_t i = 0; i < width * height; i++)
        {
            CellType ct;
            stream >> ct;
            edgy.emplace_back(ct);
        }
        graph = new LabyrinthGraph(width, height, edgy);
    }
    else
    {
        for (size_t i = 0; i < 10; i++)
        {
            size_t width = 20;
            size_t height = 20;
            auto edgy = ErzeugeLabyrinth(width, height, i);
            // printG(edgy, width, height);
            graph = new LabyrinthGraph(width, height, edgy);

            VertexT start = undefinedVertex;
            VertexT end = undefinedVertex;
            for (size_t i = 0; i < width * height; i++) {
                if(start != undefinedVertex && end != undefinedVertex)
                break;
                if(edgy[i] == CellType::Start)
                start = i;
                if(edgy[i] == CellType::Destination)
                end = i;
            }
            TextVisualizer __a;
            std::list<VertexT> weg;
            std::cout << "calc" << '\n';
            A_star(*graph, __a, start, end, weg);
            std::cout << "finished" << '\n';
            // std::cout << "Start: "  << start<< ", ENd: "  << end << '\n';
            // for (auto& x: weg) {
            //     std::cout << x << '\n';
            // }
            PruefeWeg(bspl, weg);
            PruefeHeuristik(*graph);
            std::cout << "finished check" << '\n';
        }

    }

    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik

    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    if(bspl <= 4)
    {
        TextVisualizer __a;
        for (size_t i = 0; i < graph->numVertices(); i++)
        {
            std::vector<CostT> cost;
            Dijkstra(*graph, __a, i, cost);
            PruefeDijkstra(bspl, i, cost);
        }
        for (size_t i = 0; i < graph->numVertices(); i++)
        {

            for (size_t j = 0; j < graph->numVertices(); j++)
            {
                std::list<VertexT> weg;
                A_star(*graph, __a, i, j, weg);
                PruefeWeg(bspl, weg);
            }
        }
    }
    else if(bspl <= 9)
    {
        auto szpair = StartZielPaare(bspl);
        TextVisualizer __a;
        for(auto& p: szpair)
        {
            std::list<VertexT> weg;
            A_star(*graph, __a, p.first, p.second, weg);
            PruefeWeg(bspl, weg);
        }
    }

    PruefeHeuristik(*graph);

    if(graph)
        delete(graph);
    return 0;
}
