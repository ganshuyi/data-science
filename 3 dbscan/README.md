# **DBSCAN Algorithm**

## **Summary**
The goal of this experiment is to perform clustering on a given data set by using **DBSCAN algorithm**.
> DBSCAN stands for Density-Based Spatial Clustering of Applications with Noise, which is a density-based clustering algorithm that works on the assumption that clusters are dense regions in space separated by regions of lower density.

The algorithm groups densely grouped data objects into a single cluster, by utilizing the local density of the data objects. There are two key parameters of DBSCAN:
- **Eps**: The distance that specifies the neighbourhoods. Two objects are considered to be neighbours if the Euclidean distance between them is less than or equal to eps.
- **minPts**: The minimum number of data objects to define a cluster.

Using both parameters, all objects in the data file are classified into the following groups:
- **Core object**: An object is considered a core object if there are at least [minPts] number of objects (including itself) in its surrounding are with radius [eps].
- **Border object**: An object is considered a border object if it is reachable from a core object and there are less than [minPts] number of objects within its surrounding area.
- **Outlier**: An object is considered an outlier if it is not a core object and is not reachable from any core objects.

The following is a detailed rundown of how the DBSCAN algorithm works:
1. The parameters [minPts] and [eps] are determined.
2. An initial object is selected at random, and its neighbourhood area is determined using radius [eps]. If there are at least [minPts] number of data objects in its vicinity, the object is marked as **core object** and a cluster formation starts. Else, the object is marked as an **outlier**. Once a cluster formation (e.g., cluster A) is started, all objects within the neighbourhood of the initial object become a part of cluster A. If these objects are also core objects, the objects that are in the vicinity of them are also added to cluster A. Note that an object previously marked as outlier may be revisited and be included in a cluster.
3. An object which was not visited before in the previous steps is chosen by random, and step 2 is repeated.
4. The algorithm terminates when all objects are visited.

<br/>

## **Compilation Method**

The DBSCAN algorithm is implemented in **Linux Ubuntu 20.04** environment. A Makefile is provided for compiling convenience.

All codes are compiled using GCC with the following input at command prompt. <br/>

    make

To remove all files for recompilation, the following input can be entered at command prompt. <br/>

    make clean

The compiled code can be executed with the following format. <br/>

    ./clustering.exe [input data file] [N] [Eps] [MinPts]

Example: <br/>

    ./clustering.exe input1.txt 8 15 22