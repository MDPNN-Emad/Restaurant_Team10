#ifndef RESTAURANT_H
#define RESTAURANT_H

#include "Order.h"
#include "Chef.h"
#include "Scooter.h"
#include "Table.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "CookOrds.h"
#include "random_generator.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Restaurant {
private:
    // pending queues by type
    LinkedQueue<Order*> pendingODG;
    LinkedQueue<Order*> pendingODN;
    LinkedQueue<Order*> pendingOT;
    LinkedQueue<Order*> pendingOVN;
    LinkedQueue<Order*> pendingOVC;
    priQueue<Order*> pendingOVG;

    // orders in progress / ready
    CookOrds cookingOrders;
    LinkedQueue<Order*> readyOD; // dine-in
    LinkedQueue<Order*> readyOT; // takeaway
    LinkedQueue<Order*> readyOV; // delivery

    LinkedQueue<Order*> finishedOrders;
    LinkedQueue<Order*> cancelledOrders;

    // chefs
    LinkedQueue<Chef*> freeCN;
    LinkedQueue<Chef*> freeCS;
    LinkedQueue<Chef*> busyChefs;
    //resources
    LinkedQueue<Scooter*> freeScooters;
    LinkedQueue<Table*> freeTables;

    // order counters
    int finishedCount;
    int cancelledCount;
    int totalOrders;
    int currentTime;

public:
    Restaurant()
        : finishedCount(0), cancelledCount(0), totalOrders(500), currentTime(0)
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        // create chefs
        for (int i = 1; i <= 10; ++i) freeCN.enqueue(new Chef(i, "CN", 3));
        for (int i = 11; i <= 15; ++i) freeCS.enqueue(new Chef(i, "CS", 5));

        // create tables and scooters
        for (int i = 1; i <= 10; ++i) freeTables.enqueue(new Table(i, rand() % 5 + 2));
        for (int i = 1; i <= 8; ++i) freeScooters.enqueue(new Scooter(i, 50, 8, 5));

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
    }

    ~Restaurant() {
        Order* o; int p;

        // pending orders
        while (pendingODG.dequeue(o))    delete o;
        while (pendingODN.dequeue(o))    delete o;
        while (pendingOT.dequeue(o))     delete o;
        while (pendingOVN.dequeue(o))    delete o;
        while (pendingOVC.dequeue(o))    delete o;
        while (pendingOVG.dequeue(o, p)) delete o;

        // in-flight and completed orders
        while (cookingOrders.dequeue(o, p)) delete o;
        while (readyOD.dequeue(o))       delete o;
        while (readyOT.dequeue(o))       delete o;
        while (readyOV.dequeue(o))       delete o;
        while (finishedOrders.dequeue(o))  delete o;
        while (cancelledOrders.dequeue(o)) delete o;

        // chefs
        Chef* c;
        while (freeCN.dequeue(c))    delete c;
        while (freeCS.dequeue(c))    delete c;
        while (busyChefs.dequeue(c)) delete c;

        // resources
        Scooter* s;
        while (freeScooters.dequeue(s)) delete s;

        Table* t;
        while (freeTables.dequeue(t)) delete t;
    }

    void printStatus() {
        cout << "\n========== TIME " << currentTime << " ==========" << '\n';
        cout << "Finished: " << finishedCount << " | Cancelled: " << cancelledCount
             << " | Left: " << (totalOrders - finishedCount - cancelledCount) << '\n' << '\n';

        // pending summary
        cout << "Pending Orders:\n";
        cout << "  ODG: " << getCount(pendingODG)
             << " | ODN: " << getCount(pendingODN)
             << " | OT: "  << getCount(pendingOT) << '\n';
        cout << "  OVN: " << getCount(pendingOVN)
             << " | OVC: " << getCount(pendingOVC)
             << " | OVG: " << getPriCount(pendingOVG) << '\n';

        // chefs and work
        cout << "\nChefs: Free CN=" << getChefCount(freeCN)
             << " | Free CS=" << getChefCount(freeCS)
             << " | Busy=" << getChefCount(busyChefs) << '\n';

        cout << "Cooking Orders: " << cookingOrders.getCount() << '\n';
        cout << "Ready: OD=" << getCount(readyOD)
             << " | OT=" << getCount(readyOT)
             << " | OV=" << getCount(readyOV) << '\n';

        cout << "Free Tables: " << getTableCount(freeTables)
             << " | Free Scooters: " << getScooterCount(freeScooters) << '\n';

        cout << "\n--- Sample IDs ---\n";
        printFirstFew(pendingODG, "ODG pending");
        printFirstFewPri(cookingOrders, "Cooking");
        printFirstFew(readyOT, "Ready OT");
        printFirstFew(finishedOrders, "Finished");
    }

    void simulate() {
        while ((finishedCount + cancelledCount) < totalOrders && currentTime < 200) {
            printStatus();

			// chefs assingment 
            Chef* chefPtr;
            LinkedQueue<Chef*> tempChefs;

            while (busyChefs.dequeue(chefPtr)) {
                chefPtr->working();

                if (chefPtr->is_finished()) {
                    Order* done = chefPtr->get_current_order();
                    string t = done->get_type();

                    removeFromCooking(done->get_id());

                    if (t == "OT") readyOT.enqueue(done);
                    else if (t == "ODG" || t == "ODN") readyOD.enqueue(done);
                    else readyOV.enqueue(done);

                    if (chefPtr->get_specialization() == "CS") freeCS.enqueue(chefPtr);
                    else freeCN.enqueue(chefPtr);
                } else {
                    tempChefs.enqueue(chefPtr);
                }
            }
            while (tempChefs.dequeue(chefPtr)) busyChefs.enqueue(chefPtr);

            // 30 order
            for (int i = 0; i < 30; ++i) {
                Order* order = nullptr;
                int choice = rand() % 6;

                if (choice == 0) pendingODG.dequeue(order);
                else if (choice == 1) pendingODN.dequeue(order);
                else if (choice == 2) pendingOT.dequeue(order);
                else if (choice == 3) pendingOVN.dequeue(order);
                else if (choice == 4) pendingOVC.dequeue(order);
                else {
                    int priority;
                    pendingOVG.dequeue(order, priority);
                }

                if (!order) continue;

                Chef* assigned = nullptr;
                string t = order->get_type();

                if (t == "ODG" || t == "OVG") {
                    if (freeCS.dequeue(assigned)) {
                        assigned->assign_order(order);
                        busyChefs.enqueue(assigned);
                        cookingOrders.enqueue(order, -order->get_size());
                    } else {
                        if (t == "ODG") pendingODG.enqueue(order);
                        else pendingOVG.enqueue(order, rand() % 10);
                    }
                } else {
                    if (freeCN.dequeue(assigned) || freeCS.dequeue(assigned)) {
                        assigned->assign_order(order);
                        busyChefs.enqueue(assigned);
                        cookingOrders.enqueue(order, -order->get_size());
                    } else {
                        if (t == "ODN") pendingODN.enqueue(order);
                        else if (t == "OT") pendingOT.enqueue(order);
                        else if (t == "OVC") pendingOVC.enqueue(order);
                        else pendingOVN.enqueue(order);
                    }
                }
            }

            // set finish to ready orders
            Order* ready = nullptr;

            // takeaway should finish immediately
            while (readyOT.dequeue(ready)) {
                finishedOrders.enqueue(ready);
                ++finishedCount;
            }

			// dine-in needs table assignment
            Table* table = nullptr;
            while (readyOD.dequeue(ready)) {
                if (freeTables.dequeue(table)) {
                    finishedOrders.enqueue(ready);
                    ++finishedCount;
                    freeTables.enqueue(table);
                } else {
                    readyOD.enqueue(ready);
                    break;
                }
            }

            // delivery orders /  scooter
            Scooter* scooter = nullptr;
            while (readyOV.dequeue(ready)) {
                if (freeScooters.dequeue(scooter)) {
                    finishedOrders.enqueue(ready);
                    ++finishedCount;
                    freeScooters.enqueue(scooter);
                } else {
                    readyOV.enqueue(ready);
                    break;
                }
            }

            // random cancellations (10%)
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
    }

    //counter
    int getCount(LinkedQueue<Order*>& q) {
        int count = 0;
        Order* item = nullptr;
        LinkedQueue<Order*> temp;
        while (q.dequeue(item)) { ++count; temp.enqueue(item); }
        while (temp.dequeue(item)) q.enqueue(item);
        return count;
    }

    int getPriCount(priQueue<Order*>& q) {
        int count = 0;
        Order* item = nullptr;
        int p;
        priQueue<Order*> temp;
        while (q.dequeue(item, p)) { ++count; temp.enqueue(item, p); }
        while (temp.dequeue(item, p)) q.enqueue(item, p);
        return count;
    }

    int getChefCount(LinkedQueue<Chef*>& q) {
        int count = 0;
        Chef* item = nullptr;
        LinkedQueue<Chef*> temp;
        while (q.dequeue(item)) { ++count; temp.enqueue(item); }
        while (temp.dequeue(item)) q.enqueue(item);
        return count;
    }

    int getTableCount(LinkedQueue<Table*>& q) {
        int count = 0;
        Table* item = nullptr;
        LinkedQueue<Table*> temp;
        while (q.dequeue(item)) { ++count; temp.enqueue(item); }
        while (temp.dequeue(item)) q.enqueue(item);
        return count;
    }

    int getScooterCount(LinkedQueue<Scooter*>& q) {
        int count = 0;
        Scooter* item = nullptr;
        LinkedQueue<Scooter*> temp;
        while (q.dequeue(item)) { ++count; temp.enqueue(item); }
        while (temp.dequeue(item)) q.enqueue(item);
        return count;
    }

    void removeFromCooking(int orderId) {
        priQueue<Order*> temp;
        Order* o; int p;
        while (cookingOrders.dequeue(o, p)) {
            if (o->get_id() != orderId)
                temp.enqueue(o, p);
        }
        while (temp.dequeue(o, p))
            cookingOrders.enqueue(o, p);
    }

    void printFirstFewPri(priQueue<Order*>& q, string name) {
        cout << name << ": ";
        priQueue<Order*> shown;
        priQueue<Order*> rest;
        Order* o = nullptr;
        int p, count = 0;
        while (count < 5 && q.dequeue(o, p)) {
            cout << o->get_id() << " ";
            shown.enqueue(o, p);
            ++count;
        }
        while (q.dequeue(o, p)) rest.enqueue(o, p);
        while (shown.dequeue(o, p)) q.enqueue(o, p);
        while (rest.dequeue(o, p))  q.enqueue(o, p);
        if (count == 0) cout << "(empty)";
        cout << endl;
    }

    void printFirstFew(LinkedQueue<Order*>& q, string name) {
        cout << name << ": ";
        LinkedQueue<Order*> temp;
        Order* o = nullptr;
        int shown = 0;
        while (q.dequeue(o) && shown < 5) {
            cout << o->get_id() << " ";
            temp.enqueue(o);
            ++shown;
        }
        while (temp.dequeue(o)) q.enqueue(o);
        // restore remaining items
        while (q.dequeue(o)) temp.enqueue(o);
        while (temp.dequeue(o)) q.enqueue(o);

        if (shown == 0) cout << "(empty)";
        cout << endl;
    }
};

#endif