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
    // creates xml_document object with pugximl framework in order to store file
    pugi::xml_document osmFile;

    // Checks if file can't be loaded in, and prints error message if this is the case
    if (!osmFile.load_file(fileName.c_str()))
    {
        // logs error to terminal and returns error message
        std::cerr << "Failed to load OSM file from OpenSkiMap: " << fileName << std::endl;
        // returns false boolean indicating file was not uploaded correctly
        return false;
    }

    // Find every node object in file that is inside each <osm> root element
    // Use for-loop to iterate through each node object in the root element
    // osmFile.child("osm") goes to the osm oroot and .children("node") gives all of the children of that node
    for (pugi::xml_node point : osmFile.child("osm").children("node"))
    {
        pointParser(point);
    }

    // Find every way object in file that is inside each <osm> root element
    // Use for-loop to iterate through each way object in the root element
    // osmFile.child("osm") goes to the osm oroot and .children("node") gives all of the children of that node and store this in trail variable
    for (pugi::xml_node trail : osmFile.child("osm").children("way"))
    {
        trailParser(trail);
    }

    // Return true because file has been sucessfully loaded
    return true;
}


// Function for parsing OSM file data for each point and assigning the data to the Point struct variables
void TerrainParser::pointParser(const pugi::xml_node& point)
{
    // Initialize point object from struct in header class
    // Each point object represents a specific geographical location
    Point p;
    // Assign id attribute of xml_node to id variable made for struct Point
    p.id = point.attribute("id").as_llong();
    // Assign lat attribute of xml_node to latitude variable made for struct Point
    p.latitude = point.attribute("lat").as_double();
    // Assign lon attribute of xml_node to longitude variable made for struct Point
    p.longitude = point.attribute("lon").as_double();
    // Add this point to vector list of points
    points.push_back(p);
    // Add the key-value pair of the point id and the node object respectively to the map
    skiMap[p.id] = p;
}


// Function for parsing OSM file data for each trail (xml way objet) and assigning the data the Trail struct variables
void TerrainParser::trailParser(const pugi::xml_node& trail)
{
    // Initialize trail object from struct in header class
    Trail t;
    // Assign id data from OSM to Trail struct object id variable
    t.id = trail.attribute("id").as_llong();

    // Loops through all of the "nd" aka "node references" inside the xml file "way" container/object
    for (pugi::xml_node path_node : trail.children("nd"))
    {
        // Assign node reference value (equivalent to id for individual point variables)
        long long reference = path_node.attribute("ref").as_llong();
        // Push this back to trail struct variable that holds all the point/xml_node references of the points that make up the trail
        t.trailPoints.push_back(reference);
    }

    // Loops through the metatags inside each way xml "way" object in the xml file to extract the data on what type of trail it is
    // Each xml tag is stored in xml as a key-value pair and both must be extracted and stored appropriately
    for (pugi::xml_node xmlTag : trail.children("tag"))
    {
        // store key of xmlTag as variable tagKey
        std::string tagKey = xmlTag.attribute("k").as_string();
        // store value of xmlTag as variable tagValue
        std::string tagValue = xmlTag.attribute("v").as_string();
        // If the key equals "name" in xml, assign the trailName to equal the tagValue
        if (tagKey == "name")
        {
            t.trailName = tagValue;
        }
        // if the tagKey is "aerialway" the trail is a lift, and if the tagKey is "piste:type" it is a trail
        // Store which type of trail this is (if applicable) in the trailType variable of trail struct object
        else if (tagKey == "aerialway")
        {
            t.trailType = tagValue;
        }
        // If the tagKey is "piste:type" the path is a trail and not a lift
        else if (tagKey == "piste:type")
        {
            t.trailType = tagValue;
        }
        // Push back this parsed trail to the vector list of trails
        trails.push_back(t);
    }
}


// Function for parsing csv file data from OpenTopography to parse and store elevation data for each map point
bool TerrainParser::loadElevationFile(const std::string& fileName)
{
    // Open the file given in the function parameter
    std::ifstream ElevationFile(fileName);

    // First check if file was opened, and if this is not the case, return error message and false
    if (ElevationFile.is_open() != false)
    {
        std::cerr << "OpenTopography csv file could not be opened in order to find elevation data. File name: " << fileName << std::endl;
    }

    // Instantiate line variable to store each line of the CSV file iterated over by the for-loop
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
            // increments line past 0 so we know rest of lines are not first one
            lineNumber += 1;
            // moves to next iteration
            continue;
        }

        // Creates stringstream object for storing each line and initializes string variables to hold latitude, longitude, and elevation data
        std::stringstream data(csvLine);
        std::string latitudeString;
        std::string longitudeString;
        std::string elevationString;
        
        // Read in the latitude, longitude, and elevation data to the string variables initialized
        std::getline(data, latitudeString, ',');
        std::getline(data, longitudeString, ',');
        std::getline(data, elevationString, ',');

        // If latitude, longitude, and elevation data exist for the point, it exists and we will convert this data to doubles and store it
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

    // Close the csv file
    ElevationFile.close();
    // Return true to inicate the file was successfully opened and read
    return true;

}

