#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "pugixml.hpp"

/*
-> Included same comment in .cpp but included it here also just in case

-> Utilzed this site in setting up and formatting pugximl library and utilized given templates when given in order to minimize chances of errors I am unfamiliar with
Link: https://pugixml.org/docs/manual.html

*/


// Represents a point from OpenSkiMap
struct Point
{
    // OpenSkiMap point ID
    long long id;
    // Longitude coordinate
    double longitude;
    // Latitude coordinate
    double latitude;
};

// Represents a path from OpenSkiMap
struct Trail
{
    // Unique ID for the path
    long long id;
    // IDs of points that make up this path
    std::vector<long long> trailPoints;
    // Name of the path, if available
    std::string trailName;
    // Type of path (e.g., trail or lift)
    std::string trailType;
};

// Represents a point with elevation data
// Separate struct because elevation data comes from OpenTopography CSV files
struct Elevation
{
    // Elevation at the point
    double elevation;
    // Latitude coordinate
    double latitude;
    // Longitude coordinate
    double longitude; 
};

// Parses and stores trail and elevation data from OpenSkiMap and OpenTopography
class TerrainParser
{
private:
    // Stored points that make up trails
    std::vector<Point> points;
    // Stored trails (each is a group of points)
    std::vector<Trail> trails;
    // Map for quick access to points by their OpenSkiMap ID
    std::unordered_map<long long, Point> skiMap;
    // Parsed elevation data points
    std::vector<Elevation> elevationPtData;
    // Parses point data from OpenSkiMap XML nodes
    void pointParser(const pugi::xml_node& point);
    // Parses trail data from OpenSkiMap XML nodes
    void trailParser(const pugi::xml_node& trail);

public:
    // Loads OpenSkiMap OSM file, returns true if successful
    bool loadFile(const std::string& fileName);
    // Loads OpenTopography CSV file, returns true if successful
    bool loadElevationFile(const std::string& filename);
    // Returns stored points
    const std::vector<Point>& getPoints() const;
    // Returns stored trails
    const std::vector<Trail>& getTrails() const;
    // Returns stored elevation data
    const std::vector<Elevation>& getElevation() const;
};
