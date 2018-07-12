#include "CoordinateVisualizer.h"
#include <iostream>
#include <thread>
#include <chrono>

CoordinateVisualizer::CoordinateVisualizer(std::shared_ptr<CoordinateGraph> graph) :
    graph(graph),
    state(std::vector<VertexStatus>(graph->numVertices(), VertexStatus::UnknownVertex)),
    // estate(std::map<EdgeT, EdgeStatus>(graph->mEdges.size(), EdgeStatus::UnknownEdge)),
    cost(std::vector<double>(graph->numVertices(), infty)),
    estimate(std::vector<double>(graph->numVertices(), infty)),
    enableText(true),
    windowWidth(1280),
    windowHeight(720),
    window(sf::VideoMode(windowWidth, windowHeight), "CoordinateGraph")
{
    font.loadFromFile("teil_b/font/BebasNeue-Regular.ttf");
    for(auto& vert: graph->mEdges)
    {
        for(auto& edge: vert)
        {
            estate[edge] = EdgeStatus::UnknownEdge;
        }
    }
}

CoordinateVisualizer::~CoordinateVisualizer()
{
    window.close();
}

void CoordinateVisualizer::reset()
{
    for(auto& s: state)
        s = VertexStatus::UnknownVertex;
    for(auto& e: estate)
        e.second = EdgeStatus::UnknownEdge;
    for(auto& c: cost)
        c = infty;
    for(auto& e: estimate)
        e = infty;
}

void CoordinateVisualizer::wait()
{
    sf::Event event;
    while (window.pollEvent(event) || event.type != sf::Event::Closed)
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            enableText = !enableText;
            draw();
        }
    }
}


// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void CoordinateVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
    state[vertex] = status;
}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void CoordinateVisualizer::markEdge(EdgeT e, EdgeStatus status)
{
    estate[e] = status;
}

// Aktualisiere jegliche Daten eines Knotens.
void CoordinateVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{
    state[vertex] = status;
    this->cost[vertex] = cost;
    this->estimate[vertex] = estimate;
}

// Zeichne den aktuellen Zustand des Graphen.
void CoordinateVisualizer::draw()
{
    auto ScreenPos = [&](std::pair<double, double>& vert) -> std::pair<double, double> { return {
        ((vert.first - graph->minVal.first) * (windowWidth - 100)) / graph->maxXDist,
        ((vert.second - graph->minVal.second) * (windowHeight- 100)) / graph->maxYDist
    };};

    window.clear();

    double circleRad = 15;
    sf::CircleShape circle(circleRad);
    circle.setFillColor(sf::Color(255, 255, 255));

    //Draw lines and tris
    for(size_t i = 0; i < graph->numVertices(); i++)
    {
        auto pos = ScreenPos(graph->mCoordinates[i]);
        double xPos = pos.first, yPos = pos.second;

        sf::Vertex line[2];
        line[0].position = {static_cast<float>(xPos + circleRad), static_cast<float>(yPos + circleRad) };
        for(size_t j = 0; j < graph->mEdges[i].size(); j++)
        {
            auto dest = graph->mCoordinates[graph->mEdges[i][j].first];
            double xPos2 = ((dest.first - graph->minVal.first) * (windowWidth - 100)) / graph->maxXDist;
            double yPos2 = ((dest.second - graph->minVal.second) * (windowHeight- 100)) / graph->maxYDist;
            line[1].position = {static_cast<float>(xPos2 + circleRad), static_cast<float>(yPos2 + circleRad) };

            sf::ConvexShape tri;
            tri.setPointCount(3);
            tri.setPoint(0, sf::Vector2f(0, 1));
            tri.setPoint(1, sf::Vector2f(-1, -1));
            tri.setPoint(2, sf::Vector2f(1, -1));

            double xdiff = xPos - xPos2;
            double ydiff = yPos - yPos2;
            double deg = acos((xdiff) / sqrt(ydiff * ydiff + xdiff * xdiff));
            double neg = ydiff < 0 ? -1.0 : 1.0;

            tri.rotate(neg * deg * 180.0/3.14157 + 90.0);
            tri.setPosition(circleRad * cos(deg) * 1.5, neg * circleRad * sin(deg) * 1.5);

            tri.move(xPos2 + circleRad, yPos2 + circleRad);

            tri.setScale(5, 5);

            switch (estate[graph->mEdges[i][j]]) {
                case EdgeStatus::UnknownEdge:
                    line[0].color = {127, 127, 127 };
                    line[1].color = {127, 127, 127 };
                    tri.setFillColor(sf::Color(127, 127, 127 ));
                    break;
                case EdgeStatus::Visited:
                    line[0].color = {0, 255,255 };
                    line[1].color = {0, 255,255 };
                    tri.setFillColor(sf::Color(0, 255, 255));
                    break;
                case EdgeStatus::Active:
                    line[0].color = {0, 255, 0 };
                    line[1].color = {0, 255, 0 };
                    tri.setFillColor(sf::Color(0, 255, 0));
                    break;
                case EdgeStatus::Optimal:
                    line[0].color = {255, 255, 0 };
                    line[1].color = {255, 255, 0 };
                    tri.setFillColor(sf::Color(255, 255, 0));
                    break;
            };

            window.draw(line, 2, sf::Lines);
            window.draw(tri);

            if(enableText)
            {
                sf::Text text;
                text.setFont(font);
                text.setCharacterSize(14);
                text.setFillColor(sf::Color(255.0, 255.0, 0.0));
                text.setString(std::to_string(graph->mEdges[i][j].second).erase(5));

                text.setPosition(circleRad * cos(deg) * 3, neg * circleRad * sin(deg) * 3);
                text.move(xPos2 + circleRad, yPos2 + circleRad);

                window.draw(text);

            }
        }
    }

    //Draw circles
    for(size_t i = 0; i < graph->numVertices(); i++)
    {
        auto pos = ScreenPos(graph->mCoordinates[i]);
        double xPos = pos.first, yPos = pos.second;
        circle.setPosition(xPos, yPos);

        switch (state[i]) {
            case VertexStatus::UnknownVertex:
                circle.setFillColor(sf::Color(127, 127, 127));
                break;
            case VertexStatus::InQueue:
                circle.setFillColor(sf::Color(0, 0, 127));
                break;
            case VertexStatus::Active:
                circle.setFillColor(sf::Color(0, 255, 0));
                break;
            case VertexStatus::Done:
                circle.setFillColor(sf::Color(255, 255, 255));
                break;
            case VertexStatus::Destination:
                circle.setFillColor(sf::Color(255, 0, 0));
                break;

        };

        window.draw(circle);

        if(enableText)
        {
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(20);
            text.setFillColor(sf::Color(255.0, 255.0, 0.0));
            if(cost[i] != infty)
            {
                std::string txt = std::to_string(cost[i]).erase(5);
                if(estimate[i] != 0)
                    txt.append("+" + std::to_string(estimate[i]).erase(5));
                text.setString(txt);
            }
            else
                text.setString("?");
            text.setPosition(xPos + circleRad, yPos+ circleRad);

            window.draw(text);

        }
    }

    window.display();

    auto curr = std::chrono::system_clock::now();
    auto next = std::chrono::system_clock::now();
    while((next - curr) < std::chrono::milliseconds(WAIT_TIME))
    {
        next = std::chrono::system_clock::now();
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            enableText = !enableText;
    }


}
