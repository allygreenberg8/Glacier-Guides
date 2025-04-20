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


/*
-> Both the .cpp and .h have these comments but just wanted to include it in each

-> I utilized the SFML Tutorial Suggested Formats/Templates for this code in order to minimize chances of unfamiliar bugs:
https://www.sfml-dev.org/tutorials/3.0/graphics/draw/#off-screen-drawing

-> Note: We ran into this problem, as Brody has a Dell computer and Ally has a Macbook, but SFML package installation will vary
   upon device.
   
-> Make sure to download SFML Version 3.0 (will encounter problems with syntax if use older version (ie. Version 2.6)

*/


class TerrainRenderer
{
private:
    // Takes the elevation and path data from the parser function
    void loadTerrain();

    // Draws in shading to make map look 3D
    void drawShades(sf::RenderWindow& mapWindow);

    // Draws in the red line path between the chosen starting point and ending point
    void drawPathLine(sf::RenderWindow& mapWindow);

    // Stores a reference to the parsed data from the parser function in TerrainRenderer.cpp
    const TerrainParser& terrainParser;

    // Stores a 2D vector of the elevations of the selected terrain
    std::vector<std::vector<double>> elevationData;

    // Stores the start and end points of the chosen path
    std::vector<int> pathPoints;

    // Stores the width of the map screen
    int terrainWidth;
    
    // Stores the height of the map screen
    int terrainHeight;

    // Stores the minimum elevation given for path
    double minElevation;

    // Stores maximum elevation chosen for path
    double maxElevation;


public:
    // Constructor takes in parsed elevation data from my TerrainParser function in that file
    TerrainRenderer(const TerrainParser& terrainParser);

    // Draws terrain and the chosen paths to the display window
    void renderTerrain(sf::RenderWindow& mapWindow);

    // Takes in path calculated by Dijsktra or A* so it can be drawn into map screen
    void choosePath(const std::vector<int>& path);

};

