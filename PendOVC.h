#pragma once
#ifndef PEND_OVC_H
#define PEND_OVC_H

#include "LinkedQueue.h"
#include "Order.h"

class PendOVC : public LinkedQueue<Order*> {
public:
    
    void cancelOrder(int orderID) {
        //  will do it in phase 2
    }
};

#endif

