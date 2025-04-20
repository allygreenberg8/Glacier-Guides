#include "TerrainRenderer.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <limits>
#include <algorithm>

/*
-> I utilized the SFML Tutorial Suggested Formats/Templates for this code in order to minimize chances of unfamiliar bugs:
https://www.sfml-dev.org/tutorials/3.0/graphics/draw/#off-screen-drawing

-> Note: We ran into this problem, as Brody has a Dell computer and Ally has a Macbook, but SFML package installation will vary
   upon device.
-> Make sure to download SFML Version 3.0

*/

// Constructor initializes with a TerrainParser reference and loads terrain data
TerrainRenderer::TerrainRenderer(const TerrainParser& parser)
    : terrainParser(parser)
    , terrainWidth(0)
    , terrainHeight(0)
    , minElevation(0.0)
    , maxElevation(0.0)
{
    loadTerrain();
}


// Function will take the elevation and path data from the parser function
void TerrainRenderer::loadTerrain()
{
    const auto& elevationPoints = terrainParser.getElevation();
    const size_t totalPoints = elevationPoints.size();

    // Checks base case of zero points
    if (totalPoints == 0)
    {
        terrainWidth = 0;
        terrainHeight = 0;
        return;
    }

    // Assume square grid, then calc grid size from total points
    size_t gridSize = static_cast<size_t>(std::round(std::sqrt(static_cast<double>(totalPoints))));
    terrainWidth = gridSize;
    terrainHeight = gridSize;

    // Resize elevationData to gridSize by gridSize, then initialize with zeros
    elevationData.assign(gridSize, std::vector<double>(gridSize, 0.0));

    // Fill in elevationData from elevationPoints
    for (size_t i = 0; i < totalPoints && i < gridSize * gridSize; ++i)
    {
        size_t colNum = i % gridSize;
        size_t rowNum = i / gridSize;
        elevationData[rowNum][colNum] = elevationPoints[i].elevation;
    }

    // Initialize min and max elevation values
    minElevation = std::numeric_limits<double>::max();
    maxElevation = std::numeric_limits<double>::lowest();

    // Find min and max elevation in the grid
    for (size_t rowNum = 0; rowNum < gridSize; ++rowNum)
    {
        for (size_t colNum = 0; colNum < gridSize; ++colNum)
        {
            double elevation = elevationData[rowNum][colNum];
            // Make subs if value is less than min or greater than max
            if (elevation < minElevation)
            {
                minElevation = elevation;
            }

            if (elevation > maxElevation)
            {
                maxElevation = elevation;
            }
        }
    }
}


// Function draws in shading to make map look 3D
void TerrainRenderer::drawShades(sf::RenderWindow& window)
{
    // Iterates though columns and rows of grid and change color based on relative elevation
    for (int row = 0; row < terrainHeight; ++row)
    {
        for (int col = 0; col < terrainWidth; ++col)
        {
            double elevation = elevationData[row][col];
            float intensity = 0.f;
                if (maxElevation != minElevation) {
                    intensity = (elevationPoints - minElevation) / (maxElevation - minElevation);
                }
                
            // Now make this point on the map into its individual rectangle shape (per SFML protocol)
            // Initilize each to a 10.0 by 10.0 rectangle
            //sf::gfx::RectangleShape point(sf::gfx::Vector2f(10.f,10.f));
            sf::RectangleShape point;
            point.setSize(sf::Vector2f(10.f,10.f));
            point.setPosition(sf::Vector2f(column * 10.f, row * 10.f)); 
            // Initialize color of point with higher elevations being a lighter color
            // Note: multiplying by 255 normalizes the color intensities
            point.setFillColor(sf::Color(intensity*255, intensity*255, intensity*255));
            // Draw the point to screen now after iteration calculations are complete
            window.draw(point);
        }
    }
}


// Function takes in path calculated by Dijsktra or A* so it can be drawn into map screen
void TerrainRenderer::choosePath(const std::vector<int>& path)
{
    pathPoints = path;
}


// Function will draw in the red line path between the chosen starting point and ending point
void TerrainRenderer::drawPathLine(sf::RenderWindow& window)
{
    if (pathPoints.empty())
    {
        return;
    }

    // Iterate through the points we stored and make line connecting them
    sf::VertexArray path(sf::PrimitiveType::LineStrip, pathPoints.size());
    for (size_t i = 0; i < pathPoints.size(); ++i)
    {
        int index = pathPoints[i];
        int col = index % terrainWidth;
        int row = index / terrainWidth;

        // Note: Ally, you may need to use a separate path or extension for Vector2f on MacBook
        int temp = 10.f;
        path[i].position = sf::Vector2f(col * temp, row * temp);
        path[i].color = sf::Color::Red;
    }

    window.draw(path);
}


// Function draws terrain and the chosen paths to the display window
void TerrainRenderer::renderTerrain(sf::RenderWindow& window)
{
    drawShades(window);
    drawPathLine(window);
}
