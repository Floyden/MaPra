#include <SFML/Graphics.hpp>
#include "a_stern.h"
#include <memory>


class CoordinateVisualizer : public GraphVisualizer
{
public:
    CoordinateVisualizer(std::shared_ptr<CoordinateGraph> graph);
    ~CoordinateVisualizer();

    // Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
    void markVertex(VertexT vertex, VertexStatus status);

    // Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
    void markEdge(EdgeT e, EdgeStatus status);

    // Aktualisiere jegliche Daten eines Knotens.
    void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status);

    // Zeichne den aktuellen Zustand des Graphen.
    void draw();

    void reset();
    void wait();

    size_t windowWidth;
    size_t windowHeight;

    std::shared_ptr<CoordinateGraph> graph;
    std::vector<VertexStatus> state;
    std::map<EdgeT, EdgeStatus> estate;
    std::vector<double> cost;
    std::vector<double> estimate;
    bool enableText;

    sf::RenderWindow window;
    sf::Font font;
};
