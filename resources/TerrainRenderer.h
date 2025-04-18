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
    // This function will take the elevation and path data from the parser function
    void loadTerrain();

    // Draw in shading to make map look three dimensional
    void drawShades(sf::gfx::RenderWindow& mapWindow);

    // Function will draw in the red line path between the chosen starting point and destination
    void drawPathLine(sf::gfx::RenderWindow& mapWindow);

    // This variable stores a reference to the parsed data from the parser function in TerrainRenderer.cpp
    const TerrainParser& terrainParser;

    // This variable sotres a 2D vector of the elevations of the selected terrain
    std::vector<std::vector<double>> elevationData;

    // This variable stores the start and end points
    std::vector<int> pathStartStop;

    // Stores the width of the map screen that will display the terrain
    int terrainWidth;
    
    // Stores the height of the map screen that will display the terrain
    int terrainHeight;

    // Stores minimum elevation given for path
    double minElevation;

    // Stores user-input of maximum elevation chosen for path
    double maxElevation;


public:
    // Constructor that takes in parsed elevation data from my TerrainParser function in that file
    TerrainRenderer(const TerrainParser& terrainParser);

    // Function that will draw terrain and the chosen paths to the display window
    void renderTerrain(sf::gfx::RenderWindow& mapWindow);

    // Function to take in path calculated by Dijsktra or A* so it can be drawn into map screen
    void choosePath(const std::vector<int>& path);

};
