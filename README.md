# Virtual Memory Paging Simulation

This project simulates four classic page replacement algorithms—**FIFO**, **LRU**, **Random**, and **Optimal**—to compare their performance in terms of page faults.

## 📘 Overview

In operating systems, virtual memory allows programs to use more memory than is physically available. When a needed page is not in memory, a **page fault** occurs, and a **page replacement algorithm** decides which memory page to swap out.

This simulation helps visualize how different algorithms perform given the same reference string and number of frames. Four algorithms—FIFO, LRU, Random Replacement, and Belady's Optimal Algorithm—will be implemented in this simulation.

## 🚀 Method

- User input for number of page frames (default: 3)
- Default or custom reference string input (default: `1 2 3 4 1 5 2 4 6 7 3 1 7 5 2 6 1 3`)
- Simulates and compares:
  - FIFO (First-In-First-Out)
  - LRU (Least Recently Used)
  - Random Replacement
  - Belady's Optimal Algorithm
- Output:
  - Page fault count and rate for each algorithm
  - Sorted comparison from best to worst
  - Analyze algorithms' performance and efficiency.

## 🛠️ How To Run

- g++ -o program4 program.cpp
- ./program4

