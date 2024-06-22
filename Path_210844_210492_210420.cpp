#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_NODES = 26; // Assuming nodes are labeled A-Z

int readDistanceData(const  string& filePath, int distances[MAX_NODES][MAX_NODES]) {
     ifstream file(filePath);
     int rows = 0;
    if (file.is_open()) {
        char node1, node2;
         string distance;
         string line;
         

        while (getline(file, line)) {
            int count = 0;
            distance.clear(); // Clear the distance string for each line
            rows++;
            for (int i = 0; line[i] != '\0'; i++) {
                if (count == 0)
                    node1 = line[i];
                if (count == 2)
                    node2 = line[i];
                if (count > 3)
                    distance += line[i];
                count++;
            }

            if (count == 0)
                break;

            int dist =  stoi(distance);
            distances[node1 - 'A'][node2 - 'A'] = dist;
            distances[node2 - 'A'][node1 - 'A'] = dist;
             cout << node1 << " " << node2 << " " << dist <<  endl;
        }
        file.close();
    }
    else {
         cout << "Unable to open file: " << filePath <<  endl;
    }
    rows++;

    return rows;

}


int readPaths(const  string& filePath, int distances[MAX_NODES][MAX_NODES], int rowCount, float& paths) {
    ifstream file(filePath);
    int totalSumOfTimes = 0;
    if (file.is_open()) {
        char node1, node2;
        string line;
        int row = 0;
        

        while (getline(file, line)) {
            row++;
            if (row >= rowCount) {
                int count = 0;
                for (int i = 0; line[i] != '\0'; i++) {
                    if (count == 0)
                        node1 = line[i];
                    else if (count == 2) {
                        node2 = line[i];
                        totalSumOfTimes += distances[node1 - 'A'][node2 - 'A'];
                    }
                    else if (count % 2 == 0) {
                        node1 = node2;
                        node2 = line[i];
                        totalSumOfTimes += distances[node1 - 'A'][node2 - 'A'];
                    }
                    count++;
                }

                if (count == 0)
                    break;
                paths++;
            }
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filePath << endl;
    }
    cout << "Total number of paths: " << paths << endl;
    return totalSumOfTimes;
}



int main() {
    const string filePath = "testcase2.txt";  // Replace with the actual path to your file
    float paths = 0;
    int distances[MAX_NODES][MAX_NODES] = { 0 };
    int row = readDistanceData(filePath, distances);
    int totalSum = readPaths(filePath, distances, row, paths);
    cout << "Total sum of path is: " << totalSum << endl;
    float avgTimeBetweenLocations = totalSum / paths;
    cout << "Average Time to move between locations is: " << avgTimeBetweenLocations << endl;
    return 0;
}
