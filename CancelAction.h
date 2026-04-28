#ifndef CANCEL_ACTION_H
#define CANCEL_ACTION_H

#include "Action.h"
#include "Restaurant.h"

class CancelAction : public Action {
private:
    int orderID;
public:
    CancelAction(int t, int id) : Action(t), orderID(id) {}

    int getOrderID() const { return orderID; }

    void Act(Restaurant* r) override {
        r->cancelOrder(orderID);
    }
};

#endif
