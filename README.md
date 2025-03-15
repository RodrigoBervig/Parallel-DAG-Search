# Parallel Graph Search on a DAG

## Project Overview
This project is part of the **Distributed and Parallel Programming** course at **Instituto de Informática - UFRGS**. Our goal is to implement a parallelized version of a given algorithm to improve execution time. The project follows the requirements defined in **Trabalho1_PDP.pdf** and is based on **Problem F** from the 15th Marathon of Parallel Programming WSCAD-2020, as described in **problemset.pdf**.

## Problem Statement
We selected **Problem F: Graph Search on a DAG** from [WSCAD Marathon Problems](http://lspd.mackenzie.br/marathon/20/problems.html). The problem involves computing a **maximal value search on a Directed Acyclic Graph (DAG)**. Given a set of nodes with associated values and edges representing dependencies, the algorithm recursively finds the highest possible sum starting from a given node.

### **Input Format**
- The first line contains:
  - An integer **N**: the number of entries in the file.
  - A list of node IDs for which the computation should be performed.
- The following lines define the graph structure, where each line contains:
  - **Node ID** (integer)
  - **Node value** (float)
  - **A list of neighbor Node IDs** (variable length)

### **Output Format**
For each node ID in the list to be computed, the program outputs:
```
<Node ID>: <Computed Value>
```
