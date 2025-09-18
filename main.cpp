/*
COMSC-210 | Lab 12 | Rylan Der
IDE Used: Visual Studio Code
*/

#include <array>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>

using namespace std;

template<typename T, size_t size>
void fillArray(array<T, size>& arr, string filename);

template<typename T, size_t size>
void displayArray(const array<T, size>& arr);

int main() {
    array<int, 30> nums;
    const string FILENAME = "data.txt";
    fillArray(nums, FILENAME);
    displayArray(nums);

}

template<typename T, size_t size>
void fillArray(array<T, size>& arr, string filename) {
    ifstream infile;
    infile.open(filename);
    if (!infile.good()) {
        cout << "ERROR: File not found: \"" << filename  << "\"" << endl;
        throw ios_base::failure("Invalid file name");
    }
    for(int i = 0; i < size; i++) {
        infile >> arr.at(i);
    }
    infile.close();
}

template<typename T, size_t size>
void displayArray(const array<T, size>& arr) {
    for(int i = 0; i < size; i++) {
        cout << arr.at(i) << " ";
    }
    cout << endl;

}