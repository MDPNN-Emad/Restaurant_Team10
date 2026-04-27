#ifndef ACTION_H
#define ACTION_H

class Restaurant;  // forward declaration

class Action {
protected:
    int timestep;   // when this action fires
public:
    Action(int t) : timestep(t) {}
    int getTimestep() const { return timestep; }
    virtual void Act(Restaurant* r) = 0;   // pure virtual — defined in derived classes
    virtual ~Action() {}
};

#endif
