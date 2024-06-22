#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

using namespace std;

struct RectSize {
    int width1, height1, width2, height2;
};

void readDataFromFile(const string& filename, vector<RectSize>& sizes, vector<int>& values, int& numSizes) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    regex pattern("\\((-?\\d+),(-?\\d+)\\),\\((-?\\d+),(-?\\d+)\\),\\{(-?\\d+)\\}|\\((-?\\d+),(-?\\d+)\\),\\{(-?\\d+)\\}");

    string line;
    getline(file, line);
    numSizes = stoi(line);

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

                sizes.push_back({ width1, height1, width2, height2 });
                values.push_back(value);
            }
            else {
                width1 = stoi(match[6].str());
                height1 = stoi(match[7].str());
                value = stoi(match[8].str());

                sizes.push_back({ width1, height1, -1, -1 }); // Assuming -1 for width2, height2 as default values
                values.push_back(value);
            }
        }
        else {
            cerr << "Invalid line format: " << line << endl;
        }
    }

    file.close();
}

void printOptimalSizes(const vector<RectSize>& sizes, const vector<int>& optimalSelection) {
    size_t i = 0;
    while (i < optimalSelection.size()) {
        int index = optimalSelection[i];

        cout << sizes[index].width1 << " x " << sizes[index].height1;

        if (sizes[index].width2 != -1) {
            cout << " + " << sizes[index].width2 << " x " << sizes[index].height2;
        }

        if (++i < optimalSelection.size()) {
            cout << " + ";
        }
    }
}

void findOptimalSelections(const vector<RectSize>& sizes, const vector<int>& values, int numSizes) {
    vector<int> maxValues(numSizes + 1, 0);
    vector<vector<int>> optimalSelections(numSizes + 1);

    int i = 1;
    while (i <= numSizes) {
        int maxValue = 0;
        int optimalIndex = -1;

        size_t j = 0;
        while (j < sizes.size()) {
            int area = sizes[j].width1 * sizes[j].height1;
            if (area <= i) {
                int currentMaxValue = values[j] + maxValues[i - area];

                if (currentMaxValue > maxValue) {
                    maxValue = currentMaxValue;
                    optimalIndex = j;
                }
            }
            ++j;
        }

        maxValues[i] = maxValue;

        if (optimalIndex != -1) {
            optimalSelections[i] = optimalSelections[i - sizes[optimalIndex].width1 * sizes[optimalIndex].height1];
            optimalSelections[i].push_back(optimalIndex);
        }

        ++i;
    }

    cout << "Maximum Value: " << maxValues[numSizes] << endl;
    cout << "Optimal Selections: ";
    printOptimalSizes(sizes, optimalSelections[numSizes]);
    cout << " = " << maxValues[numSizes] << "k" << endl;
}

int main() {
    vector<RectSize> sizes;
    vector<int> values;
    int numSizes;
    vector<RectSize> optimalSizes;
    readDataFromFile("TestCase3.txt", sizes, values, numSizes);

    if (!sizes.empty()) {
        findOptimalSelections(sizes, values, numSizes);
    }

    return 0;
}
