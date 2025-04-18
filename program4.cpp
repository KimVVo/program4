#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

// Function to simulate FIFO page replacement
int simulateFIFO(const vector<int>& refString, int frames) {
    queue<int> pageQueue;
    unordered_set<int> pages;
    int pageFaults = 0;

    for (int page : refString) {
        if (pages.find(page) == pages.end()) {
            pageFaults++;
            if (pages.size() >= frames) {
                int oldest = pageQueue.front();
                pageQueue.pop();
                pages.erase(oldest);
            }
            pageQueue.push(page);
            pages.insert(page);
        }
    }
    return pageFaults;
}

// Function to simulate LRU page replacement
int simulateLRU(const vector<int>& refString, int frames) {
    vector<int> pageList;
    unordered_set<int> pages;
    int pageFaults = 0;

    for (int page : refString) {
        if (pages.find(page) == pages.end()) {
            pageFaults++;
            if (pages.size() >= frames) {
                int lruPage = pageList.front();
                pageList.erase(pageList.begin());
                pages.erase(lruPage);
            }
            pages.insert(page);
            pageList.push_back(page);
        } else {
            // Move page to the end (most recently used)
            pageList.erase(find(pageList.begin(), pageList.end(), page));
            pageList.push_back(page);
        }
    }
    return pageFaults;
}

// Function to simulate Random page replacement
int simulateRandom(const vector<int>& refString, int frames) {
    vector<int> pageList;
    unordered_set<int> pages;
    int pageFaults = 0;
    random_device rd;
    mt19937 gen(rd());

    for (int page : refString) {
        if (pages.find(page) == pages.end()) {
            pageFaults++;
            if (pages.size() >= frames) {
                uniform_int_distribution<> dis(0, pageList.size() - 1);
               int index = dis(gen);
int pageToRemove = pageList[index];
pageList.erase(pageList.begin() + index);
pages.erase(pageToRemove);
            }
            pages.insert(page);
            pageList.push_back(page);
        }
    }
    return pageFaults;
}

// Function to simulate Belady's Optimal page replacement
int simulateOptimal(const vector<int>& refString, int frames) {
    unordered_set<int> pages;
    vector<int> pageList;
    int pageFaults = 0;

    for (size_t i = 0; i < refString.size(); ++i) {
        int page = refString[i];
        if (pages.find(page) == pages.end()) {
            pageFaults++;
            if (pages.size() >= frames) {
                // Find the page that will not be used for the longest time
                unordered_map<int, size_t> nextUse;
                for (int p : pageList) {
                    nextUse[p] = refString.size();
                    for (size_t j = i + 1; j < refString.size(); ++j) {
                        if (refString[j] == p) {
                            nextUse[p] = j;
                            break;
                        }
                    }
                }
                // Find page with furthest next use
                int toRemove = pageList[0];
                size_t furthest = nextUse[toRemove];
                for (int p : pageList) {
                    if (nextUse[p] > furthest) {
                        furthest = nextUse[p];
                        toRemove = p;
                    }
                }
                pageList.erase(find(pageList.begin(), pageList.end(), toRemove));
                pages.erase(toRemove);
            }
            pages.insert(page);
            pageList.push_back(page);
        }
    }
    return pageFaults;
}

int main() {
    int frames = 3; // Default to 3 frames
    string input;
    vector<int> defaultRefString = {1, 2, 3, 4, 1, 5, 2, 4, 6, 7, 3, 1, 7, 5, 2, 6, 1, 3};
    vector<int> refString;

    // Input number of frames
    cout << "Enter the number of frames (default is 3): ";
    getline(cin, input);
    if (!input.empty()) {
        stringstream ss(input);
      
        ss >> frames;
    } else if (input.empty()) {
        cout << "Using default number of frames: " << frames << endl;
    }
    if (frames <= 0) {
        frames = 3;
        cout << "Invalid number of frames. Using default: " << frames << endl;
    }

    // Input reference string
    cout << "Enter the reference string (space-separated integers, press Enter for default): ";
    getline(cin, input);
    if (input.empty()) {
        refString = defaultRefString;
        cout << "Using default reference string: ";
        for (int page : refString) {
            cout << page << " ";
        }
        cout << endl;
    } else {
        // Replace commas with spaces
        replace(input.begin(), input.end(), ',', ' ');
        stringstream ss(input);
        int page;
        while (ss >> page) {
            refString.push_back(page);
        }
    }

    // Run simulations
    int fifoFaults = simulateFIFO(refString, frames);
    int lruFaults = simulateLRU(refString, frames);
    int randomFaults = simulateRandom(refString, frames);
    int optimalFaults = simulateOptimal(refString, frames);

    // Calculate and display page fault rates
    double totalReferences = refString.size();
    cout << "\nSimulation Results:\n";
    cout << "FIFO Page Faults: " << fifoFaults << ", Fault Rate: " << (fifoFaults / totalReferences) * 100 << "%\n";
    cout << "LRU Page Faults: " << lruFaults << ", Fault Rate: " << (lruFaults / totalReferences) * 100 << "%\n";
    cout << "Random Page Faults: " << randomFaults << ", Fault Rate: " << (randomFaults / totalReferences) * 100 << "%\n";
    cout << "Optimal Page Faults: " << optimalFaults << ", Fault Rate: " << (optimalFaults / totalReferences) * 100 << "%\n";

    // Compare and analyze performance
    cout << "\nPerformance Comparison:\n";

    // Store fault data in a vector of pairs
    vector<pair<string, int>> results = {
        {"FIFO", fifoFaults},
        {"LRU", lruFaults},
        {"Random", randomFaults},
        {"Optimal", optimalFaults}
    };

    // Sort by number of faults (ascending)
    sort(results.begin(), results.end(), [](auto& a, auto& b) {
        return a.second < b.second;
    });

    // Display sorted results
    for (auto& result : results) {
        string name = result.first;
        int faults = result.second;
        double rate = (faults / totalReferences) * 100;
        cout << name << ": " << faults << " faults, Fault Rate: " << rate << "%\n";
    }
    

    // Highlight the best performing algorithm
cout << "\nBest Performance: " << results[0].first
     << " with only " << results[0].second << " page faults.\n";

// Special note if Optimal is not the best
if (results[0].first != "Optimal") {
    cout << "Note: Although Optimal is the theoretical best (because it looks ahead), it was outperformed in this run.\n";
}

// Performance Analysis
cout << "\nPerformance Analysis:\n";
for (size_t i = 0; i < results.size(); ++i) {
    string name = results[i].first;
    int faults = results[i].second;
    double rate = (faults / totalReferences) * 100;

    if (name == "Optimal") {
        cout << "- Optimal: Ideal performance (used for benchmarking others).\n";
    } else if (name == "LRU") {
        cout << "- LRU: Performs well by using recent access history. It had ";
        if (faults == optimalFaults) cout << "equal performance to Optimal.\n";
        else cout << "a fault rate of " << rate << "%.\n";
    } else if (name == "FIFO") {
        cout << "- FIFO: Simple but can suffer from Belady's anomaly. Fault rate: " << rate << "%.\n";
    } else if (name == "Random") {
        cout << "- Random: Unpredictable, performance can vary. Fault rate: " << rate << "% (may differ each run).\n";
    }
}

    return 0;
}