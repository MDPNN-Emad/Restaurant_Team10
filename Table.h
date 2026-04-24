#pragma once
#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <ostream>
using namespace std;

class Table {
private:
    int id;
    int capacity;               
    int occupiedSeats;          
    int occupiedUntil;          // Timestep when table becomes free (0 = free)

    int orderIDs[10];  // assuming Max 10 orders per table (used in phase 2)
    int orderCount;    // How many currently stored

public:
    // Constructor
    Table(int id, int capacity)
        : id(id), capacity(capacity), occupiedSeats(0), occupiedUntil(0), orderCount(0) {
    }

  
    int getID() const { return id; }
    int getCapacity() const { return capacity; }
    int getOccupiedSeats() const { return occupiedSeats; }
    int getFreeSeats() const { return capacity - occupiedSeats; }
    int getOccupiedUntil() const { return occupiedUntil; }
    int getOrderCount() const { return orderCount; }

    // Get order ID at specific index
    int getOrderID(int index) const {
        if (index >= 0 && index < orderCount) {
            return orderIDs[index];
        }
        return -1;  // Invalid index
    }

    // Check if table is available at given time (seats & is not occupied beyond current time)
    bool isAvailable(int currentTime) const {
        return (occupiedUntil <= currentTime) && (getFreeSeats() > 0);
    }

    // Check if table can fit a given number of seats
    bool canFit(int seats) const {
        return getFreeSeats() >= seats;
    }

    // Assign (dine-in or sharable case) >> // duration calculation
    bool assignOrder(int orderID, int seats, int duration, int currentTime) {
        
        if (occupiedUntil > currentTime) {
            return false; 
        }

        
        if (getFreeSeats() < seats) {
            return false;  
        }

        //can we store another order ID (I assumed max of 10)
        if (orderCount >= 10) {
            return false;   
        }

       
        orderIDs[orderCount] = orderID;
        orderCount++;
        occupiedSeats += seats;

        // Update occupiedUntil 
        int newFreeTime = currentTime + duration;
        if (newFreeTime > occupiedUntil) {
            occupiedUntil = newFreeTime;
        }

        return true;
    }

    // Free the table completely 
    void freeTable(int currentTime) {
        if (occupiedUntil <= currentTime) {
            
            occupiedSeats = 0;
            occupiedUntil = 0;
            orderCount = 0; //ik there will be memory waste ( I'll make linked list in phase 2)
        }
    }

    // Remove a specific order (cancellation) for shared tables
    void removeOrder(int orderID, int seats) {
        
        for (int i = 0; i < orderCount; i++) {
            if (orderIDs[i] == orderID) {
                
                for (int j = i; j < orderCount - 1; j++) {
                    orderIDs[j] = orderIDs[j + 1];
                }
                orderCount--;
              
                
                occupiedSeats -= seats;// Update occupied seats


                
                if (orderCount == 0) {
                    // table free when no orders left
                    occupiedSeats = 0;
                    occupiedUntil = 0;
                }
                return;  
            }
        }
        cout << "order not found!" << endl;
    }

    // Friend operator<< to access members
    friend ostream& operator<<(ostream& os, const Table* table) {
        if (table) {
            os << "[T" << table->id
                << ", " << table->capacity
                << ", " << table->getFreeSeats() << "]";
        }
        else {
            os << "[null]";
        }
        return os;
    }
};

#endif
