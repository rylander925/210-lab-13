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

const int SIZE = 30;
const string FILENAME = "data.txt";

void fillArray(array<double, SIZE>& arr);

void displayArray(const array<double, SIZE>& arr);

int main() {

}

void fillArray(array<double, SIZE>& arr) {
    ifstream infile;
    infile.open(FILENAME);
    if (!infile.good()) {
        cout << "ERROR: File not found: \"" << FILENAME  << "\"" << endl;
        throw ios_base::failure("Invalid file name");
    }
    for(int i = 0; i < SIZE; i++) {
        infile >> arr.at(i);
    }
    infile.close();
}