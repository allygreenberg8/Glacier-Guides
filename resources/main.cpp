#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "TerrainParser.h"
#include "TerrainRenderer.h"
#include "Graph.h"

// Window dimensions
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const float SCALE_X = 5.0f;
const float SCALE_Y = 5.0f;

// Function to normalize coordinates to fit the window
sf::Vector2f normalizeCoordinates(double longitude, double latitude,
                                  double minLon, double maxLon,
                                  double minLat, double maxLat)
{
    float x = (longitude - minLon) / (maxLon - minLon) * WINDOW_WIDTH;
    float y = (latitude - minLat) / (maxLat - minLat) * WINDOW_HEIGHT;
    return {x, y};
}

int main()
{
    // Setup window
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Glacier Guides - Trail Viewer");
    window.setFramerateLimit(60);
    
    // Grab data
    // Parse terrain
    TerrainParser parser;
    if (!parser.loadFile("map.osm"))
    {
        std::cerr << "Failed to load terrain data.\n";
        return 1;
    }

    const auto &points = parser.getPoints();
    const auto &trails = parser.getTrails();

    std::cout << "Number of points: " << points.size() << std::endl;
    std::cout << "Number of trails: " << trails.size() << std::endl;

    // Find bounds for coordinate normalization
    double minLon = std::numeric_limits<double>::max();
    double maxLon = std::numeric_limits<double>::lowest();
    double minLat = std::numeric_limits<double>::max();
    double maxLat = std::numeric_limits<double>::lowest();

    for (const auto &point : points)

    {
        minLon = std::min(minLon, point.longitude);
        maxLon = std::max(maxLon, point.longitude);
        minLat = std::min(minLat, point.latitude);
        maxLat = std::max(maxLat, point.latitude);
    }

    // Build graph
    Graph graph;
    for (const auto &point : points)

    {
        graph.addPin(point.id, point.latitude, point.longitude);
    }

    // Add edges for trails
    for (const auto &trail : trails)

    {
        const auto &pts = trail.trailPoints;
        for (size_t i = 0; i + 1 < pts.size(); ++i)

        {
            // Calculate distance as weight using Euclidean distance formula 
            const auto &p1 = points[i];
            const auto &p2 = points[i + 1];
            double weight = std::sqrt(
                std::pow(p2.longitude - p1.longitude, 2) +
                std::pow(p2.latitude - p1.latitude, 2));
            graph.addEdge(pts[i], pts[i + 1], weight);
        }
    }

    // Main rendering loop
    while (window.isOpen())

    {
        if (const std::optional<sf::Event> event = window.pollEvent())

        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            //TODO: add in keyboard controls
        }

        window.clear(sf::Color::Black);

        // Draw trails
        for (const auto &trail : trails)

        {
            const auto &ids = trail.trailPoints;
            if (ids.size() < 2)
                continue;

            sf::VertexArray lines(sf::PrimitiveType::LineStrip, ids.size());
            for (size_t i = 0; i < ids.size(); ++i)
            {
                const auto &p = points[i];
                sf::Vector2f pos = normalizeCoordinates(p.longitude, p.latitude, minLon, maxLon, minLat, maxLat);
                lines[i].position = pos;
                lines[i].color = sf::Color(200, 200, 200); // light gray
            }
            window.draw(lines);
        }

        window.display();
    }

    return 0;
}