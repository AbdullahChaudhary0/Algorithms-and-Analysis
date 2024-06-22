#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

int main()
{
    const string filePath = "testcase3.txt";  // Replace with the actual path to your file
    string line;
    ifstream file(filePath);
    if (file.is_open()) {
        string numVertex;

        while (getline(file, line)) {
            numVertex += line[0];
            break;
        }
        file.close();
    }
    else {
        cout << "Unable to open file: " << filePath << endl;
    }
    file.close();

    int n = stoi(line);  // Number of vertex
    list<int>* arrayLinkedList = new list<int>[n];
    for (int i = 0; i < n; ++i) {
        arrayLinkedList[i].push_back(i);
    }

    line = '\0';
    ifstream file1(filePath);
    if (file1.is_open()) {
        string vertex1;
        string vertex2;
        int rows = 0;

        while (getline(file1, line)) {
            int count = 0;
            if (rows > 0) {
                for (int i = 0; line[i] != '\0'; i++) {
                    vertex1 = line[0];
                    vertex2 = line[2];
                    count++;
                }
                int ver1 = stoi(vertex1);
                int ver2 = stoi(vertex2);
                // Traverse each linked list
                for (int j = 0; j < n; j++) {
                    // Check if ver1 is present in the list
                    auto it = find(arrayLinkedList[j].begin(), arrayLinkedList[j].end(), ver1);
                    if (it != arrayLinkedList[j].end()) {
                        // ver1 is present, add ver2 to the end of the list
                        arrayLinkedList[j].push_back(ver2);
                    }
                }
            }
            if (rows > 1) {
                if (count == 0)
                    break;
            }
            rows++;
        }
        file1.close();
    }
    else {
        cout << "Unable to open file: " << filePath << endl;
    }
    file1.close();
    int sizeCycle = INT_MAX;
    int cycleNum = 0;
    bool flag = 0;
    for (int i = 0; i < n; ++i) {
        int count = 0;
        for (auto it = next(arrayLinkedList[i].begin()); it != arrayLinkedList[i].end(); ++it) {
            if (*it == arrayLinkedList[i].front()) {
                flag = 1;
                if (sizeCycle > count + 1) {
                    sizeCycle = count + 1;
                    cycleNum = i;
                }
                break;
            }
            count++;
        }
    }
    if (flag) {
        cout << "Size of the shortest cycle is: " << sizeCycle << endl;
        cout << "The shortest cycle of this network is: ";
        auto it = arrayLinkedList[cycleNum].begin();
        auto end = arrayLinkedList[cycleNum].end();
        while (it != end) {
            cout << *it;
            ++it;
            if (it != end) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    if (!flag) {
        cout << "-1" << endl;
    }

    return 0;
}
