#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

bool comparePattern(vector<vector<string>>& T, vector<vector<string>>& P, int textRows, int textCols, int patternRows, int patternCols, int textStartRow, int textStartCol) {
    for (int i = 0; i < patternRows; i++) {
        for (int j = 0; j < patternCols; j++) {
            if (P[i][j] != T[textStartRow + i][textStartCol + j]) {
                return false;
            }
        }
    }
    return true;
}

bool isPatternExists(vector<vector<string>>& T, vector<vector<string>>& P, int textRows, int textCols, int patternRows, int patternCols) {
    for (int i = 0; i <= textRows - patternRows; ++i) {
        for (int j = 0; j <= textCols - patternCols; ++j) {
            if (comparePattern(T, P, textRows, textCols, patternRows, patternCols, i, j)) {
                return true;
            }
        }
    }
    return false;
}

int diagonalPatternExistence(vector<vector<string>>& T, vector<vector<string>>& P, int textRows, int textCols, int patternRows, int patternCols) {
    int count = 0;

        for (int i = 0; i <= textRows - patternRows; ++i) {
        for (int j = 0; j <= textCols - patternCols; ++j) {
            if (comparePattern(T, P, textRows, textCols, patternRows, patternCols, i, j)) {
                count++;
                i += patternRows - 1; 
                break;
            }
        }
    }
    if (count >= 2) {
        return count;
    }
    else {
        return 0;
    }
}

void splitString(const string& inputString, vector<vector<string>>& matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = inputString[i * size + j];
        }
    }
}

void parseLine(const string& line, vector<vector<string>>& matrix, int row) {
    stringstream ss(line);
    string token;
    int col = 0;
    while (getline(ss, token, ',') && col < matrix[row].size()) {
        matrix[row][col++] = token;
    }
}

int main() {
    ifstream inputFile("TestCase3.txt");
    if (!inputFile.is_open()) {
        cout << "Unable to open file!" << endl;
        return 1;
    }

    vector<vector<string>> matrixT;
    vector<vector<string>> matrixP;
    string pattern;
    string expectedResult;
    string line;
    int currentRow = 0;
    bool isFirstLine = true;
    bool isMatrixT = true;
    bool isPattern = false;

    while (getline(inputFile, line)) {
        if (line.empty()) {
            if (isMatrixT) {
                isMatrixT = false;
                continue;
            }
            isPattern = !isPattern;
            continue;
        }

        if (isMatrixT) {
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }
            matrixT.emplace_back();
            stringstream ss(line);
            string token;
            while (getline(ss, token, ',')) {
                matrixT[currentRow].push_back(token);
            }
            currentRow++;
        }
        else if (isPattern) {
            expectedResult = line;
            isPattern = false;
        }
        else {
            pattern += line; 
        }
    }

    inputFile.close();

    cout << "Text:" << endl;
    for (const auto& row : matrixT) {
        for (const auto& col : row) {
            cout << col << " ";
        }
        cout << endl;
    }

    int patternSize = static_cast<int>(sqrt(pattern.size()));
    matrixP.resize(patternSize, vector<string>(patternSize));
    splitString(pattern, matrixP, patternSize);

    cout << "Pattern:" << endl;
    for (const auto& row : matrixP) {
        for (const auto& col : row) {
            cout << col << " ";
        }
        cout << endl;
    }

    cout << "Expected Result: " << expectedResult << endl;
    
    bool patternExists = isPatternExists(matrixT, matrixP, matrixT.size(), matrixT[0].size(), matrixP.size(), matrixP[0].size());
    int count = diagonalPatternExistence(matrixT, matrixP, matrixT.size(), matrixT[0].size(), matrixP.size(), matrixP[0].size());

    if (patternExists) {
        cout << "Pattern exists in the text." << std::endl;
        cout << "Pattern occurs diagonally " << count << " times." << std::endl;
    }
    else {
        cout << "Pattern does not exist in the text." << std::endl;
    }
    return 0;
}

