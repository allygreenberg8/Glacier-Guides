#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "pugixml.hpp"


// Struct representing variables for each point selected on OpenSkiMap
struct Point
{
    // OpenSkiMap point ID
    long long id;
    // Longitude variable
    double longitude;
    // Latitude variable
    double latitude;
};


// Struct representing variables/characteristics of the path chosen in OpenSkiMap
struct Trail
{
    // Unique ID for path from OpenSkiMap
    long long id;
    // Points that make up this path (container of xml_node references)
    std::vector<long long> trailPoints;
    // If name available, store name of path here:
    std::string trailName;
    // Stores what type of trail the the path is (ie. trail or lift)
    std::string trailType;
};


// Struct reprsenting point with elevation data
// Needed to create separate struct for this because will need to get data from OpenTopography:
    // Therefore parsing will be different as file types will be .csv
struct Elevation
{
    // Store elevation at specified point
    double elevation;
    // Stores latitude of specified point
    double latitude;
    // Store longitude of specified point
    double longitude; 
};


// Class that contains the member variable and functions that will be used to parse and store the data from
// OpenSkiMap and OpenTopography for trail and elevation data.
class TerrainParser
{
private:
    // Vector holding stored points that will make up each trail
    std::vector<Point> points;
    // Vector holding stored all the trails stored (each trail is a specific group of points)
    std::vector<Trail> trails;
    // Unordered map for quick access of map points by referencing them by their id given in OpenSkiMap
    std::unordered_map<long long, Point> skiMap;
    // Vector holder all parsed elevation data for each elevation point
    std::vector<Elevation> elevationPtData;
    // Function will parse the xml files from OpenSkiMap to extract all needed points data
    void pointParser(const pugi::xml_node& point);
    // Function will parse the xml files from OpenSkiMap to extract all needed trailcd data
    void trailParser(const pugi::xml_node& trail);

public:
    // Function loads in OpenSkiMap osm file and returns true if successful
    bool loadFile(const std::string& fileName);
    // Function loads in the OpenTopography csv file and returns true if successful
    bool loadElevationFile(const std::string& filename);
    // Retrieves stored points and returns a constant reference to them
    const std::vector<Point>& getPoints() const;
    // Retrieves stored trails and returns a constant reference to them
    const std::vector<Trail>& getTrails() const;
    // Retrieves stored elevation data and returns a constant reference to them
    const std::vector<Elevation>& getElevation() const;
};

