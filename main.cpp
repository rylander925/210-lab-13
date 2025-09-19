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
    //(the "==" operator is not useful for doubles anyways)
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
 * If specified, displays only the first given number of elements
 * @param arr Array of groceries to display
 * @param nums If specified, amount of items to display
 * @param showPrices If true, will show prices with item names
 * @param columns Entries per line when displaying
 * @param columnWidth Spacing per column when displaying
 */
template<size_t size>
void displayGroceries(const array<Grocery, size>& arr, bool showPrices = true, int nums = size, int columns = 5, int columnWidth = 25);

/**
 * Output a line of characters
 * @param size Length of the line
 * @param lineChar Character composing line
 */
void coutLine(int size = 150, char lineChar = '=');

/**
 * Accumulator function to add grocery prices via accumulate() function
 * @param acc Accumulator variable to store summed prices
 * @param g Reference to a grocery variable to take the price from
 * @return Sum of accumulator variable and price of the grocery g
 */
double accPrices(double acc, const Grocery& g);

/**
 * Outputs a lowercase copy of a string. Does not modify the given string.
 * @param str String to return a lowercase copy of
 * @return Lowercase copy of str
 */
string LowerString(const string str);

int main() {
    const string FILENAME = "data.txt";
    const int SIZE = 30;
    const int DISPLAY_AMOUNT = 5; //For displaying top 5/least 5 prices 
    double totalPrice;
    array<Grocery, SIZE> groceries;

    //Used with find() to prompt user for an item
    Grocery searchDummy; //operator== for find() requires another grocery object
    array<Grocery, groceries.size()>::iterator foundGrocery;

    //Populate & display array
    fillGroceries(groceries, FILENAME);
    cout << "Displaying data: " << endl;
    displayGroceries(groceries);

    //store price in a var to use to calculate average & show total price
    totalPrice = accumulate(groceries.begin(), groceries.end(), 0.0, accPrices);

    //Display total, average, max & min prices
    coutLine();
    cout << fixed << setprecision(2);
    cout << "Items: " << groceries.size() << endl;
    cout << "Total price: $" << totalPrice << endl;
    cout << "Average price: $" << totalPrice / groceries.size() << endl;
    cout << "Price range: $" 
         << max_element(groceries.begin(), groceries.end())->price 
         << "-"
         << min_element(groceries.begin(), groceries.end())->price 
         << endl;

    //Use sort w/ forward & reverse iterators to display cheapest/most expensive items
    coutLine();
    cout << "Top " << DISPLAY_AMOUNT << " cheapest items:" << endl;
    sort(groceries.begin(), groceries.end());
    displayGroceries(groceries, true, DISPLAY_AMOUNT);
    coutLine();
    cout << "Top " << DISPLAY_AMOUNT << " most expensive items:" << endl;
    sort(groceries.rbegin(), groceries.rend());
    displayGroceries(groceries, true, DISPLAY_AMOUNT);

    //Prompt user for item name & outputs its price. Continues until the user quits
    coutLine();
    do {
        //Prompt user for an item name to find prices until a valid item is found
        do {
            cout << "Enter the name of an item to search for or type \"quit\" to quit: " << endl;
            //operator== compares names of groceries
            //get name from input and populate it to the dummy Grocery variable
            getline(cin, searchDummy.name);
            foundGrocery = find(groceries.begin(), groceries.end(), searchDummy);
    
            //output list of grocery names if given an invalid name
            if (foundGrocery == groceries.end() && searchDummy.name != "quit") {
                cout << " > Invalid grocery name. Here is a list of valid names:" << endl;
                displayGroceries(groceries, false);
            }
        } while(foundGrocery == groceries.end() && searchDummy.name != "quit");
        //output price of found item
        if (searchDummy.name != "quit") {
            cout << foundGrocery->name << ": $" << foundGrocery->price << endl;
        }
    } while (searchDummy.name != "quit");
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
void displayGroceries(const array<Grocery, size>& arr, bool showPrices, int nums, int columns, int columnWidth) {
    stringstream grocery;
    //Take the min of size & nums to prevent out of bounds errors
    nums = (nums > size) ? size : nums;
    //Outputs elements
    for(int i = 0; i < nums; i++) {
        //choose what to output: w/ prices or w/o prices
        if (showPrices) {
            //To properly use setw(), combine data into a string first using a stringstream
            grocery << fixed << setprecision(2) << "$" << arr.at(i).price << " - " << arr.at(i).name;
        } else {
            grocery << arr.at(i).name;
        }

        cout << left << setw(columnWidth) << grocery.str();

        //add an endl after every "columns" elements
        if ((i % (columns) == (columns - 1)) && (i != nums - 1)) {
            cout << endl;
        }

        //clear stringstream for next set of data
        grocery.str("");
    }
    cout << endl;
}

string LowerString(const string str) {
    //creates a copy of the string and makes it lowercase
    string newStr = str;
    for (char& c : newStr) {
        c = tolower(c);
    }
    return newStr;
}

void coutLine(int size, char lineChar) {
    char prevFillChar = cout.fill();
    cout << setw(size) << setfill(lineChar) << "" << setfill(prevFillChar) << endl;
}

//to be passed to accumulate function
double accPrices(double acc, const Grocery&g) { return acc + g.price; }

//Compare by name for compatability with find()
bool operator==(const Grocery& g1, const Grocery& g2) {
    return LowerString(g1.name) == LowerString(g2.name);
}

//Define comparison operators to compare by prices
bool operator<(const Grocery& g1, const Grocery& g2) { return g1.price < g2.price; }
bool operator>(const Grocery& g1, const Grocery& g2) { return g2.price < g1.price; }
bool operator<=(const Grocery& g1, const Grocery& g2) { return !(g2.price < g1.price); }
bool operator>=(const Grocery& g1, const Grocery& g2) { return !(g1.price < g2.price); }