#ifndef CHEF_H
#define CHEF_H

#include <iostream>
#include <string>
#include "Order.h"
using namespace std;

class Chef {
private:
    int id;
    string specialization; 
    // special Cs or normal CN

    int speed;   
    // da dishes le kol timestamp

    bool is_available;

    Order* current_order; 
    // null if free

    int remaining_dishes;      

public:
    Chef(int chef_id, string chef_specialization, int chef_speed) {
        id = chef_id;
        specialization = chef_specialization;
        speed = chef_speed;
        is_available = true;
        current_order = nullptr;
        remaining_dishes = 0;
    }

    Chef(const Chef&) = delete;
    Chef& operator=(const Chef&) = delete;

    // Getters
    int get_id() const { return id; }
    string get_specialization() const { return specialization; }
    int get_speed() const { return speed; }
    bool get_available() const { return is_available; }
    Order* get_current_order() const { return current_order; }
    int get_remaining_work() const { return remaining_dishes; }

    // Setters
    void set_available(bool available) { is_available = available; }

    // Assignment functions
    void assign_order(Order* order) {
        current_order = order;
        remaining_dishes = order->get_size();
        is_available = false;
    }

    void release_order() {
        current_order = nullptr;
        remaining_dishes = 0;
        is_available = true;
    }

    void working() {
        if (current_order != nullptr) {
            remaining_dishes = remaining_dishes - speed;
            if (remaining_dishes <= 0) {
                remaining_dishes = 0;
            }
        }
    }

    bool is_finished() {
        return (current_order != nullptr && remaining_dishes <= 0);
    }

    void print() const{
        cout << "Chef " << id << " (" << specialization << ")";
        if (!is_available) {
            cout << " cooking order " << current_order->get_id();
        }
    }

	//overloading the << operator for easy printing of chef details

    friend ostream& operator<<(ostream& os, const Chef* chef) {
        if (chef) {
            os << "Chef " << chef->id << " (" << chef->specialization << ")";
            if (!chef->is_available && chef->current_order) {
                os << " cooking order " << chef->current_order->get_id();
            }
        }
        else {
            os << "Chef(null)";
        }
        return os;
    }
};


#endif