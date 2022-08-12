# **Recommender System with Collaborative Filtering**

## **Summary**
The goal of this project is to **predict the ratings of movies** in the provided test dataset using the given training data which contains movie ratings of users.

An **item-based collaborative filtering** approach is implemented to predict ratings for each user-item pair.

> The basic principle of this approach is that it calculates the rating of a movie based on ratings of similar items, i.e., **similarities between movies** are utilized to generate recommendations.

In the recommender system, the procedure is stepwise: first, users rate different items, and then the system makes predictions about the user’s ratings for an item that has not yet been rated. Thus, the new predictions are built upon the existing ratings based on ratings of similar items. 

To predict the rating **R** that an active user **A** would very likely give to a movie **M**, the approach includes:
1. Finding the most similar (i.e., nearest) movies to movie **M**
2. Calculating the rating **R** using the weighted average of the ratings for the most similar movies by user **A**

A **rating matrix** is implemented to store the information of each user-item pair. Each entry represents the rating for a movie **M** given by a user **A**. In this implementation, **cosine similarity** is used as the metric to compute the similarity between two movies. Mathematically, it measures the cosine of the angle between two vectors projected in a multi-dimensional space. The output value ranges from 0 to 1—0 means both vectors are dissimilar whereas 1 means both vectors are identical.

<br/>

## **Compilation Method**

The recommender system is implemented in **Linux Ubuntu 20.04** environment. A Makefile is provided for compiling convenience.

All codes are compiled using GCC with the following input at command prompt. <br/>

    make

To remove all files for recompilation, the following input can be entered at command prompt. <br/>

    make clean

The compiled code can be executed with the following format. <br/>

    ./recommender.exe [training file] [test file]

Example: <br/>

    ./recommender.exe u1.base u1.test