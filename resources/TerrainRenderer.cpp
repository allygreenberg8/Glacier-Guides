#include "TerrainRenderer.h"


// Code for constructor function
TerrainRenderer::TerrainRenderer(const TerrainParser& parser)
    // Initialize the member variable storing a reference to TerrainParser
    : terrainParser(parser)
    // Set terrain width, height, min and max elevations to 0.0
    ,terrainWidth(0.0)
    ,terrainHeight(0.0)
    ,minElevation(0.0)
    ,maxElevation(0.0)
    {
        // Call terrain loading function to process terrain data
        loadTerrain();
    } 

    
// This function will take the elevation and path data from the parser function
void TerrainRenderer::loadTerrain()
{
    // Since getElevation() returns a vector of Elevation onjects we need to creat a flat list
    // Then we will eventually extract the elevation data member variable stored in each struct
    const auto& elevationPoints = terrainParser.getElevation();
    // Create variable that saves how many elevation points we have parsed
    unsigned int totalElevationPts = elevationPoints.size();
    // Check if there are no elevation points --> this means there is no data:
        // If this is the case --> create empty graph/"terrain"
    if (totalElevationPts == 0)
    {
        // For making an empty graph --> set both the height and width of the terrain to 0 and return function
        terrainWidth = 0;
        terrainHeight = 0;
        return;
    }

    // For calculations, we will assume grid width and height are equal:
    /// Therefore grid area can be computed number of elevation point squared (because N by N grid assumed with N being the number of nodes in each row and column)
    size_t vertex = static_cast<size_t>(std::round(std::sqrt(double(totalElevationPts))));
    // Now Set terrain height and width both equal to this number of vertex / "elevation points"
    terrainHeight = vertex;
    terrainWidth = vertex;

    // Now we need to increase the size of the elevationData class-member vector to an N by N array by filling N^2 vertex elements set equal to 0
    elevationData.assign(vertex, std::vector<int>(vertex,0.0));

    // Now iterate through N^2 elements that will depict this terrain and calculate what the x and y indices of each will be
    // Once these indices are determined, extract the particular elevation member variable from the Elevation struct object that we are iterating through currently
    for (size_t i = 0; i < totalElevationPts && i < vertex*vertex;++i)
    {
        // Calculate specific x and y indices (ie. row and column indices in the N by N matrix)
        // Column number = x ; Row Number = y
        size_t x = i % vertex;
        size_t y = i / vertex;
        // Now add the elevation data from the Elevation object from TerrainParser to the specified row and column indices
        elevationData[y][x] = elevationPoints[i].elevation;
    }

    // Set min and max elevation values to initialize to min and max values respectively before we calculate the exact data from our given points
    minElevation = std::numeric_limits<double>::max();
    maxElevation = std::numeric_limits<double>::lowest();

    // Now iterate through vertex by vertex matrix of points and changes minElevation if lower elevation found and does the same for maxElevation if a greater elevation is found
    for (size_t rows=0; rows < vertex; ++rows)
    {
        for (size_t columns=0; columns < vertex; ++columns)
        {
            // Checks if specific elevation pt in matrix has a higher elevation than max or lower than min and if it does add it to the respective variable
            double elevationPt = elevationData[rows][columns];
            maxElevation = std::max(maxElevation, elevationPt);
            minElevation = std::min(minElevation, elevationPt);
        }
    }
}


// Function to draw in shading to make map look three dimensional
void TerrainRenderer::drawShades(sf::RenderWindow& window)
{
    // First we need to loop through the array of vertexes/points that make up the array
    // Note: assuming array is N by N with N being the number of terrain points (ie. number of vertices)
    for (int row = 0; row < terrainHeight; ++row)
    {
        for (int column = 0; column < terrainWidth; ++column)
        {
            // Store elevation pts in respective point in matrix
            double elevationPoints = elevationData[row][column];
            // Normalize the elevation of this point to a specified color density that will vary based on its elevation through this formula
            float intensity = (elevationPoints-minElevation) / (maxElevation - minElevation);

            // Now make this point on the map into its individual rectangle shape (per SFML protocol)
            // Initilize each to a 10.0 by 10.0 rectangle
            //sf::RectangleShape point(sf::Vector2f(10.f,10.f));
            sf::RectangleShape point;
            point.setSize(sf::Vector2f(10.f,10.f));
            point.setPosition(column * 10.f, row * 10.f);
            // Initialize color of point with higher elevations being a lighter color
            // Note: multiplying by 255 normalizes the color intensities
            point.setFillColor(sf::Color(intensity*255, intensity*255, intensity*255));
            // Draw the point to screen now after iteration calculations are complete
            window.draw(point);
        }
    }
}


// Function to take in path calculated by Dijsktra or A* so it can be drawn into map screen
void TerrainRenderer::choosePath(const std::vector<int>& chosenPath)
{
    // Sotre list of tile indices/ "terrain points" that make up the chosen path by either Dijkstra or A*
    pathStartStop = chosenPath;
}


// This function will take the given path points and scale them to fit at the appropriate points in our map and then draw a line between these points in red
void TerrainRenderer::drawPathLine(sf::RenderWindow& mapWindow)
{
    // First check if there are no path points (this means a path was not found)
    // If so --> return empty path
    if (pathStartStop.empty())
    {
        return;
    }

    // For this next line of code I am getting errors for the type LineStrip that should be a part of the SFML library
    // Create sfml line object for the path
    sf::VertexArray path(sf::LineStrip, pathStartStop.size());

    // Now iterate through array of N by N points and assign each point to a particular element of the array
    for (size_t i = 0; i < pathStartStop.size(); ++i)
    {
        // Add this particular elment index to variable
        int index = pathStartStop[i];
        // Calculate column (x-coordinate) and row (y-coordinate)
        // Error with this as well: unsure why terrainWidth causing error for column calculation
        int column = index % terrainWidth;
        int row = index / terrainWidth;

        // Now calculate position and add red color to path point
        path[i].position = sf::Vector2f(column*10.f, row*10.f);
        path[i].color = sf::Color::Red;
    }

    // Now draw this path to the screen
    mapWindow.draw(path);
}


// Function that will draw terrain and the chosen paths to the display window
// This function will be used in our main.cpp to call all of the needed helper functions for rendering our graph
void TerrainRenderer::renderTerrain(sf::RenderWindow& mapWindow)
{
    // Call drawShades function to add shading to make graphical interface look closer to 3-dimensional
    drawShades(mapWindow);
    // Call drawPathLine function to draw line on graph for chosen path between starting and ending points
    drawPathLine(mapWindow);
}


