#include "Restaurant.h"
#include "RequestAction.h"
#include "CancelAction.h"
#include "random_generator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// Replaced by Restaurant::loadInputFile().
Restaurant::Restaurant()
    : finishedCount(0), cancelledCount(0), totalOrders(0), currentTime(0), TH(0)
{
    /*
    srand(static_cast<unsigned int>(time(nullptr)));

    // create chefs
    for (int i = 1; i <= 10; ++i) freeCN.enqueue(new Chef(i, "CN", 3));
    for (int i = 11; i <= 15; ++i) freeCS.enqueue(new Chef(i, "CS", 5));

    // create tables and scooters
    for (int i = 1; i <= 10; ++i) { int cap = rand() % 5 + 2; freeTables.enqueue(new Table(i, cap), cap); }
    for (int i = 1; i <= 8; ++i) freeScooters.enqueue(new Scooter(i, 50, 8, 5), 0);

    // generate orders
    RandomGenerator rng;
    for (int i = 1; i <= totalOrders; ++i) {
        Order* order = rng.generate_random_order(i, rand() % 100);
        string type = order->get_type();

        if (type == "ODG") pendingODG.enqueue(order);
        else if (type == "ODN") pendingODN.enqueue(order);
        else if (type == "OT")  pendingOT.enqueue(order);
        else if (type == "OVN") pendingOVN.enqueue(order);
        else if (type == "OVC") pendingOVC.enqueue(order);
        else if (type == "OVG") pendingOVG.enqueue(order, rand() % 10);
    }
    */
}

Restaurant::~Restaurant() {
    Order* o; int p;
    Action* a;

    // pending orders
    while (pendingODG.dequeue(o))    delete o;
    while (pendingODN.dequeue(o))    delete o;
    while (pendingOT.dequeue(o))     delete o;
    while (pendingOVN.dequeue(o))    delete o;
    while (pendingOVC.dequeue(o))    delete o;
    while (pendingOVG.dequeue(o, p)) delete o;

    // in-flight and completed orders
    while (cookingOrders.dequeue(o, p)) delete o;
    while (readyOD.dequeue(o))          delete o;
    while (readyOT.dequeue(o))          delete o;
    while (readyOV.dequeue(o))          delete o;
    while (finishedOrders.pop(o))         delete o;
    while (inServiceOrders.dequeue(o, p)) delete o;
    while (cancelledOrders.dequeue(o))    delete o;

    // chefs 
    Chef* c;
    while (freeCN.dequeue(c)) delete c;
    while (freeCS.dequeue(c)) delete c;

    // resources
    Scooter* s;
    while (freeScooters.dequeue(s, p)) delete s;
    while (backScooters.dequeue(s, p)) delete s;
    while (maintScooters.dequeue(s))   delete s;

    Table* t;
    while (freeTables.dequeue(t, p))   delete t;
    while (busySharable.dequeue(t, p)) delete t;
    while (busyNoShare.dequeue(t, p))  delete t;

    while (actionsList.dequeue(a)) delete a;
}

// UI::printStatus() handles this.
void Restaurant::printStatus()
{
    /*
    cout << "\n========== TIME " << currentTime << " ==========" << '\n';
    cout << "Finished: " << finishedCount << " | Cancelled: " << cancelledCount
         << " | Left: " << (totalOrders - finishedCount - cancelledCount) << '\n' << '\n';

    cout << "Pending Orders:\n";
    cout << "  ODG: " << pendingODG.getCount()
         << " | ODN: " << pendingODN.getCount()
         << " | OT: "  << pendingOT.getCount()  << '\n';
    cout << "  OVN: " << pendingOVN.getCount()
         << " | OVC: " << pendingOVC.getCount()
         << " | OVG: " << pendingOVG.getCount() << '\n';

    cout << "\nChefs: Free CN=" << freeCN.getCount()
         << " | Free CS=" << freeCS.getCount() << '\n';

    cout << "Cooking Orders: " << cookingOrders.getCount() << '\n';
    cout << "Ready: OD=" << readyOD.getCount()
         << " | OT=" << readyOT.getCount()
         << " | OV=" << readyOV.getCount() << '\n';

    cout << "Free Tables: " << freeTables.getCount()
         << " | Free Scooters: " << freeScooters.getCount() << '\n';
    */
}

//Replaced by Restaurant::runSimulation().
void Restaurant::simulate()
{
    /*
    while ((finishedCount + cancelledCount) < totalOrders && currentTime < 200) {
        printStatus();

        // advance cooking — iterate cookingOrders; get chef via order->get_assigned_chef()
        priQueue<Order*> tempCooking;
        Order* cookingOrd; int cookPri;
        while (cookingOrders.dequeue(cookingOrd, cookPri)) {
            Chef* chefPtr = cookingOrd->get_assigned_chef();
            chefPtr->working();
            if (chefPtr->is_finished()) {
                string t = cookingOrd->get_type();
                if (t == "OT") readyOT.enqueue(cookingOrd);
                else if (t == "ODG" || t == "ODN") readyOD.enqueue(cookingOrd);
                else readyOV.enqueue(cookingOrd);
                chefPtr->release_order();
                if (chefPtr->get_specialization() == "CS") freeCS.enqueue(chefPtr);
                else freeCN.enqueue(chefPtr);
            } else {
                tempCooking.enqueue(cookingOrd, cookPri);
            }
        }
        while (tempCooking.dequeue(cookingOrd, cookPri)) cookingOrders.enqueue(cookingOrd, cookPri);

        // dispatch up to 30 pending orders
        for (int i = 0; i < 30; ++i) {
            Order* order = nullptr;
            int choice = rand() % 6;
            if      (choice == 0) pendingODG.dequeue(order);
            else if (choice == 1) pendingODN.dequeue(order);
            else if (choice == 2) pendingOT.dequeue(order);
            else if (choice == 3) pendingOVN.dequeue(order);
            else if (choice == 4) pendingOVC.dequeue(order);
            else { int priority; pendingOVG.dequeue(order, priority); }

            if (!order) continue;

            Chef* assigned = nullptr;
            string t = order->get_type();
            if (t == "ODG" || t == "OVG") {
                if (freeCS.dequeue(assigned)) {
                    assigned->assign_order(order);  // sets order->assigned_chef = assigned
                    cookingOrders.enqueue(order, -order->get_size());
                } else {
                    if (t == "ODG") pendingODG.enqueue(order);
                    else pendingOVG.enqueue(order, rand() % 10);
                }
            } else {
                if (freeCN.dequeue(assigned) || freeCS.dequeue(assigned)) {
                    assigned->assign_order(order);  // sets order->assigned_chef = assigned
                    cookingOrders.enqueue(order, -order->get_size());
                } else {
                    if (t == "ODN") pendingODN.enqueue(order);
                    else if (t == "OT") pendingOT.enqueue(order);
                    else if (t == "OVC") pendingOVC.enqueue(order);
                    else pendingOVN.enqueue(order);
                }
            }
        }

        Order* ready = nullptr;

        while (readyOT.dequeue(ready)) { finishedOrders.push(ready); ++finishedCount; }

        Table* table = nullptr; int tp = 0;
        while (readyOD.dequeue(ready)) {
            if (freeTables.dequeue(table, tp)) {
                finishedOrders.push(ready); ++finishedCount;
                freeTables.enqueue(table, tp);
            } else { readyOD.enqueue(ready); break; }
        }

        Scooter* scooter = nullptr; int sp = 0;
        while (readyOV.dequeue(ready)) {
            if (freeScooters.dequeue(scooter, sp)) {
                finishedOrders.push(ready); ++finishedCount;
                freeScooters.enqueue(scooter, sp);
            } else { readyOV.enqueue(ready); break; }
        }

        if (rand() % 100 < 10) {
            Order* cancelled = nullptr;
            if (pendingOVC.dequeue(cancelled)) {
                cancelledOrders.enqueue(cancelled);
                ++cancelledCount;
                cout << "  Cancelled order " << cancelled->get_id() << " from pending OVC\n";
            }
        }

        ++currentTime;
        cout << "\nPress Enter for next timestep...";
        cin.get();
    }

    cout << "\n========== SIMULATION DONE ==========" << '\n';
    cout << "Finished at time: " << currentTime << endl;
    cout << "Finished orders: " << finishedCount << endl;
    cout << "Cancelled orders: " << cancelledCount << endl;
    */
}

int Restaurant::getTH() const { return TH; }
void Restaurant::setTH(int t) { TH = t; }

bool Restaurant::loadInputFile(string filename)
{
    ifstream fin(filename);
    if (!fin.is_open()) return false;

    int numCN;int numCS;int cnSpeed;int csSpeed;
    fin >> numCN >> numCS >> cnSpeed >> csSpeed;

    int scooterCount;int scooterSpeed;
    fin >> scooterCount >> scooterSpeed;

    int mainOrds;int mainDur;
    fin >> mainOrds >> mainDur;

    int tableCount;
    fin >> tableCount;

    int id = 1;
    for (int i = 0; i < numCN; i++) {
        freeCN.enqueue(new Chef(id, "CN", cnSpeed));
        id++;
    }
        
    for (int i = 0; i < numCS; i++) {
        freeCS.enqueue(new Chef(id, "CS", csSpeed));
        id++;
    }
        

    for (int i = 1; i <= scooterCount; i++)
		freeScooters.enqueue(new Scooter(i, scooterSpeed, mainDur, mainOrds), 0);//0=distance travelled priority

    int tblId = 1;
    int remaining = 0;
    while (remaining < tableCount) {
        int count;int cap;
        fin >> count >> cap;
        for (int i = 0; i < count; i++, tblId++)
			freeTables.enqueue(new Table(tblId, cap), -cap); // -cap; to prioritize smaller tables // priQueue has larger priority value at head
        remaining += count;
    }

    int th;int M;
    fin >> th >> M;
    setTH(th);

	for (int i = 0; i < M; i++) { //read and classify according to type(no. of inputs), and enqueue in actionsList
        string token;
        fin >> token;
        if (token == "Q") {
            string typ;
            fin >> typ;
            int tq;int oid;int sz; double pr;
            fin >> tq >> oid >> sz >> pr;
            if (typ == "ODG" || typ == "ODN") {
                int seats, dur;
                char shareChar;
                fin >> seats >> dur >> shareChar;
                actionsList.enqueue(new RequestAction(tq, typ, oid, sz, pr, seats, dur, shareChar == 'Y'));
            } else if (typ == "OVC" || typ == "OVG" || typ == "OVN") {
                int dist;
                fin >> dist;
                actionsList.enqueue(new RequestAction(tq, typ, oid, sz, pr, dist));
            } else { // OT
                actionsList.enqueue(new RequestAction(tq, typ, oid, sz, pr));
            }
            totalOrders++;
        } else { // X
            int tCancel;int oid;
            fin >> tCancel >> oid;
            actionsList.enqueue(new CancelAction(tCancel, oid));
        }
    }

    fin.close();
    return true;
}

void Restaurant::printLoadDiagnostics()
{
    cout << "Free CN: "          << freeCN.getCount()        << "\n";
    cout << "Free CS: "          << freeCS.getCount()        << "\n";
    cout << "Free Scooters: "    << freeScooters.getCount()  << "\n";
    cout << "Free Tables: "      << freeTables.getCount()    << "\n";
    cout << "TH: "               << TH                       << "\n";
    cout << "Actions in queue: " << actionsList.getCount()   << "\n";
    cout << "Total Orders: "     << totalOrders              << "\n";
}
