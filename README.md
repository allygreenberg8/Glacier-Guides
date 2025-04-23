<H1> Glacier Guides - PathFinding for Rigorous Skiing </h1>

<h2> Table of Contents </h2>

- [Problem Overview and Program Introduction](#problem-overview-and-program-introduction)
	- [Problem Overview and Program Introduction](#getting-started-with-the-project)
		- [Step 1: Create Your Project Repository](#step-1-create-your-project-repository)
		- [Step 2: Repository Name and Visibility](#step-2-repository-name-and-visibility)
		- [Step 3: Collaboration and Team Members](#step-3-collaboration-and-team-members)
		- [Step 4: Development](#step-4-development)
			- [Issues:](#issues)
			- [Branches](#branches)
			- [Commits](#commits)
				- [Guidelines for good commit messages](#guidelines-for-good-commit-messages)
			- [Pull Requests](#pull-requests)
	- [Markdown and README](#markdown-and-readme)
	- [Additional Resources and Documentation:](#additional-resources-and-documentation)
		- [General Documentation](#general-documentation)
		- [Additional Specific Documentation](#additional-specific-documentation)
			- [Quickstart](#quickstart)
			- [Repositories](#repositories)
			- [Branches](#branches-1)
			- [Issues](#issues-1)
			- [Bonus Documentation](#bonus-documentation)

# Problem Overview and Program Introduction

Ski mountaineers, heli-skiers, and backcountry skiers often face difficulties in finding the most safe paths and trails as the conditions are ever so changing. Although GPS apps already exist for mountain hikers, there are none that account for the steep slopes, elevation and altitude changes, avalanche risk, and weather conditions for skiers. With our project we are aiming to optimize navigation though mountainous terrain for skiers utilizing graph-based pathfinding algorithms.

 These routes are rough, dynamic terrains where skiers need to prioritize safety, optimize on time efficiency, and conserve energy. When finding the best route they must balance these factors which can be difficult to manage when they can change overnight.
 
Glacier Guiders is a C++ application that takes parses in data from OpenSkiMap and OpenTopography.org and utilizes this terrain and elevation data to find the shortest, but also safest route (by accounting for a maximum elevation).
Therefore, this application balances efficiency with safety to give common and off-roads skiiers a useful tool for making the most out of their experience on the slopes.  We will compare Dijkstra’s Algorithm and A* Algorithms to determine which routes are most effective at certain times of the year for the best outcome for these skiers.


## Additional Resources and Documentation:

#### Downloadable External Libraries or Repositories Utilized:

- [Pugixml](https://pugixml.org/)
- [SFML](https://www.sfml-dev.org/tutorials/1.6/start-vc.php)
- [vcpkg Public Repository (alternative download option for SFML](https://github.com/microsoft/vcpkg)
- [READMEs](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/about-readmes)
- [Repo visibility](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/managing-repository-settings/setting-repository-visibility)
- [Teams and Collaboration](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/managing-repository-settings/managing-teams-and-people-with-access-to-your-repository)
- [Working with Files](https://docs.github.com/en/repositories/working-with-files)
