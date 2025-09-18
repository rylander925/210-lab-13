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
#include <iomanip>

using namespace std;

struct Grocery {
    string name;
    double price;
    //Compare groceries by price
    friend bool operator<(const Grocery& g1, const Grocery& g2);
};
//Add other comparisons in case min/max requires
bool operator<=(const Grocery& g1, const Grocery& g2);
bool operator>(const Grocery& g1, const Grocery& g2);
bool operator>=(const Grocery& g1, const Grocery& g2);

template<typename T, size_t size>
void fillArray(array<T, size>& arr, string filename);

template<typename T, size_t size>
void displayArray(const array<T, size>& arr);

template<typename T, size_t size>
double averageArray(const array<T, size>& arr);

template<typename T, size_t size>
double medianArray(const array<T, size>& arr);

void coutLine(int size = 50, char lineChar = '=');

int main() {
    const string FILENAME = "data.txt";
    string input;
    array<int, 30> nums;
    fillArray(nums, FILENAME);

    cout << "Displaying data: " << endl;
    displayArray(nums);

    
}

void coutLine(int size, char lineChar) {
    char prevFillChar = cout.fill();
    cout << setw(size) << setfill(lineChar) << "" << setfill(prevFillChar) << endl;
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

template<typename T, size_t size>
double medianArray(const array<T, size>& arr) {
    array<T, size> copy = arr;
    sort(copy.begin(), copy.end());
    return (size % 2 == 0) ? ((copy.at(size / 2) + copy.at((size / 2) - 1)) / 2.0) : (copy.at(size / 2));
}

template<typename T, size_t size>
double averageArray(const array<T, size>& arr) {
    return accumulate(arr.begin(), arr.end(), 0) * 1.0 / size;
}

//Define comparison operators to compare by price
bool operator<(const Grocery& g1, const Grocery& g2) {
    return g1.price < g2.price;
}
bool operator>(const Grocery& g1, const Grocery& g2) {
    return g2.price < g1.price;
}
bool operator<=(const Grocery& g1, const Grocery& g2) {
    return !(g2.price < g1.price);
}
bool operator>=(const Grocery& g1, const Grocery& g2) {
    return !(g1.price < g2.price);
}