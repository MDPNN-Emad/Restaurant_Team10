#pragma once
#ifndef COOK_ORDS_H
#define COOK_ORDS_H

#include "priQueue.h"
#include "Order.h"

class CookOrds : public priQueue<Order*> {
public:
   

    void cancelOrder(int orderID) {
        //  will do it in phase 2
    }
};

#endif

