#ifndef REQUEST_ACTION_H
#define REQUEST_ACTION_H

#include "Action.h"
#include <string>
using namespace std;

class RequestAction : public Action {
private:
    string typ;
    int    id;
    int    size;
    double price;
    // OD-specific
    int    seats;
    int    duration;
    bool   canShare;
    // OV-specific
    int    distance;
public:
    RequestAction(int t, string typ, int id, int size, double price,
                  int seats, int duration, bool canShare, int distance)
        : Action(t), typ(typ), id(id), size(size), price(price),
          seats(seats), duration(duration), canShare(canShare), distance(distance)
    {}

    string getTyp()      const { return typ; }
    int    getId()       const { return id; }
    int    getSize()     const { return size; }
    double getPrice()    const { return price; }
    int    getSeats()    const { return seats; }
    int    getDuration() const { return duration; }
    bool   getCanShare() const { return canShare; }
    int    getDistance() const { return distance; }

    void Act(Restaurant* r) override {
        // Phase 2: create order and add to pending list
    }
};

#endif
