#pragma once
#include "unit.h"
#include <cmath>

#define WAIT_TIME 500

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph {
public:
    CoordinateGraph(size_t vertcount, std::vector<NeighborT> edgy, std::vector<std::pair<double, double>> cd) :
        DistanceGraph(vertcount), mEdges(edgy), mCoordinates(cd), maxDist(-1.0)
    {
        maxVal = {0.0, 0.0};
        minVal = { infty, infty};
        for(auto& cd: mCoordinates)
        {
            if(cd.first > maxVal.first)
                maxVal.first = cd.first;
            if(cd.second > maxVal.second)
                maxVal.second = cd.second;

            if(cd.first < minVal.first)
                minVal.first = cd.first;
            if(cd.second < minVal.second)
                minVal.second = cd.second;
        }
        maxXDist = maxVal.first - minVal.first;
        maxYDist = maxVal.second - minVal.second;
        maxDist = std::sqrt(maxXDist * maxXDist + maxYDist * maxYDist);
    };

    const NeighborT& getNeighbors( VertexT v) const override;

    CostT estimatedCost( VertexT from, VertexT to) const override;

    CostT cost( VertexT from, VertexT to) const override;

    void addEdge(VertexT from, VertexT to, CostT cost);
    void setCoordinates(VertexT v, double x, double y);

    std::vector<NeighborT> mEdges;
    std::vector<std::pair<double, double>> mCoordinates;
    double maxDist;
    double maxXDist;
    double maxYDist;

    std::pair<double, double> maxVal;
    std::pair<double, double> minVal;
};

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
