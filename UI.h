#ifndef UI_H
#define UI_H

#include "Order.h"
#include "Chef.h"
#include "Table.h"
#include "Scooter.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include <iostream>
#include <string>
using namespace std;

class UI {
private:
    void printFirstFew(LinkedQueue<Order*>& q, const string& name) {
        cout << name << ": ";
        LinkedQueue<Order*> shown;
        LinkedQueue<Order*> rest;
        Order* o = nullptr;
        int count = 0;

        while (count < 5 && q.dequeue(o)) {
            cout << o->get_id() << " ";
            shown.enqueue(o);
            ++count;
        }
        while (q.dequeue(o)) rest.enqueue(o);

        // restore original order: shown items first, then the rest
        while (shown.dequeue(o)) q.enqueue(o);
        while (rest.dequeue(o))  q.enqueue(o);

        if (count == 0) cout << "(empty)";
        cout << endl;
    }

public:
    void printStatus(
        int currentTime,
        int finishedCount,
        int cancelledCount,
        int totalOrders,
        LinkedQueue<Order*>&   pendingODG,
        LinkedQueue<Order*>&   pendingODN,
        LinkedQueue<Order*>&   pendingOT,
        LinkedQueue<Order*>&   pendingOVN,
        LinkedQueue<Order*>&   pendingOVC,
        priQueue<Order*>&      pendingOVG,
        LinkedQueue<Chef*>&    freeCN,
        LinkedQueue<Chef*>&    freeCS,
        LinkedQueue<Chef*>&    busyChefs,
        LinkedQueue<Order*>&   cookingOrders,
        LinkedQueue<Order*>&   readyOD,
        LinkedQueue<Order*>&   readyOT,
        LinkedQueue<Order*>&   readyOV,
        LinkedQueue<Table*>&   freeTables,
        LinkedQueue<Scooter*>& freeScooters,
        LinkedQueue<Order*>&   finishedOrders,
        LinkedQueue<Order*>&   cancelledOrders
    ) {
        cout << "\n========== TIME " << currentTime << " ==========" << '\n';
        cout << "Finished: " << finishedCount
             << " | Cancelled: " << cancelledCount
             << " | Left: " << (totalOrders - finishedCount - cancelledCount)
             << '\n' << '\n';

        cout << "Pending Orders:\n";
        cout << "  ODG: " << pendingODG.getCount()
             << " | ODN: " << pendingODN.getCount()
             << " | OT: "  << pendingOT.getCount()  << '\n';
        cout << "  OVN: " << pendingOVN.getCount()
             << " | OVC: " << pendingOVC.getCount()
             << " | OVG: " << pendingOVG.getCount() << '\n';

        cout << "\nChefs: Free CN=" << freeCN.getCount()
             << " | Free CS=" << freeCS.getCount()
             << " | Busy=" << busyChefs.getCount() << '\n';

        cout << "Cooking Orders: " << cookingOrders.getCount() << '\n';
        cout << "Ready: OD=" << readyOD.getCount()
             << " | OT=" << readyOT.getCount()
             << " | OV=" << readyOV.getCount() << '\n';

        cout << "Free Tables: " << freeTables.getCount()
             << " | Free Scooters: " << freeScooters.getCount() << '\n';

        cout << "\n--- Sample IDs ---\n";
        printFirstFew(pendingODG,     "ODG pending");
        printFirstFew(cookingOrders,  "Cooking");
        printFirstFew(readyOT,        "Ready OT");
        printFirstFew(finishedOrders, "Finished");
    }

    int readMode() {
        int mode = 0;
        while (mode != 1 && mode != 2) {
            cout << "Select mode:\n"
                 << "  1 - Interactive (step by step)\n"
                 << "  2 - Silent (run to end)\n"
                 << "Choice: ";
            cin >> mode;
            if (mode != 1 && mode != 2)
                cout << "Invalid. Enter 1 or 2.\n";
        }
        cin.ignore();
        return mode;
    }

    string readInputFileName() {
        string name;
        cout << "Enter input file name: ";
        cin >> name;
        cin.ignore();
        return name;
    }

    string readOutputFileName() {
        string name;
        cout << "Enter output file name: ";
        cin >> name;
        cin.ignore();
        return name;
    }

    void pressAnyKey() {
        cout << "\nPress Enter for next timestep...";
        cin.get();
    }
};

#endif
