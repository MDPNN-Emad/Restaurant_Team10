#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
using namespace std;

class Chef; 

class Order {
private:
    // Basic order information for all orders:
    int id;                 
    string type;        
    // ODG, ODN, OT, OVC, OVG, OVN

    int request_time;       
    // When order was placed

    int size;               
    // Number of dishes

    double price;          

    // Timestamps
    int assignment_time;     
    int ready_time;          
    int service_start_time;   
    int finish_time;         

    // For Dine in orders (ODG and ODN) 
    int seats;              
    int order_duration;      
    bool can_share;          

    // For Delivery orders (OVC, OVG, OVN)
    int distance;

    Chef* assigned_chef;

public:
    // Constructor for Dine in orders (ODG, ODN)
    Order(int order_id, string order_type, int req_time, int order_size, double order_price,
        int num_seats, int duration, bool shareable) {

        id = order_id;
        type = order_type;
        request_time = req_time;
        size = order_size;
        price = order_price;
        seats = num_seats;
        order_duration = duration;
        can_share = shareable;

        assignment_time = -1;
        ready_time = -1;
        service_start_time = -1;
        finish_time = -1;
        distance = -1;
        assigned_chef = nullptr;
    }

    // Constructor for Delivery orders (OVC, OVG, OVN)
    Order(int order_id, string order_type, int req_time, int order_size, double order_price,
        int dist_in_meters) {

        id = order_id;
        type = order_type;
        request_time = req_time;
        size = order_size;
        price = order_price;
        distance = dist_in_meters;

        assignment_time = -1;
        ready_time = -1;
        service_start_time = -1;
        finish_time = -1;
        seats = -1;
        order_duration = -1;
        can_share = false;
        assigned_chef = nullptr;
    }

    // Constructor for Takeaway orders (OT)
    Order(int order_id, string order_type, int req_time, int order_size, double order_price) {

        id = order_id;
        type = order_type;
        request_time = req_time;
        size = order_size;
        price = order_price;

        assignment_time = -1;
        ready_time = -1;
        service_start_time = -1;
        finish_time = -1;
        seats = -1;
        order_duration = -1;
        can_share = false;
        distance = -1;
        assigned_chef = nullptr;
    }

    // getters
    int get_id() const { return id; }
    string get_type() const { return type; }
    int get_request_time() const { return request_time; }
    int get_size() const { return size; }
    double get_price() const { return price; }
    int get_assignment_time() const { return assignment_time; }
    int get_ready_time() const { return ready_time; }
    int get_service_start_time() const { return service_start_time; }
    int get_finish_time() const { return finish_time; }
    int get_seats() const { return seats; }
    int get_order_duration() const { return order_duration; }
    bool get_can_share() const { return can_share; }
    int get_distance() const { return distance; }
    Chef* get_assigned_chef() const { return assigned_chef; }
    void set_assigned_chef(Chef* c) { assigned_chef = c; }

    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;

    //setters(mesh han3mel behom 7aga dlwa2ti)
    void set_assignment_time(int time) { assignment_time = time; }
    void set_ready_time(int time) { ready_time = time; }
    void set_service_start_time(int time) { service_start_time = time; }
    void set_finish_time(int time) { finish_time = time; }

    //print func
    void print() const  {
        cout << "[" << type << " " << id << "]";
        if (type == "OT") {
            cout << " Size:" << size << " Price:" << price;
        }
        else if (type == "ODG" || type == "ODN") {
            cout << " Size:" << size << " Price:" << price
                << " Seats:" << seats << " Dur:" << order_duration;
            if (can_share) cout << " Share:Y";
            else cout << " Share:N";
        }
        else if (type == "OVC" || type == "OVG" || type == "OVN") {
            cout << " Size:" << size << " Price:" << price
                << " Dist:" << distance;
        }
    }
};


// Overload 
inline ostream& operator<<(ostream& os, const Order* order) {
    order->print();
    return os;
}

#endif