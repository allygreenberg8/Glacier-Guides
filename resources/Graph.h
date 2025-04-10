#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;

struct Pin{
    // node is representative of a location - pin on the map 
    int pin_ID;
    double latitude;    // north/south
    double longitude;   // east/west
    double elevation;   // height

    // Constructor
    Pin(int pin_ID, double latitude, double longitude, double elevation){
        this->pin_ID = pin_ID;
        this->latitude = latitude;
        this->longitude = longitude;
        this->elevation = elevation;
    }
};

class Graph{
    private: 
    unordered_map <int, Pin> pins;
    unordered_map <int, list<pair<int, double>>> adjList;

    public:
    void addPin(int pin_ID, double latitude = 0.0 , double longitude = 0.0, double elevation = 0.0){
        pins[pin_ID] = Pin(pin_ID, latitude, longitude, elevation); // add new pin to map with its attributes
    }
    void addEdge(int from, int to, double weight){
        adjList[from].emplace_back(to, weight);
    }

    const unordered_map <int, Pin>& getPins() const{
        return pins;
    }
    const unordered_map <int, list<pair<int, double>>>& getAdjList() const{
        return adjList;
    }

};
