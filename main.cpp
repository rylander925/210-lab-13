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

const int IGNORE_CHARS = 100; //Characters ignored by istream.ignore()

struct Grocery {
    string name;
    double price;
    //For >, <, >=, <=, compare groceries by price for sorting, min, and max
    friend bool operator<(const Grocery& g1, const Grocery& g2);

    //For ==, compare groceries by name for find()
    friend bool operator==(const Grocery& g1, const Grocery& g2);
};
//Add other comparisons in case min/max requires
bool operator<=(const Grocery& g1, const Grocery& g2);
bool operator>(const Grocery& g1, const Grocery& g2);
bool operator>=(const Grocery& g1, const Grocery& g2);

/**
 * Fills given array with grocery names and prices from the provided file
 * @param arr Array to fill with grocery objects
 * @param filename Name of file to retrieve data from
 * @note File lines must be formatted as Names\nPrice without whitespace after name
 */
template<size_t size>
void fillGroceries(array<Grocery, size>& arr, string filename);

/**
 * Displays given array of groceries
 * @todo Add better formatting
 * @param arr Array of groceries to display
 */
template<size_t size>
void displayGroceries(const array<Grocery, size>& arr);

void coutLine(int size = 50, char lineChar = '=');

/**
 * Accumulator function to add grocery prices via accumulate() function
 * @param acc Accumulator variable to store summed prices
 * @param g Reference to a grocery variable to take the price from
 * @return Sum of accumulator variable and price of the grocery g
 */
double accPrices(double acc, const Grocery& g);

int main() {
    const string FILENAME = "data.txt";
    const int SIZE = 5;
    const int DISPLAY_AMOUNT = 5; //For displaying top 5/least 5 prices 

    double totalPrice;
    array<Grocery, SIZE> groceries;

    fillGroceries(groceries, FILENAME);
    totalPrice = accumulate(groceries.begin(), groceries.end(), 0.0, accPrices);
    cout << "Displaying data: " << endl;
    displayGroceries(groceries); //side effect: sets precision formatting to money format

    //Display total, average, max & min prices
    coutLine();
    cout << "Items: " << groceries.size();
    cout << "Total price: $" << totalPrice << endl;
    cout << "Average price: $" << totalPrice / groceries.size() << endl;
    cout << "Price range: $" 
         << max_element(groceries.begin(), groceries.end())->price 
         << "-"
         << min_element(groceries.begin(), groceries.end())->price 
         << endl;

    //Use sort w/ forward & reverse iterators to display cheapest/most expensive items
    coutLine();
    cout << "Top 5 cheapest items:" << endl;
    sort(groceries.begin(), groceries.end());
    displayGroceries(groceries);
    coutLine();
    cout << "Top 5 most expensive items:" << endl;
    sort(groceries.rbegin(), groceries.rend());
    displayGroceries(groceries);

    //Prompt user for an item name to find prices until a valid item is found
    coutLine();
    Grocery searchDummy; //operator== for find() requires another grocery object
    array<Grocery, groceries.size()>::iterator foundGrocery;
    do {
        cout << "Enter the name of an item to search for: " << endl;
        //operator== compares names of groceries
        //get name from input and populate it to the dummy Grocery variable
        getline(cin, searchDummy.name);
        foundGrocery = find(groceries.begin(), groceries.end(), searchDummy);

        //output list of grocery names if given an invalid name
        if (foundGrocery == groceries.end()) {
            cout << " > Invalid grocery name. Here is a list of valid names:" << endl << " > ";
            for(Grocery g : groceries) { 
                cout << g.name << " " ;
            }
            cout << endl << endl;
        }
    } while(foundGrocery == groceries.end());
    //output price of found item
    cout << foundGrocery->name << ": " << foundGrocery->price << endl;
}

double accPrices(double acc, const Grocery&g) {
    return acc + g.price;
}

void coutLine(int size, char lineChar) {
    char prevFillChar = cout.fill();
    cout << setw(size) << setfill(lineChar) << "" << setfill(prevFillChar) << endl;
}

template<size_t size>
void fillGroceries(array<Grocery, size>& arr, string filename) {
    //Open file
    ifstream infile;
    infile.open(filename);
    //Verify file opened properly
    if (!infile.good()) {
        cout << "ERROR: File not found: \"" << filename  << "\"" << endl;
        throw ios_base::failure("Invalid file name");
    }
    //Fill groceries; assumes formatted as:
    //Blueberries
    //12.15
    for(Grocery& g : arr) {
        getline(infile, g.name);
        infile >> g.price;
        infile.ignore(IGNORE_CHARS, '\n');
    }
    infile.close();
}

template<size_t size>
void displayGroceries(const array<Grocery, size>& arr) {
    //Set decimal precision for money formatting
    cout << fixed << setprecision(2);
    //Outputs elements followed by two spaces
    for(Grocery g : arr) {
        cout << g.name << ": $" << g.price << "  ";
    }
    cout << endl;
}

//Compare by name for compatability with find()
bool operator==(const Grocery& g1, const Grocery& g2) {
    return g1.name == g2.name;
}

//Define comparison operators to compare by prices
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