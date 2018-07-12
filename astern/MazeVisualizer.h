#include <SFML/Graphics.hpp>
#include "unit.h"

class MazeVisualizer : public GraphVisualizer
{
public:
    MazeVisualizer(size_t width, size_t height, const std::vector<CellType>& maze);
    ~MazeVisualizer();

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

    size_t width;
    size_t height;
    size_t windowWidth;
    size_t windowHeight;
    std::vector<CellType> maze;
    std::vector<VertexStatus> state;
    std::vector<double> cost;
    std::vector<double> estimate;

    sf::RenderWindow window;
    // sf::Font font;
};
