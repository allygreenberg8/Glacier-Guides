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


// Struct Representing varaibles/characteristics of the path chosen in OpenSkiMap
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



class TerrainParser
{
public:
    // Function loads in OpenSkiMap file and returns true if successful
    bool loadFile(const std::string& fileName);
    // Retrieves stored points and returns a constant reference to them
    const std::vector<Point>& getPoints() const;
    // Retrieves stored trails and returns a constant reference to them
    const std::vector<Trail>& getTrails() const;

private:
    // Vector holding stored points that will make up each trail
    std::vector<Point> points;
    // Vector holding stored all the trails stored (each trail is a specific group of points)
    std::vector<Trail> trails;
    // Unordered map for quick access of map points by referencing them by their id given in OpenSkiMap
    std::unordered_map<long long, Point> skiMap;
    // Function will parse the xml files from OpenSkiMap to extract all needed points data
    void pointParser(const pugi::xml_node& point);
    // Function will parse the xml files from OpenSkiMap to extract all needed trailcd data
    void trailParser(const pugi::xml_node& trail);
};


