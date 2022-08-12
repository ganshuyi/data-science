# **Decision Tree Algorithm**

## **Summary**
The goal of this assignment is to construct a **decision tree** based on a given training data, and then utilize the decision tree to classify samples in a test dataset.

> Decision tree induction is a greedy algorithm that constructs a decision tree in a **top-down recursive divide-and-conquer** manner.

The key idea is to use a decision tree to partition the data space into cluster (dense) regions and empty (sparse) regions. The algorithm iteratively splits the tree on the attribute that results in the maximum **gain ratio**, until the data is clustered into sufficiently homogenous groups. At the end of the training, a learned decision tree is returned to be used to make optimal categorized predictions.

The following is a detailed rundown of how the decision tree algorithm works:
1. The algorithm begins with all the training examples in training set D at the root node.
2. The splitting test attribute is selected on the basis of a heuristic (i.e., gain ratio).
3. Training examples are branched into subsets for each possible outcome of the selected splitting attribute.
4. This process is repeated recursively for each branch, using only samples that reach the branch.
5. The algorithm is terminated when one of the following stopping criterions are met:
    - All samples for a given node belongs to the same class.
    - There are no remaining attributes for further partitioning. Majority voting is employed to classify the leaf.
    - There are no samples left
<br/><br/>


## **Compilation Method**

The decision tree is implemented in **Linux Ubuntu 20.04** environment. A Makefile is provided for compiling convenience.

All codes are compiled using GCC with the following input at command prompt. <br/>

    make

To remove all files for recompilation, the following input can be entered at command prompt. <br/>

    make clean

The compiled code can be executed with the following format. <br/>

    ./dt.exe [training file] [test file] [output file]

Example: <br/>

    ./dt.exe dt_train.txt dt_test.txt dt_result.txt