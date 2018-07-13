#include "MazeVisualizer.h"
#include <iostream>
#include <chrono>
#include "a_stern.h"

MazeVisualizer::MazeVisualizer(
    size_t width,
    size_t height,
    const std::vector<CellType>& maze) :
        width(width),
        height(height),
        maze(maze),
        state(std::vector<VertexStatus>(maze.size(), VertexStatus::UnknownVertex)),
        cost(std::vector<double>(maze.size(), infty)),
        estimate(std::vector<double>(maze.size(), infty)),
        windowWidth(800),
        windowHeight(600),
        window(sf::VideoMode(windowWidth, windowHeight), "My window")
{
}

MazeVisualizer::~MazeVisualizer()
{
    // sf::Event event;
    // while (window.pollEvent(event) || event.type != sf::Event::Closed);
    window.close();
}

void MazeVisualizer::reset()
{
    for(auto& s: state)
        s = VertexStatus::UnknownVertex;
    for(auto& c: cost)
        c = infty;
    for(auto& e: estimate)
        e = infty;
}

void MazeVisualizer::wait()
{
    sf::Event event;
    while (window.pollEvent(event) || event.type != sf::Event::Closed);
}

// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void MazeVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
    state[vertex] = status;
}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void MazeVisualizer::markEdge(EdgeT e, EdgeStatus status)
{

}

// Aktualisiere jegliche Daten eines Knotens.
void MazeVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{
    this->state[vertex] = status;
    this->cost[vertex] = cost;
    this->estimate[vertex] = estimate;
}

// Zeichne den aktuellen Zustand des Graphen.
void MazeVisualizer::draw()
{
    const float xStep = static_cast<float>(windowWidth) / static_cast<float>(width);
    const float yStep = static_cast<float>(windowHeight) / static_cast<float>(height);

    window.clear(sf::Color::Black);
    sf::RectangleShape rect(sf::Vector2f(xStep,yStep));

    // std::cout << xStep << '\n';
    // std::cout << yStep << '\n';

    for(size_t y = 0; y < height; y++)
    {
        rect.setPosition(0, y * yStep);
        for(size_t x = 0; x < width; x++)
        {
            if(maze[x + y * height] == CellType::Wall)
                rect.setFillColor(sf::Color(0, 0, 0));
            else
            {
                switch (state[x + y * height]) {
                    case VertexStatus::UnknownVertex:
                        rect.setFillColor(sf::Color(127, 127, 127));
                        break;
                    case VertexStatus::InQueue:
                        rect.setFillColor(sf::Color(0, 0, 127));
                        break;
                    case VertexStatus::Active:
                        rect.setFillColor(sf::Color(0, 255, 0));
                        break;
                    case VertexStatus::Done:
                        rect.setFillColor(sf::Color(255, 255, 255));
                        break;
                    case VertexStatus::Destination:
                        rect.setFillColor(sf::Color(255, 0, 0));
                        break;
                };
            }

            window.draw(rect);
            rect.move(xStep, 0);
        }
    }
    window.display();

    auto curr = std::chrono::system_clock::now();
    auto next = std::chrono::system_clock::now();
    while((next - curr) < std::chrono::milliseconds(WAIT_TIME))
    {
        next = std::chrono::system_clock::now();
    }
    //
    // sf::CircleShape shape(100);
    // shape.setFillColor(sf::Color(100, 250, 50));
    // shape.setOutlineThickness(10); // creates an outline around the circle
    // shape.setOutlineColor(sf::Color(250, 150, 100)); // outline color is orange
    // shape.setOrigin(shape.getRadius(), shape.getRadius());
    // shape.setPosition(200,0); // positioning the circle 200 units right from the top left corner
    // window.draw(shape); // draws the shape, a green circle with an orange outline
    // shape.move(0,300); // moves the position 300 units down from the previous position (-> circle is drawn at (200,300))
    // window.draw(shape); // draws the shape, another green circle with an orange outline
    // sf::Vertex line[] =
    // {
    //     sf::Vertex(sf::Vector2f(shape.getPosition())),
    //     sf::Vertex(sf::Vector2f(800,600))
    // };
    // window.draw(line, 2, sf::Lines); //draws the line
    //
}
