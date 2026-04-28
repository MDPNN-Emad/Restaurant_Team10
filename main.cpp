#include "Restaurant.h"
#include <iostream>
using namespace std;

// loader smoke test
int main() {
    Restaurant r;
    if (!r.loadInputFile("test_input.txt")) {
        cout << "Failed to open input file\n";
        return 1;
    }

    r.printLoadDiagnostics();

    cin.get();
    return 0;
}
