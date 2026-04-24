#ifndef SCOOTER_H
#define SCOOTER_H

#include <iostream>
#include <ostream>
using namespace std;

class Scooter {
private:
    int id;
    int speed;                  
    int Main_Ords;   // number of orders before maintenance
    int Main_Dur;        // maintenance duration
    //i will use them in phase 2
    int totalDistanceTraveled;  // cumulative distance (I will use it in priority)
    bool inMaintenance;
    int maintenanceRemaining;   // timesteps left in maintenance (0 if not in maintenance)
    int ordersServed;           // count since last maintenance
    bool isAvailable;       

public:
    // Constructor
    Scooter(int id, int speed, int Main_Dur, int Main_Ords)
        : id(id), speed(speed), Main_Dur(Main_Dur), Main_Ords(Main_Ords), totalDistanceTraveled(0),
        inMaintenance(false), maintenanceRemaining(0), ordersServed(0), isAvailable(true) { } 

    // Getters
    int getID() const { return id; }
    int getSpeed() const { return speed; }
    int getMainDuration() const { return Main_Dur; }
    int getMainOrders() const { return Main_Ords; }
    int getTotalDistance() const { return totalDistanceTraveled; }
    bool getInMaintenance() const { return inMaintenance; }
    int getMaintenanceRemaining() const { return maintenanceRemaining; }
    int getOrdersServed() const { return ordersServed; }
    bool getIsAvailable() const { return isAvailable && !inMaintenance; }

    // Setters / modifiers
    void addDistance(int distance) {
        totalDistanceTraveled += distance;
    }

    void startMaintenance(int duration) {
        inMaintenance = true;
        maintenanceRemaining = duration;
        isAvailable = false;
        ordersServed = 0;   // reset counter after maintenance
    }

    void updateMaintenance() {
        if (inMaintenance) {
            maintenanceRemaining--;
            if (maintenanceRemaining <= 0) {
                inMaintenance = false;
                isAvailable = true;
                maintenanceRemaining = 0;
            }
        }
    }

    void setAvailable(bool available) {
        isAvailable = available;
    }

    void incrementOrdersServed() {
        ordersServed++;
    }

    bool needsMaintenance() const {
        if (!inMaintenance && ordersServed >= Main_Ords)
            return true;
        return false;

    }

    friend ostream& operator<<(ostream& os, const Scooter* scooter) {
        if (scooter) {
            os << "Scooter(ID= " << scooter->id
                << ", Speed= " << scooter->speed
                << ", TotalDist= " << scooter->totalDistanceTraveled
                << (scooter->isAvailable ? ", Avail= Y" : ", Avail= N")
                << (scooter->inMaintenance ? ", Maint= Y" : ", Maint= N") << ")";
        }
        else {
            os << "Scooter(null)";
        }
        return os;
    }
};

#endif // SCOOTER_H