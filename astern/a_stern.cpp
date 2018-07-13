
#include "text_visualizer.h"
#include "MazeVisualizer.h"
#include "CoordinateVisualizer.h"
#include "a_stern.h"
#include <set>

#include <memory>
#include <limits>
#include <fstream>
#include <iterator>
#include <map>
#include <thread>
#include <chrono>

CostT CoordinateGraph::estimatedCost( VertexT from, VertexT to) const
{
    auto fromCoord = mCoordinates[from];
    auto toCoord = mCoordinates[to];

    double distance = std::sqrt(
        (fromCoord.first - toCoord.first) *  (fromCoord.first - toCoord.first) +
        (fromCoord.second - toCoord.second) *  (fromCoord.second - toCoord.second)
    ) / maxXDist;

    double distance2 = infty;
    for(auto& e: mEdges[from])
        if(e.first == to)
            distance2 = e.second;
    return distance < distance2 ? distance : distance2;
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

void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart)
{
    std::map<VertexT, VertexT> parents;

    kostenZumStart.resize(g.numVertices());
    for(auto& e: kostenZumStart)
        e = infty;
    kostenZumStart[start] = 0;

    v.updateVertex(start, 0.0, 0.0, start, VertexStatus::Active);
    auto& neighbors = g.getNeighbors(start);
    for(auto neighbor: neighbors)
    {
        v.markEdge({start, neighbor.first}, EdgeStatus::Active);
        v.draw();
        kostenZumStart[neighbor.first] = neighbor.second;
        parents[neighbor.first] = start;
        v.markEdge({start, neighbor.first}, EdgeStatus::Visited);
    }
    v.updateVertex(start, 0.0, 0.0, start, VertexStatus::Done);
    v.draw();

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

        v.updateVertex(current, kostenZumStart[current], 0.0, current, VertexStatus::Active);
        v.markEdge({parents[current], current}, EdgeStatus::Optimal);

        auto& neighbors = g.getNeighbors(current);
        for(auto neighbor: neighbors)
        {
            v.markEdge({current, neighbor.first}, EdgeStatus::Active);
            v.draw();
            if(kostenZumStart[neighbor.first] >
                    kostenZumStart[current] + neighbor.second)
            {
                kostenZumStart[neighbor.first] = kostenZumStart[current] + neighbor.second;
                parents[neighbor.first] = current;
                v.updateVertex(neighbor.first, kostenZumStart[neighbor.first], 0, parents[neighbor.first], VertexStatus::InQueue);
            }
            v.markEdge({current, neighbor.first}, EdgeStatus::Visited);
        }
        v.updateVertex(current, kostenZumStart[current], 0.0, current, VertexStatus::Done);
        v.draw();
    }
    v.draw();
}


bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    std::set<VertexT> known = {start};
    std::set<VertexT> finished;

    std::vector<double> score(g.numVertices(), infty);
    score[start] = 0.0;

    std::vector<double> est(g.numVertices(), infty);
    est[start] = 0.0;
    est[ziel] = g.estimatedCost(start, ziel);

    v.updateVertex(start, 0.0, score[start], start, VertexStatus::Active);
    v.updateVertex(ziel, infty, est[ziel], ziel, VertexStatus::Destination);
    v.draw();
    std::vector<VertexT> path(g.numVertices(), undefinedVertex);

    while (!known.empty())
    {
        VertexT current = *known.begin();
        for(auto& x: known)
            if(est[x] < est[current])
                current = x;

        if(current == ziel)
        {
            v.markVertex(start, VertexStatus::Active);
            //done
            weg.clear();
            VertexT tmp = current;
            while(tmp != start)
            {
                v.markVertex(tmp, VertexStatus::Active);
                v.markEdge({path[tmp], tmp}, EdgeStatus::Optimal);
                weg.push_front(tmp);
                tmp = path[tmp];
            }
            weg.push_front(start);
            v.markVertex(ziel, VertexStatus::Destination);

            v.draw();
            return true;
        }
        v.updateVertex(current, score[current], est[current], path[current], VertexStatus::Active);

        known.erase(current);
        finished.insert(current);

        auto neighbors = g.getNeighbors(current);
        for(auto& _nb : neighbors)
        {
            auto nb = _nb.first;
            v.markEdge({current, _nb.first}, EdgeStatus::Active);
            v.draw();
            v.markEdge({current, _nb.first}, EdgeStatus::Visited);
            if(finished.find(nb) != finished.end())
                continue;
            known.insert(nb);
            if(score[current] + g.cost(current, nb) >= score[nb])
                continue;

            v.updateVertex(nb, score[nb], est[nb], path[nb], VertexStatus::InQueue);

            path[nb] = current;
            score[nb] = score[current] + g.cost(current, nb);
            est[nb] = //score[nb] +
                    g.estimatedCost(nb, ziel);

        }
        v.draw();
        v.markVertex(current, VertexStatus::Done);
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


    std::shared_ptr<DistanceGraph> graph;
    std::unique_ptr<GraphVisualizer> visualizer;
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

        {
            auto _graph = std::shared_ptr<CoordinateGraph>(new CoordinateGraph(vert, edgy, crd));
            graph = _graph;
            visualizer = std::unique_ptr<CoordinateVisualizer>(
                new CoordinateVisualizer(_graph));
        }
        CoordinateVisualizer* fnaa = reinterpret_cast<CoordinateVisualizer*>(visualizer.get());

        for (size_t i = 0; i < graph->numVertices(); i++)
        {
            std::vector<CostT> cost;
            Dijkstra(*graph, *visualizer, i, cost);
            fnaa->wait();
            fnaa->reset();
            // if(visualizer)
            //     delete visualizer;
            // PruefeDijkstra(bspl, i, cost);
        }
        for (size_t i = 0; i < graph->numVertices(); i++)
        {

            for (size_t j = 0; j < graph->numVertices(); j++)
            {
                std::list<VertexT> weg;
                A_star(*graph, *visualizer, i, j, weg);
                fnaa->wait();
                fnaa->reset();
                // PruefeWeg(bspl, weg);
            }
        }
        // delete visualizer;
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
        graph = std::unique_ptr<LabyrinthGraph>(new LabyrinthGraph(width, height, edgy));
        visualizer = std::unique_ptr<MazeVisualizer>(new MazeVisualizer(width, height, edgy));
        MazeVisualizer* fnaa = reinterpret_cast<MazeVisualizer*>(visualizer.get());

        auto szpair = StartZielPaare(bspl);
        for(auto& p: szpair)
        {
            std::list<VertexT> weg;
            A_star(*graph, *visualizer, p.first, p.second, weg);
            fnaa->wait();
            fnaa->reset();
            // PruefeWeg(bspl, weg);
        }
    }
    else
    {
        for (size_t i = 0; i < 10; i++)
        {
            size_t width = 50;
            size_t height = 50;
            auto edgy = ErzeugeLabyrinth(width, height, i);
            // printG(edgy, width, height);
            graph = std::unique_ptr<LabyrinthGraph>(new LabyrinthGraph(width, height, edgy));

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
            std::list<VertexT> weg;
            visualizer = std::unique_ptr<MazeVisualizer>(new MazeVisualizer(width, height, edgy));
            MazeVisualizer* fnaa = reinterpret_cast<MazeVisualizer*>(visualizer.get());
            A_star(*graph, *visualizer, start, end, weg);
            fnaa->wait();
            fnaa->reset();

            PruefeHeuristik(*graph);
            std::cout << "finished check" << '\n';
        }

    }

    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik

    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg

    PruefeHeuristik(*graph);

    // if(graph)
    //     delete graph;
    // if(visualizer)
    //     delete visualizer;
    return 0;
}
