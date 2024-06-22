#include <iostream>
#include <vector>
#include <regex>
#include <fstream>

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

int findOptimalDivisions(const vector<Dimension>& dimensions, const vector<int>& prices, int n, vector<Dimension>& optimalDimensions, vector<vector<int>>& memo) {
    if (n == 0 || n % 100 != 0) {
        return 0;
    }

    if (memo[n / 100][n % 100] != -1) {
        return memo[n / 100][n % 100];
    }

    int maxProfit = 0;
    int selectedDimension = -1;

    int i = 0;

    while (i < dimensions.size()) {
        int size = dimensions[i].x1 * dimensions[i].y1;
        if (size <= n) {
            vector<Dimension> currentDimensions;
            int currentProfit = prices[i] + findOptimalDivisions(dimensions, prices, n - size, currentDimensions, memo);

            if (currentProfit > maxProfit) {
                maxProfit = currentProfit;
                selectedDimension = i;
                optimalDimensions = move(currentDimensions);
            }
        }
        i++;
    }

    if (selectedDimension != -1) {
        optimalDimensions.push_back(dimensions[selectedDimension]);
    }

    memo[n / 100][n % 100] = maxProfit;
    return maxProfit;
}

int main() {
    vector<Dimension> dimensions;
    vector<int> prices;
    int n;

    readDataFromFile("TestCase3.txt", dimensions, prices, n);
    //int maxN = 10000;
    vector<vector<int>> memo((n / 100) + 1, vector<int>(100, -1));
    vector<Dimension> optimalDims;

    int maxProfitValue = findOptimalDivisions(dimensions, prices, n, optimalDims, memo);

    cout << "Maximum Profit: " << maxProfitValue << endl;
    cout << "Optimal Dimensions: ";

    size_t i = 0;
    while (i < optimalDims.size()) {
        cout << optimalDims[i].x1 << "x" << optimalDims[i].y1 << " ";
        i++;
    }
    cout << endl;

    return 0;
}
