#ifndef CANCEL_ACTION_H
#define CANCEL_ACTION_H

#include "Action.h"

class CancelAction : public Action {
private:
    int orderID;
public:
    CancelAction(int t, int id) : Action(t), orderID(id) {}

    void Act(Restaurant* r) override {
        // Phase 2: cancel the order
    }
};

#endif
