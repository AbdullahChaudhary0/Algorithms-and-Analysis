#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

struct RectSize {
    int width1, height1, width2, height2;
};

void printOptimalDimensions(const vector<RectSize>& dimensions, const vector<int>& optimalIndices) {
    size_t i = 0;
    while (i < optimalIndices.size()) {
        if (i > 0) {
            cout << " + ";
        }
        const RectSize& dim = dimensions[optimalIndices[i]];
        cout << dim.width1 << "x" << dim.height1;
        i++;
    }
}

void findOptimalDivisions(const vector<RectSize>& dimensions, const vector<int>& prices, int n) {
    vector<int> dp(n + 1, 0);
    vector<int> optimalIndices(n + 1, -1);

    int i = 1;
    while (i <= n) {
        int maxProfit = 0;
        int optimalIndex = -1;

        size_t j = 0;
        while (j < dimensions.size()) {
            int size = dimensions[j].width1 * dimensions[j].height1;
            if (size <= i) {
                int currentProfit = prices[j] + dp[i - size];

                if (currentProfit > maxProfit) {
                    maxProfit = currentProfit;
                    optimalIndex = j;
                }
            }
            j++;
        }

        dp[i] = maxProfit;
        optimalIndices[i] = optimalIndex;
        i++;
    }

    cout << "Maximum Profit: " << dp[n] << endl;
    cout << "Optimal Divisions: ";

    // Printing
    int currentSize = n;
    while (currentSize > 0) {
        int index = optimalIndices[currentSize];
        const RectSize& dim = dimensions[index];
        cout << dim.width1 << "x" << dim.height1;
        currentSize -= dim.width1 * dim.height1;
        if (currentSize > 0) {
            cout << " + ";
        }
    }
    cout << " = " << dp[n] << endl;
}

void readDataFromFile(const string& filename, vector<RectSize>& dimensions, vector<int>& prices, int& n) {
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
            int width1, height1, width2, height2, value;

            if (match[5].matched) {
                width1 = stoi(match[1].str());
                height1 = stoi(match[2].str());
                width2 = stoi(match[3].str());
                height2 = stoi(match[4].str());
                value = stoi(match[5].str());

                dimensions.push_back({ width1, height1, width2, height2 });
                prices.push_back(value);
            }
            else {
                width1 = stoi(match[6].str());
                height1 = stoi(match[7].str());
                value = stoi(match[8].str());

                dimensions.push_back({ width1, height1, -1, -1 }); // Assuming -1 for width2, height2 as default values
                prices.push_back(value);
            }
        }
        else {
            cerr << "Invalid line format: " << line << endl;
        }
    }

    file.close();
}

int main() {
    vector<RectSize> dimensions;
    vector<int> prices;
    int n;

    readDataFromFile("TestCase3.txt", dimensions, prices, n);

    if (!dimensions.empty()) {
        findOptimalDivisions(dimensions, prices, n);
    }

    return 0;
}
