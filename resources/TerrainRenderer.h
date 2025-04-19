#include "TerrainParser.h"
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>


class TerrainRenderer
{
private:
    // Function will take the elevation and path data from the parser function
    void loadTerrain();

    // Function draws in shading to make map look 3D
    void drawShades(sf::RenderWindow& mapWindow);

    // Function will draw in the red line path between the chosen starting point and ending point
    void drawPathLine(sf::RenderWindow& mapWindow);

    // Variable stores a reference to the parsed data from the parser function in TerrainRenderer.cpp
    const TerrainParser& terrainParser;

    // Variable sotres a 2D vector of the elevations of the selected terrain
    std::vector<std::vector<double>> elevationData;

    // Variable stores the start and end points of the chosen pathy
    std::vector<int> pathPoints;

    // Variable stores the width of the map screen that will display the terrain
    int terrainWidth;
    
    // Variable stores the height of the map screen that will display the terrain
    int terrainHeight;

    // Variable stores the minimum elevation given for path
    double minElevation;

    // Variable stores the user-input of maximum elevation chosen for path
    double maxElevation;


public:
    // Constructor takes in parsed elevation data from my TerrainParser function in that file
    TerrainRenderer(const TerrainParser& terrainParser);

    // Function draws terrain and the chosen paths to the display window
    void renderTerrain(sf::RenderWindow& mapWindow);

    // Function takes in path calculated by Dijsktra or A* so it can be drawn into map screen
    void choosePath(const std::vector<int>& path);

};

