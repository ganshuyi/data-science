# **Apriori Algorithm**

## **Summary**
The goal of the assignment is to obtain association rules of a given input datafile by using the **Apriori algorithm**.

This program performs association rule learning through two steps using support and confidence metrics. The first step is to find frequent set of items in the given input datafile, i.e., at least the value of minimum support (%). This is achieved through the Apriori algorithm. The given input file is processed by inserting each transaction into C++ STL vector form.

The following is the rundown of how the Apriori algorithm works:
1. Generate candidate itemset with size (k+1) from all transactions. For the 1st iteration, this process returns a unique itemset, i.e., no duplicate items.
2. Generate frequent itemset from the candidate itemset by calculating the support for each item and remove items that do not satisfy the minimum support.
3. Repeat until no candidate itemset that satisfies the minimum threshold can be found.

After obtaining the frequent itemset which satisfies the minimum support, the program generates association rules **X → Y**, which is an implication of two itemset, for which there is a direct relationship between the specific items in these sets. For that, the support and confidence of each rule has to be computed. All the possible association rules and their respective value of support and confidence are inserted into a tuple format, to be returned as the output of the program.
<br/>

**Note:** <br/>
> Support and confidence can be computed with the following formulas: <br/>
    >- Support (A) = Transactions(A) / Total Transactions * 100
    >- Confidence (A → B) = Probability (A & B) / Support (A)

<br/>

## **Compilation Method**

The Apriori algorithm is implemented in **Linux Ubuntu 20.04** environment. A Makefile is provided for compiling convenience.

All codes are compiled using GCC with the following input at command prompt. <br/>

    make

To remove all files for recompilation, the following input can be entered at command prompt. <br/>

    make clean

The compiled code can be executed with the following format. <br/>

    ./apriori.exe [minimum support] [input file] [output file]

Example: <br/>

    ./apriori.exe 5 input.txt output.txt