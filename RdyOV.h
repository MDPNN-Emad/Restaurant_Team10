#pragma once
#ifndef RDY_OV_H
#define RDY_OV_H

#include "LinkedQueue.h"
#include "Order.h"

class RdyOV : public LinkedQueue<Order*> {
public:


    void cancelOrder(int orderID) {
        //  will do it in phase 2
    }
};

#endif

