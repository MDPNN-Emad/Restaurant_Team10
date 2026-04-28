#ifndef REQUEST_ACTION_H
#define REQUEST_ACTION_H

#include "Action.h"
#include "Restaurant.h"
#include <string>
using namespace std;

class RequestAction : public Action {
private:
    string typ;
    int    id;
    int    size;
    double price;
    int    seats     = -1;
    int    duration  = -1;
    bool   canShare  = false;
    int    distance  = -1;
public:
    // dine-in (OD): (ODG, ODN)
    RequestAction(int t, string typ, int id, int size, double price,
                  int seats, int duration, bool canShare)
        : Action(t), typ(typ), id(id), size(size), price(price),
          seats(seats), duration(duration), canShare(canShare)
    {}

    // delivery (OV): (OVC, OVG, OVN)
    RequestAction(int t, string typ, int id, int size, double price,
                  int distance)
        : Action(t), typ(typ), id(id), size(size), price(price),
          distance(distance)
    {}

    // takeaway (OT)
    RequestAction(int t, string typ, int id, int size, double price)
        : Action(t), typ(typ), id(id), size(size), price(price)
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
        Order* o = nullptr;
        if (typ == "ODG" || typ == "ODN")
            o = new Order(id, typ, timestep, size, price, seats, duration, canShare);
        else if (typ == "OVC" || typ == "OVG" || typ == "OVN")
            o = new Order(id, typ, timestep, size, price, distance);
        else // OT
            o = new Order(id, typ, timestep, size, price);
        r->addPendingOrder(o);
    }
};

#endif
