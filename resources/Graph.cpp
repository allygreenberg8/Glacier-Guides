#include "Graph.h"
#include <iostream>
#include <vector> 
#include <unordered_map>
#include "TerrainParser.h"
#include "TerrainParser.cpp"
using namespace std;

// Using the haversine formula to acurately calculate the weights for the distance between two locations, need to use this formula because the Earth is spherical - https://www.movable-type.co.uk/scripts/latlong.html
double haversine(double latitudeStart, double longitudeStart, double latitudeEnd, double longitudeEnd) {
    const double earthRadius = 6371.0; // radius of the Earth in kilometers

    //calculate the differences between the latitudes and longitudes
    double latitudeDiff = latitudeEnd - latitudeStart;
    double longitudeDiff = longitudeEnd - longitudeStart;

    //convert the differences to radians because the haversine formula uses radians as it is a trigonometric function
    latitudeDiff *= M_PI / 180.0;
    longitudeDiff *= M_PI / 180.0;

    //calculate the central angle between the two points
    double centralAngle = sin(latitudeDiff / 2) * sin(latitudeDiff / 2) + cos(latitudeStart * M_PI / 180.0) * cos(latitudeEnd * M_PI / 180.0) * sin(longitudeDiff / 2) * sin(longitudeDiff / 2);
    
    ///calculate the arc length between the two points
    double arc = 2 * atan2(sqrt(centralAngle), sqrt(1 - centralAngle));

    //the radius times the arc length gives the distance between the two points
    double distance = earthRadius * arc;

    //the returned distance is in kilometers
    return distance;
}

void Graph::findShortestPath(int start, int end) {
}

void Graph::buildGraphFromParsed(const TerrainParser& parser) {
    // get points
    const vector<Point>& points = parser.getPoints();
    // get trails 
    const vector<Trail>& trails = parser.getTrails();
    // get elevations
    const vector<Elevation>& elevations = parser.getElevation();

    // NODES IN GRAPH
    // create a pin for each point - a pin is like a node in a graph
    for (const Point& point : points) {
        double elevation = 0.0;
        // find exact elevation for each point
        for (const Elevation& e : elevations) {
            if ((point.latitude == e.latitude) && (point.longitude == e.longitude)) {
                elevation = e.elevation;
                break;
            }
        }

        // add the pin into the graph with its attributes
        addPin(point.id, point.latitude, point.longitude, elevation);
    }

    // EDGES IN GRAPH
    // create a directed graph because you can only go down the moutain (only downhill)
    for (const Trail& trail : trails){
        // we need to skip the lifts that are the edges 
        // this may need to be changed based on the what the data catagorizes the lift as
        if (trail.trailType == "lift" || trail.trailType == "chair_lift" || trail.trailType == "t-bar" || trail.trailType == "rope_tow" || trail.trailType == "j-bar" || trail.trailType == "drag_lift") {
            
            // lifts - the weight is based only on distance
            for (int i = 0; i < trail.trailPoints.size() - 1; i++) {
                int fromPoint = trail.trailPoints[i];
                int toPoint = trail.trailPoints[i + 1];

                // calculate the weight of the edge aka the distance using the haversine formula
                double weight = haversine(pins[fromPoint].latitude, pins[fromPoint].longitude, pins[toPoint].latitude, pins[toPoint].longitude);
                // add the downhill edge
                addEdge(fromPoint, toPoint, weight);

                // lifts have a fixed multiplier for their weights 
                double weightMultiplier = weight * 1.5;
                addEdge(fromPoint, toPoint, weightMultiplier);
            }
        }
        else{
            // for each pair of points in the trail, add an edge between them
            for (int i = 0; i < trail.trailPoints.size() - 1; i++) {
                int fromPoint = trail.trailPoints[i];
                int toPoint = trail.trailPoints[i + 1];

                //if the trail goes downhill then create the edge 
                if (pins[fromPoint].elevation > pins[toPoint].elevation) {
                    // calculate the weight of the edge aka the distance using the haversine formula
                    double weight = haversine(pins[fromPoint].latitude, pins[fromPoint].longitude, pins[toPoint].latitude, pins[toPoint].longitude);
                    
                    // calculate elevation change
                    double elevationChange = pins[fromPoint].elevation - pins[toPoint].elevation;

                    // calculate the slope of the trail
                    double slope = elevationChange / (weight * 1000); // multiple by 1000 to convert to meters
                    
                    // calculate saftey weights with reference to the slope that references the elevation
                    double safety;
                    // for beginners
                    if (slope < 0.15){
                        safety = 1.0;  //easy
                    }
                    // for intermediate
                    if (slope < 0.25){
                        safety = 1.2; //medium 
                    }
                    // for advanced 
                    if (slope < 0.35){
                        safety = 1.5; //hard
                    }
                    // for expert
                    else{
                        safety = 2.0; //extreme
                    }

                    // set the final weight of the edges that takes distance and safety into consideration
                    double finalWeight = weight * safety; //remember weight here is the original distance
                    
                    // add the downhill edge
                    addEdge(fromPoint, toPoint, finalWeight);
                }
            }
        } 
    }
}