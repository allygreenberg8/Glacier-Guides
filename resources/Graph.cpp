#include "Graph.h"
#include <unordered_set>

// Using the haversine formula to acurately calculate the weights for the distance between two locations, need to use this formula because the Earth is spherical - https://www.movable-type.co.uk/scripts/latlong.html
double haversine(double latitudeStart, double longitudeStart, double latitudeEnd, double longitudeEnd)
{
    const double earthRadius = 6371.0; // radius of the Earth in kilometers

    // calculate the differences between the latitudes and longitudes
    double latitudeDiff = latitudeEnd - latitudeStart;
    double longitudeDiff = longitudeEnd - longitudeStart;

    // convert the differences to radians because the haversine formula uses radians as it is a trigonometric function
    latitudeDiff *= M_PI / 180.0;
    longitudeDiff *= M_PI / 180.0;

    // calculate the central angle between the two points
    double centralAngle = sin(latitudeDiff / 2) * sin(latitudeDiff / 2) + cos(latitudeStart * M_PI / 180.0) * cos(latitudeEnd * M_PI / 180.0) * sin(longitudeDiff / 2) * sin(longitudeDiff / 2);

    /// calculate the arc length between the two points
    double arc = 2 * atan2(sqrt(centralAngle), sqrt(1 - centralAngle));

    // the radius times the arc length gives the distance between the two points
    double distance = earthRadius * arc;

    // the returned distance is in kilometers
    return distance;
}

void Graph::findShortestPathDijkstra(int startVertex, const vector<vector<pair<int, int>>> &graph, vector<int> &distances)
{
    // Stores max value system can hold in order to represent initialization to infinity of each distance
    const int infinity = numeric_limits<int>::max();

    // Initialize graph.size() amount of elements and distances and initialize to infinity
    for (int i = 0; i < graph.size(); i++)
    {
        distances[i] = infinity;
    }
    // Initialize distance for starting element to equal 0
    distances[startVertex] = 0;

    // Initialize minheap for storing (distance, vertex) pair - both will be int types
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> minHeap;

    // Add start index and startVertex itself to minheap in form (distance, vertex name)
    minHeap.emplace(0, startVertex);

    // While minHeap is not empty;
    while (!minHeap.empty())
    {
        // Unpack tuple of root of minHeap (this will be the element we are relaxing its edges for in this iteration)
        // Since this is a minHeap, this node will be the one with the smallest known distance
        auto [distance, currentVertex] = minHeap.top();
        // Pop this root out of minHeap now
        minHeap.pop();

        // If a better path to vertex, "currentVertex" has already been found, skip this vertex and continue to next iteration
        if (distance > distances[currentVertex])
        {
            continue;
        }

        // Iterate through the adjacency list graph and adjacent vetices to graph[currentVertex] and perform relaxations
        // Replace current distance values with relaxation values if the value found by relaxtion is less
        for (unsigned int i = 0; i < graph[currentVertex].size(); ++i)
        {
            // Initialize neighbor vertex 'name' in this iteration
            int neighbor = graph[currentVertex][i].first;
            // Initialize edge weight between currentVertex and neighbor
            int weight = graph[currentVertex][i].second;
            // Calculate new distance using edge relaxation formula
            int newDistance = distances[currentVertex] + weight;
            // If this newDistance is less than the current distance for neighbor, replace the value
            if (newDistance < distances[neighbor])
            {
                // Replace distance to neighbor with newDistance to neighbor in the minHeap
                distances[neighbor] = newDistance;
                minHeap.emplace(newDistance, neighbor);
            }
        }
    }
}

void Graph::buildGraphFromParsed(const TerrainParser &parser)
{
    // get points
    const vector<Point> &points = parser.getPoints();
    // get trails
    const vector<Trail> &trails = parser.getTrails();
    // get elevations
    const vector<Elevation> &elevations = parser.getElevation();

    // NODES IN GRAPH
    // create a pin for each point - a pin is like a node in a graph
    for (const Point &point : points)
    {
        double elevation = 0.0;
        // find exact elevation for each point
        for (const Elevation &e : elevations)
        {
            if ((point.latitude == e.latitude) && (point.longitude == e.longitude))
            {
                elevation = e.elevation;
                break;
            }
        }

        // add the pin into the graph with its attributes
        addPin(point.id, point.latitude, point.longitude, elevation);
    }

    // EDGES IN GRAPH
    // create a directed graph because you can only go down the moutain (only downhill)
    for (const Trail &trail : trails)
    {
        // we need to skip the lifts that are the edges
        // this may need to be changed based on the what the data catagorizes the lift as
        if (trail.trailType == "lift" || trail.trailType == "chair_lift" || trail.trailType == "t-bar" || trail.trailType == "rope_tow" || trail.trailType == "j-bar" || trail.trailType == "drag_lift")
        {

            // lifts - the weight is based only on distance
            for (int i = 0; i < trail.trailPoints.size() - 1; i++)
            {
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
        else
        {
            // for each pair of points in the trail, add an edge between them
            for (int i = 0; i < trail.trailPoints.size() - 1; i++)
            {
                int fromPoint = trail.trailPoints[i];
                int toPoint = trail.trailPoints[i + 1];

                // if the trail goes downhill then create the edge
                if (pins[fromPoint].elevation > pins[toPoint].elevation)
                {
                    // calculate the weight of the edge aka the distance using the haversine formula
                    double weight = haversine(pins[fromPoint].latitude, pins[fromPoint].longitude, pins[toPoint].latitude, pins[toPoint].longitude);

                    // calculate elevation change
                    double elevationChange = pins[fromPoint].elevation - pins[toPoint].elevation;

                    // calculate the slope of the trail
                    double slope = elevationChange / (weight * 1000); // multiple by 1000 to convert to meters

                    // calculate saftey weights with reference to the slope that references the elevation
                    double safety;
                    // for beginners
                    if (slope < 0.15)
                    {
                        safety = 1.0; // easy
                    }
                    // for intermediate
                    if (slope < 0.25)
                    {
                        safety = 1.2; // medium
                    }
                    // for advanced
                    if (slope < 0.35)
                    {
                        safety = 1.5; // hard
                    }
                    // for expert
                    else
                    {
                        safety = 2.0; // extreme
                    }

                    // set the final weight of the edges that takes distance and safety into consideration
                    double finalWeight = weight * safety; // remember weight here is the original distance

                    // add the downhill edge
                    addEdge(fromPoint, toPoint, finalWeight);
                }
            }
        }
    }
}

// Logic From https://www.geeksforgeeks.org/a-search-algorithm/
/*
// A* Search Algorithm
1.  Initialize the open list
2.  Initialize the closed list
    put the starting node on the open
    list (you can leave its f at zero)
3.  while the open list is not empty
    a) find the node with the least f on
       the open list, call it "q"
    b) pop q off the open list

    c) generate q's 8 successors and set their
       parents to q

    d) for each successor
        i) if successor is the goal, stop search

        ii) else, compute both g and h for successor
          successor.g = q.g + distance between
                              successor and q
          successor.h = distance from goal to
          successor (This can be done using many
          ways, we will discuss three heuristics-
          Manhattan, Diagonal and Euclidean
          Heuristics)

          successor.f = successor.g + successor.h
        iii) if a node with the same position as
            successor is in the OPEN list which has a
           lower f than successor, skip this successor
        iV) if a node with the same position as
            successor  is in the CLOSED list which has
            a lower f than successor, skip this successor
            otherwise, add  the node to the open list
     end (for loop)

    e) push q on the closed list
    end (while loop)
*/
void Graph::findShortestPathAStar(int startNode, int goalNode, const vector<vector<pair<int, int>>> &adjacencyList, vector<int> &shortestDistances)
{
    // 1. Initialize the open list
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> openList;

    // 2. Initialize the closed list
    unordered_set<int> visitedNodes;

    // Put the starting node on the open list with f = 0
    vector<double> gCost(adjacencyList.size(), numeric_limits<double>::max());
    vector<double> fCost(adjacencyList.size(), numeric_limits<double>::max());
    vector<int> parentNodes(adjacencyList.size(), -1);

    gCost[startNode] = 0.0;
    fCost[startNode] = 0.0;
    openList.emplace(0.0, startNode);

    // 3. While the open list is not empty
    while (!openList.empty())
    {
        // a) Find the node with the least f on the open list, call it "curr"
        auto [currentFCost, curr] = openList.top();
        openList.pop();

        // b) If curr is the goal, stop search
        if (curr == goalNode)
        {
            shortestDistances[goalNode] = gCost[goalNode];
            return;
        }

        // Push curr onto the visited list
        visitedNodes.insert(curr);

        // c) Generate curr's neighbors
        for (const auto &neighbor : adjacencyList[curr])
        {
            int neighborNode = neighbor.first;
            double edgeWeight = neighbor.second;

            // Skip if the neighbor is already in the visited list
            if (visitedNodes.find(neighborNode) != visitedNodes.end())
            {
                continue;
            }

            // d) Compute g, h, and f for the neighbor
            double tentativeGCost = gCost[curr] + edgeWeight;
            double heuristicCost = haversine(pins[neighborNode].latitude, pins[neighborNode].longitude,
                                             pins[goalNode].latitude, pins[goalNode].longitude);
            double tentativeFCost = tentativeGCost + heuristicCost;

            // If a better path to the neighbor is found, update it
            if (tentativeFCost < fCost[neighborNode])
            {
                gCost[neighborNode] = tentativeGCost;
                fCost[neighborNode] = tentativeFCost;
                parentNodes[neighborNode] = curr;

                // Add the neighbor
                openList.emplace(tentativeFCost, neighborNode);
            }
        }
    }

    // If we reach here, no path was found
    shortestDistances[goalNode] = -1; // Indicate no path exists
}