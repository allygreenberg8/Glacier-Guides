#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include "TerrainParser.h"
#include "TerrainRenderer.h"
#include "Graph.h"
using namespace std;

int main()
{
    /*
    // Setup window
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Glacier Guides - Trail Viewer");
    window.setFramerateLimit(60);
    
    // Grab data
    // Parse terrain
    TerrainParser parser;

    vector<string> fileNames = ["Glacier-Guides/SkiMapData/Age_of_Reason.osm", "Glacier-Guides/SkiMapData/Backdoor.osm", 
    "Glacier-Guides/SkiMapData/Berries_Upper.osm", "Glacier-Guides/SkiMapData/Birds_of_Prey.osm", "Glacier-Guides/SkiMapData/Boa.osm".
    "Glacier-Guides/SkiMapData/Cinch_Upper.osm", "Glacier-Guides/SkiMapData/Copperhead.osm", "Glacier-Guides/SkiMapData/Elkhorn_Trail.osm".
    "Glacier-Guides/SkiMapData/Golden_Peak_Race.osm", "Glacier-Guides/SkiMapData/Harmony.osm". "Glacier-Guides/SkiMapData/Kellys_Toll_Road.osm",
    "Glacier-Guides/SkiMapData/Lady_of_the_Lake.osm", "Glacier-Guides/SkiMapData/Lodgers_Loop.osm", "Glacier-Guides/SkiMapData/Lookout_Bypass.osm",
    "Glacier-Guides/SkiMapData/Meadows.osm", "Glacier-Guides/SkiMapData/ParkCity.osm", "Glacier-Guides/SkiMapData/Poppyfields_West.osm",
     "Glacier-Guides/SkiMapData/RedTail.osm", "Glacier-Guides/SkiMapData/Silk_Road_East.osm", "Glacier-Guides/SkiMapData/Silver_Streak.osm"
    "Glacier-Guides/SkiMapData/Sleepytime_Road.osm", "Glacier-Guides/SkiMapData/Thaynes_Canyon.osm", "Glacier-Guides/SkiMapData/TheSlot.osm",
    "Glacier-Guides/SkiMapData/Tycoon.osm"]

    // Loops through all OpenSkiMap files and parses the data
    for (int i = 0; i < fileNames.size(); i++)
    // Parse terrain - only going to use one file for testing purposes
     */
    
     TerrainParser parse;

    // Print current working directory for debugging
    filesystem::path currentPath = filesystem::current_path();
    cout << "Current working directory: " << currentPath << std::endl;

    // Try to load the file with correct path
    // Note: this does not work when trying to pull from SkiMap Data folder
    if (!parse.loadFile("map.osm"))
    {
        cerr << "Failed to load terrain data." << endl;
        return 1;
    }
    // get the points and trails from the parser
    const auto &points = parse.getPoints();
    const auto &trails = parse.getTrails();

    // build graph
    Graph graph;
    for (const auto &point : points)
    {
        graph.addPin(point.id, point.latitude, point.longitude);
    }

    // add the edges for the trails wiht the corrects points
    for (const auto &trail : trails)
    {
        const auto &pts = trail.trailPoints;
        for (size_t i = 0; i + 1 < pts.size(); i++)
        {
            const auto &from = points[pts[i]];
            const auto &to = points[pts[i + 1]];
            double weight = sqrt(pow(to.longitude - from.longitude, 2) + pow(to.latitude - from.latitude, 2));
            graph.addEdge(pts[i], pts[i + 1], weight);
        }
    }

    // choose test points (first and last points)
    if (trails.empty() || trails[0].trailPoints.size() < 2)
    {
        cerr << "No enough points to test." << endl;
        return 1;
    }

    int start = trails[0].trailPoints.front();
    int end = trails[0].trailPoints.back();

    cout << "Start: " << start << ", End: " << end << endl;

    // Find maximum node ID to size data structures
    // This prevents excessive memory allocation while ensuring we can index all nodes - was having a seg fault
    size_t maxNodeId = 0;
    size_t maxId = 0;
    for (const auto &[id, _] : graph.getPins())
    {
        maxId = max(maxId, static_cast<size_t>(id));
    }
    // adjacency list representation
    // Each index contains a vector of pairs (neighbor_id, weight)
    // Initialize path and distance tracking for both algorithms
    vector<vector<pair<int, int>>> allPaths(maxId + 1);
    vector<int> dijkstraDistances(maxId + 1, numeric_limits<int>::max());
    vector<int> astarDistances(maxId + 1, numeric_limits<int>::max());

    // distance vectors for both algs
    for (const auto &[from, edges] : graph.getAdjList())
    {
        for (const auto &[to, weight] : edges)
        {
            allPaths[to].emplace_back(from, static_cast<int>(weight));
        }
    }

    // Dijkstra's Algorithm Time Calculation
    auto startTime = chrono::high_resolution_clock::now();
    graph.findShortestPathDijkstra(start, allPaths, dijkstraDistances);
    auto endTime = chrono::high_resolution_clock::now();
    auto dijkstraDuration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    // A* Algorithm Time Calculation
    startTime = chrono::high_resolution_clock::now();
    graph.findShortestPathAStar(start, end, allPaths, astarDistances);
    endTime = chrono::high_resolution_clock::now();
    auto astarDuration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    // Print the times
    cout << "Dijkstra's Algorithm Path: " << dijkstraDuration << " microseconds" << endl;
    cout << "A* Algorithm: " << astarDuration << " microseconds" << endl;

    // Compare the times
    if (dijkstraDuration < astarDuration)
    {
        cout << "Dijkstra's Algorithm is faster." << endl;
    }
    else if (dijkstraDuration > astarDuration)
    {
        cout << "A* Algorithm is faster." << endl;
    }
    else
    {
        cout << "Both algorithms have the same runtime." << endl;
    }
}
