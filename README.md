<H1> Glacier Guides - PathFinding for Rigorous Skiing </h1>

<h2> Table of Contents </h2>

- [Problem Overview and Program Introduction](#problem-overview-and-program-introduction)
- [Start-Up Instructions](#start-up-instructions)
- [File Structure](#file-structure)
- [Contributors](#contributors)
- [Demo Video](#demo-video)
- [Additional Resources and Documentation:](#additional-resources-and-documentation)
  - [Downloadable External LibrariesUtilized](#downloadable-external-libraries-utilized)

# Problem Overview and Program Introduction

Ski mountaineers, heli-skiers, and backcountry skiers often face difficulties in finding the most safe paths and trails as the conditions are ever so changing. Although GPS apps already exist for mountain hikers, there are none that account for the steep slopes, elevation and altitude changes, avalanche risk, and weather conditions for skiers. With our project we are aiming to optimize navigation though mountainous terrain for skiers utilizing graph-based pathfinding algorithms.

These routes are rough, dynamic terrains where skiers need to prioritize safety, optimize on time efficiency, and conserve energy. When finding the best route they must balance these factors which can be difficult to manage when they can change overnight.
 
Glacier Guiders is a C++ application that takes parses in data from OpenSkiMap and utilizes this terrain and elevation data to find the shortest, but also safest route (by accounting for a maximum elevation).
Therefore, this application balances efficiency with safety to give common and off-roads skiiers a useful tool for making the most out of their experience on the slopes.  We will compare Dijkstra’s Algorithm and A* Algorithms to determine which routes are most effective at certain times of the year for the best outcome for these skiers.

# Start-Up Instructions:

For our repository you should follow the standard procedure to clone this repository and pull it to your local.  We would reccomend installing SFML from the official site that is linked in our "Downloadable External Libraries or Repositories Utilized" section link two.  For an alternative implementation of SFML, the vcpkg submodule can be downloaded as a submodule to your local repository and then must be manually linked via terminal.  The necessary pugixml library files are included in the resources folder so no additional work is needed here to set up that parsing library.  I hope you enjoy our Glacier-Guiders program!

# File Structure:

## Glacier-Guides
  ### resources
    #### SkMap Data
    #### images
    #### vcpkg
    #### Graph.cpp
    #### Graph.h
    #### TerrainParser.cpp
    #### TerrainParser.h
    #### main.cpp
    #### map.osm
    #### pugiconfig.hpp
    #### pugixml.cpp
    #### pugixml.hpp
    #### MARKDOWN.md
### CMakeLists.txt
### ReadMe.md

# Contributors:

1. Brody Grissom (@bgriss11)
2. Allyson Greenberg (@allgreenberg8)

# Demo Video:

https://youtu.be/RfRKfnOuLCQ?si=_6IBxXFurFIFi-y0

# Additional Resources and Documentation:

#### Downloadable External Libraries or Repositories Utilized:

- [Pugixml](https://pugixml.org/)
- [SFML](https://www.sfml-dev.org/tutorials/1.6/start-vc.php)
- [vcpkg Public Repository (alternative download option for SFML](https://github.com/microsoft/vcpkg)
