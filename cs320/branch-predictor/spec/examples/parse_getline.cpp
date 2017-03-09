/* Example
 * Parsing Branch Traces
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;

int main(int argc, char *argv[]) {

  // Temporary variables
  unsigned long long addr;
  string behavior, line;

  // Open file for reading
  ifstream infile("test_input.txt");

  // The following loop will read a line at a time
  while(getline(infile, line)) {
    // Now we have to parse the line into it's two pieces
    stringstream s(line);
    s >> std::hex >> addr >> behavior;
    // Now we can output it
    cout << addr;
    if(behavior == "T") {
      cout << " -> taken" << endl;
    }else {
      cout << " -> not taken" << endl;
    }
  }

  return 0;
}
