#include "Restaurant.h"
#include "UI.h"
#include <iostream>
using namespace std;

int main() {
    UI ui;
    int mode = ui.readMode();
    string inFile = ui.readInputFileName();

    Restaurant r;
    if (!r.loadInputFile(inFile)) {
        cout << "Failed to open input file\n";
        return 1;
    }
    r.setMode(mode);
    r.runSimulation();

    cin.get();
    return 0;
}
