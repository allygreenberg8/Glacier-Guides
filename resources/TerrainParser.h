#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
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
    // Points that make up this path
    std::vector<long long> trailPoints;
    // If name available, store name of path here:
    std::string trailName;
};



class TerrainParser
{
public:
    // Function loads in OpenSkiMap file and returns true if successful
    bool loadFile(const std::string& fileName);
    // Retrieves stored points and returns a constant reference to them
    const std::vector<Point> getPoints() const;
    // Retrieves stored trails and returns a constant reference to them
    const std::vector<Trail> getTrails() const;

private:
    // Vector holding stored map points that will make up each trail
    std::vector<Point> points;
    std::vector<Trail> trails;
    std::unordered_map<long long, Point> skiMap;

    void pointParser(const pugi::xml_node& point);
    void trailParser(const pugi::xml_node& trail);





};


