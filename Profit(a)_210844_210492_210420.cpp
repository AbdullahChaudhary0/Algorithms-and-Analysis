#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

using namespace std;

struct Dimension {
    int x1, y1, x2, y2;
};

void readDataFromFile(const string& filename, vector<Dimension>& dimensions, vector<int>& prices, int& n) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    regex pattern("\\((-?\\d+),(-?\\d+)\\),\\((-?\\d+),(-?\\d+)\\),\\{(-?\\d+)\\}|\\((-?\\d+),(-?\\d+)\\),\\{(-?\\d+)\\}");

    string line;
    getline(file, line);
    n = stoi(line);

    while (getline(file, line)) {
        smatch match;
        if (regex_match(line, match, pattern)) {
            int x1, y1, x2, y2, value;

            if (match[5].matched) {
                x1 = stoi(match[1].str());
                y1 = stoi(match[2].str());
                x2 = stoi(match[3].str());
                y2 = stoi(match[4].str());
                value = stoi(match[5].str());

                dimensions.push_back({ x1, y1, x2, y2 });
                prices.push_back(value);
            }
            else {
                x1 = stoi(match[6].str());
                y1 = stoi(match[7].str());
                value = stoi(match[8].str());

                dimensions.push_back({ x1, y1, -1, -1 }); // Assuming -1 for x2, y2 as default values
                prices.push_back(value);
            }
        }
        else {
            cerr << "Invalid line format: " << line << endl;
        }
    }

    file.close();
}

int calculateMaximumProfit(const vector<Dimension>& dimensions, const vector<int>& prices, int n, vector<Dimension>& optimalDimensions) {
    if (n == 0 || n % 100 != 0) {
        return 0;
    }

    int maxProfit = 0;
    int selectedDimension = -1;
    int i = 0; // Change for loop into while loop

    while (i < dimensions.size()) {
        int size = dimensions[i].x1 * dimensions[i].y1;
        if (size <= n) {
            vector<Dimension> currentDimensions;
            int currentProfit = prices[i] + calculateMaximumProfit(dimensions, prices, n - size, currentDimensions);

            if (currentProfit > maxProfit) {
                maxProfit = currentProfit;
                selectedDimension = i;
                optimalDimensions = move(currentDimensions);
            }
        }
        i++; // Increment the loop variable
    }

    if (selectedDimension != -1) {
        optimalDimensions.push_back(dimensions[selectedDimension]);
    }

    return maxProfit;
}

void printOptimalDimensions(const vector<Dimension>& optimalDimensions) {
    size_t i = 0;

    while (i < optimalDimensions.size()) {
        cout << optimalDimensions[i].x1 << " x " << optimalDimensions[i].y1;

        if (optimalDimensions[i].x2 != -1 && optimalDimensions[i].y2 != -1) {
            cout << " + " << optimalDimensions[i].x2 << " x " << optimalDimensions[i].y2;
        }

        if (i < optimalDimensions.size() - 1) {
            cout << " + ";
        }

        i++; // Increment the loop variable
    }
}

int main() {
    vector<Dimension> myDimensions;
    vector<int> myPrices;
    int myN;
    vector<Dimension> myOptimalDimensions;
    readDataFromFile("TestCase1.txt", myDimensions, myPrices, myN);

    int myMaximumProfit;
    int o = 0;
    if (!myDimensions.empty()) {
        myMaximumProfit = calculateMaximumProfit(myDimensions, myPrices, myN, myOptimalDimensions);
        o = myMaximumProfit;
    }

    cout << "Maximum Profit: " << o << endl;
    cout << "Optimal Dimensions: ";
    printOptimalDimensions(myOptimalDimensions);

    return 0;
}
