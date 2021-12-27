Summary
=======

This program finds the fastest way to travel from a location at the top of a MxN grid and exit at a location at the bottom
of the grid using Dijkstra's path finding algorithm.

Insights
========

**Grid to Graph**

The algorithm treats the MxN input grid as a unidirectional graph with each vertex having up to 4 edges (minimum of 2 for corner vertices)
with weights equivalent to the value at the index of the MxN grid. For example, in the input grid below the vertex of location (1,1)
has 4 edges of weights 6 connecting to (0,1), (2,1), (1,0), (1,2).

**Dijkstra's Algorithm**

Using this grid to graph conversion, the program implements modified version of Dijkstra's algorithm based on a minimum binary heap priority queue
of the vertices. The priority queue is constructed based on the minimum path distance of each explored vertices in mind.

**Example:**

Input Grid
```
3 0 7 1 7                                                                                                                 
7 6 0 8 0                                                                                                                 
3 0 6 4 5                                                                                                                 
0 5 0 4 4                                                                                                                 
7 3 1 8 3            
```
Fastest Path: Start Node (0,1) to Exit Location (4,2): 

```
(0,1) -> (1,1) -> (2,1) -> (3,1) -> (3,2) -> (4,2)
```


Usage
=====

This program expects a binary file MxN grid input passed in to the first argument and in return creates three outputs: text file grid translation,
binary file of shorest paths from each starting location to exit location, binary file of overall shortest path time and number of steps as well as
coordinates for each step in the path.

```console
foo@bar:~$ ./dijkstra-grid binary_grid.b text_grid.txt fastest_times.b fastest_path.b    
```

Algorithmic References
======================

* [Dijkstra's Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
* [Priority Queue](https://en.wikipedia.org/wiki/Priority_queue)
* [Binary Heap](https://en.wikipedia.org/wiki/Binary_heap)
