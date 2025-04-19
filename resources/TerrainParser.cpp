#include "TerrainParser.h"


// Helper function to get the private TerrainParser class list of parsed out Point objects
const std::vector<Point>& TerrainParser::getPoints() const
{
    return points;
}


// Helper function to get the private TerrainParser class list of parsed out Trail objects
const std::vector<Trail>& TerrainParser::getTrails() const
{
    return trails;
}


// Helper function to get the private TerrainParser class list of parsed out Elevation data objects
const std::vector<Elevation>& TerrainParser::getElevation() const
{
    return elevationPtData;
}


// Function to load OSM files from OpenSkiMap
bool TerrainParser::loadFile(const std::string& fileName)
{
    // creates xml object to read file (note: kinda similar to stringstream)
    pugi::xml_document osmFile;

    // Checks if file can't be loaded in, and prints error message if this is the case
    if (!osmFile.load_file(fileName.c_str()))
    {
        // logs error to terminal and returns error message
        std::cerr << "Failed to load OSM file from OpenSkiMap: " << fileName << std::endl;
        // returns false boolean indicating file was not uploaded correctly
        return false;
    }

    // Use for-loop to iterate through each node object in the root element (ie. kinda like iterating though BST)
    for (pugi::xml_node point : osmFile.child("osm").children("node"))
    {
        pointParser(point);
    }

    // Use for-loop to iterate through each way object in the root element
    for (pugi::xml_node trail : osmFile.child("osm").children("way"))
    {
        trailParser(trail);
    }

    // File has been loaded succesfully
    return true;
}


// Function for parsing OSM file data for each point and assigning the data to the Point struct variables
void TerrainParser::pointParser(const pugi::xml_node& point)
{
    // Initialize point and assign it a parsed id, latitute, and lonigitude and push it back for storage
    Point p;
    p.id = point.attribute("id").as_llong();
    p.latitude = point.attribute("lat").as_double();
    p.longitude = point.attribute("lon").as_double();
    points.push_back(p);
    skiMap[p.id] = p;
}


// Function for parsing OSM file data for each trail (xml way objet) and assigning the data the Trail struct variables
void TerrainParser::trailParser(const pugi::xml_node& trail)
{
    // Initialize trail object and its id
    Trail t;
    t.id = trail.attribute("id").as_llong();

    // Loops through all of the "nd" aka "node references" inside the xml file "way" container/object
    for (pugi::xml_node path_node : trail.children("nd"))
    {
        long long reference = path_node.attribute("ref").as_llong();
        t.trailPoints.push_back(reference);
    }

    // Loops through the metatags inside each way xml "way" object in the xml file to extract the data on what type of trail it is
    for (pugi::xml_node xmlTag : trail.children("tag"))
    {
        std::string tagKey = xmlTag.attribute("k").as_string();
        std::string tagValue = xmlTag.attribute("v").as_string();
        
        if (tagKey == "name")
        {
            t.trailName = tagValue;
        }
            
        // Note: if the tagKey is "aerialway" the trail is a lift
        else if (tagKey == "aerialway")
        {
            t.trailType = tagValue;
        }
        // If the tagKey is "piste:type" the path is a trail and not a lift
        else if (tagKey == "piste:type")
        {
            t.trailType = tagValue;
        }
        
        trails.push_back(t);
    }
}


// Function for parsing csv file data from OpenTopography to parse and store elevation data for each map point
bool TerrainParser::loadElevationFile(const std::string& fileName)
{
    std::ifstream ElevationFile(fileName);

    if (ElevationFile.is_open() != false)
    {
        std::cerr << "OpenTopography csv file could not be opened in order to find elevation data. File name: " << fileName << std::endl;
    }

    std::string csvLine;
    // This int will be used to skip over the first header line of the csv file (first line = 0)
    int lineNumber = 0;

    // Iterate through the lines of the csv file and store the latitude, longitude, and elevation data for each point in the Elevation struct
    // object and store that in a vector that is a member of class TerrainParser
    while (std::getline(ElevationFile, csvLine))
    {
        // Checks if line is first (ie. header) line
        if (lineNumber == 0)
        {
            lineNumber += 1;
            continue;
        }

        // Read in latitude, longitude, and elevation data with stringstream
        std::stringstream data(csvLine);
        std::string latitudeString;
        std::string longitudeString;
        std::string elevationString;
        
        std::getline(data, latitudeString, ',');
        std::getline(data, longitudeString, ',');
        std::getline(data, elevationString, ',');

        // Create point to store if exists
        if (latitudeString.empty() == false && longitudeString.empty() == false && elevationString.empty() == false)
        {
            // Instantiate Elevation struct object
            Elevation elevationPoint;
            // Convert elevation, longitude, and latitude data for each elevationPoint from strings to doubles
            elevationPoint.elevation = std::stod(elevationString);
            elevationPoint.longitude = std::stod(longitudeString);
            elevationPoint.latitude = std::stod(latitudeString);
            // Add this elevationPoint object to the list of elevation points
            elevationPtData.push_back(elevationPoint);
        }
    }
    
    ElevationFile.close();
    
    return true;

}

