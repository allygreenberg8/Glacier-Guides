#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "TerrainParser.h"
#include "TerrainRenderer.h"
#include "Graph.h"

const float SCALE_X = 5.0f;
const float SCALE_Y = 5.0f;

int main(){
    // Setup window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Glacier Guides - Trail Viewer");
    window.setFramerateLimit(60);

    // Parse terrain
    TerrainParser parser;
    if (!parser.loadFile("map.osm")) {
        std::cerr << "Failed to load terrain data.\n";
        return 1;
    }

    const auto& points = parser.getPoints();       
    const auto& trails = parser.getTrails();   

    // not sure if parser is working right or if my file is right
    std::cout << "Number of points: " << points.size() << std::endl;
    std::cout << "Number of trails: " << trails.size() << std::endl;

    //Build graph 
    Graph graph;
    
    /*
    // Build graph
    Graph graph;
    for (const auto& [id, pt] : points) {
        graph.addPin(id, pt.latitude, pt.longitude, pt.elevation);
    }

    for (const auto& trail : trails) {
        const auto& pts = trail.trailPoints;
        for (size_t i = 0; i + 1 < pts.size(); ++i) {
            graph.addEdge(pts[i], pts[i + 1]);
        }
    }

    // Example: compute shortest path
    long startId = trails[0].trailPoints.front();
    long endId = trails[0].trailPoints.back();
    std::vector<long long> path = graph.findShortestPathDijkstra(startId, endId);

    // Main loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        // Draw trails
        for (const auto& trail : trails) {
            const auto& ids = trail.trailPoints;
            if (ids.size() < 2) continue;

            sf::VertexArray lines(sf::PrimitiveType::LineStrip, ids.size());
            for (size_t i = 0; i < ids.size(); ++i) {
                const auto& p = points.at(ids[i]);
                lines[i].position = sf::Vector2f(p.longitude * SCALE_X, p.latitude * SCALE_Y);
                lines[i].color = sf::Color(200, 200, 200);  // light gray
            }
            window.draw(lines);
        }

        // Draw shortest path
        sf::VertexArray pathLine(sf::PrimitiveType::LineStrip, path.size());
        for (size_t i = 0; i < path.size(); ++i) {
            const auto& p = points.at(path[i]);
            pathLine[i].position = sf::Vector2f(p.longitude * SCALE_X, p.latitude * SCALE_Y);
            pathLine[i].color = sf::Color::Green;
        }
        window.draw(pathLine);

        window.display();
    }
    */
    return 0;
}