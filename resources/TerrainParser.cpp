#include "TerrainParser.h"

/*
Utilzed this site in setting up and formatting pugximl library and utilized given templates when given in order to minimize chances of errors I am unfamiliar with
Link: https://pugixml.org/docs/manual.html
*/


// Returns the list of parsed points
const std::vector<Point>& TerrainParser::getPoints() const
{
    return points;
}


// Returns the list of parsed trails
const std::vector<Trail>& TerrainParser::getTrails() const
{
    return trails;
}


// Returns the list of parsed elevation data points
const std::vector<Elevation>& TerrainParser::getElevation() const
{
    return elevationPtData;
}


// Loads an OSM file from OpenSkiMap
bool TerrainParser::loadFile(const std::string& fileName)
{
    pugi::xml_document osmFile;

    if (!osmFile.load_file(fileName.c_str()))
    {
        std::cerr << "Failed to load OSM file: " << fileName << std::endl;
        return false;
    }

    // Parse all node elements under the osm root
    for (pugi::xml_node point : osmFile.child("osm").children("node"))
    {
        pointParser(point);
    }

    // Parse all way elements under the osm root
    for (pugi::xml_node trail : osmFile.child("osm").children("way"))
    {
        trailParser(trail);
    }

    return true;
}


// Parses a point from an OSM node element
void TerrainParser::pointParser(const pugi::xml_node& point)
{
    Point p;
    p.id = point.attribute("id").as_llong();
    p.latitude = point.attribute("lat").as_double();
    p.longitude = point.attribute("lon").as_double();
    points.push_back(p);
    skiMap[p.id] = p;
}


// Parses a trail from an OSM way element
void TerrainParser::trailParser(const pugi::xml_node& trail)
{
    Trail t;
    t.id = trail.attribute("id").as_llong();

    // Collect all node references that make up the trail
    for (pugi::xml_node path_node : trail.children("nd"))
    {
        long long reference = path_node.attribute("ref").as_llong();
        t.trailPoints.push_back(reference);
    }

    // Extract tags for the trail (ie. name and type of trail)
    for (pugi::xml_node xmlTag : trail.children("tag"))
    {
        std::string tagKey = xmlTag.attribute("k").as_string();
        std::string tagValue = xmlTag.attribute("v").as_string();

        if (tagKey == "name")
        {
            t.trailName = tagValue;
        }
        else if (tagKey == "aerialway" || tagKey == "piste:type")
        {
            t.trailType = tagValue;
        }
    }

    trails.push_back(t);
}


// Loads elevation data from an OpenTopography CSV file
bool TerrainParser::loadElevationFile(const std::string& fileName)
{
    std::ifstream ElevationFile(fileName);

    if (!ElevationFile.is_open())
    {
        std::cerr << "Failed to open elevation CSV file: " << fileName << std::endl;
        return false;
    }

    std::string csvLine;
    int lineNumber = 0;

    while (std::getline(ElevationFile, csvLine))
    {
        if (lineNumber == 0)
        {
            // Skip header file
            lineNumber++;
            continue;
        }

        std::stringstream data(csvLine);
        std::string latitudeString, longitudeString, elevationString;

        std::getline(data, latitudeString, ',');
        std::getline(data, longitudeString, ',');
        std::getline(data, elevationString, ',');

        // If we have all this data, we can make a elevation point now
        if (!latitudeString.empty() && !longitudeString.empty() && !elevationString.empty())
        {
            Elevation elevationPoint;
            elevationPoint.elevation = std::stod(elevationString);
            elevationPoint.longitude = std::stod(longitudeString);
            elevationPoint.latitude = std::stod(latitudeString);
            elevationPtData.push_back(elevationPoint);
        }
    }

    ElevationFile.close();
    return true;
}
