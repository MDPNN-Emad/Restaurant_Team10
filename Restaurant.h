#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
#include "Order.h"
#include "Chef.h"
#include "Scooter.h"
#include "Table.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Action.h"
#include "ArrayStack.h"
#include "CookOrds.h"
#include "PendOVC.h"
#include "RdyOV.h"
#include "TableQueue.h"

class Restaurant {
private:
    // pending queues by type
    LinkedQueue<Order*> pendingODG;
    LinkedQueue<Order*> pendingODN;
    LinkedQueue<Order*> pendingOT;
    LinkedQueue<Order*> pendingOVN;
    PendOVC             pendingOVC;
    priQueue<Order*>    pendingOVG;

    // orders in progress / ready
    CookOrds            cookingOrders;
    LinkedQueue<Order*> readyOD;
    LinkedQueue<Order*> readyOT;
    RdyOV               readyOV;

    ArrayStack<Order*>  finishedOrders;
    LinkedQueue<Order*> cancelledOrders;
    priQueue<Order*>    inServiceOrders;

    // chefs
    LinkedQueue<Chef*>  freeCN;
    LinkedQueue<Chef*>  freeCS;

    // resources
    priQueue<Scooter*>    freeScooters;
    priQueue<Scooter*>    backScooters;
    LinkedQueue<Scooter*> maintScooters;
    TableQueue            freeTables;
    TableQueue            busySharable;
    TableQueue            busyNoShare;

    // counters
    int finishedCount;
    int cancelledCount;
    int totalOrders;
    int currentTime;
    int TH;
    int mode;

    LinkedQueue<Action*> actionsList;

    void assignChefs();
    int  computeOVGPriority(Order* o);
    void checkCookingOrders();
    void assignTables();
    void checkTables();
    void checkTakeaway();
    void assignScooters();
    void checkScooters();

public:
    Restaurant();
    ~Restaurant();
    void printStatus();
    void simulate();
    int  getTH() const;
    void setTH(int t);
    int  getMode() const;
    void setMode(int m);
    bool loadInputFile(string filename);
    void printLoadDiagnostics();
    void addPendingOrder(Order* o);
    void cancelOrder(int id);
    void runSimulation();
    bool writeOutputFile(string filename);
};

#endif
