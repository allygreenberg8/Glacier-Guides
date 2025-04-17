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
}
