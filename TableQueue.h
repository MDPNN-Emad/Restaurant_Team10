#pragma once
#ifndef TABLE_QUEUE_H
#define TABLE_QUEUE_H 

#include "priQueue.h"
#include "Table.h"
#include "Order.h"

class TableQueue : public priQueue<Table*> {
public:
    
    Table* getBestFit(Order* order) {// used to Find table with smallest capacity and fit the order
       
        
        return nullptr;
    }

    
    bool removeTable(int tableID) {
     
        return false;
    }

    void addTable(Table* table, int priority) {
        enqueue(table, priority);
    }
};

#endif

