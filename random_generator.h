#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include "Order.h"
#include "LinkedQueue.h"
#include "Chef.h"

#include <cstdlib>
#include <ctime>
using namespace std;

class RandomGenerator {
private:
    // Order type distribution percentages 
    int odg_percent = 15;  
    int odn_percent = 20;   
    int ot_percent = 20;    
    int ovc_percent = 15;   
    int ovg_percent = 10;   
    int ovn_percent = 20;   

    // returns a random number between min_val and max_val 
    int get_random(int min_val, int max_val) {
        return min_val + (rand() % (max_val - min_val + 1));
    }

    
    string get_random_order_type() {
        int random_num = get_random(1, 100);
        int cumulative = 0;

        
        cumulative = cumulative + odg_percent; 
        if (random_num <= cumulative) {
            return "ODG";
        }

        
        cumulative = cumulative + odn_percent;  
        if (random_num <= cumulative) {
            return "ODN";
        }

        
        cumulative = cumulative + ot_percent;   
        if (random_num <= cumulative) {
            return "OT";
        }


        cumulative = cumulative + ovc_percent;  
        if (random_num <= cumulative) {
            return "OVC";
        }

        cumulative = cumulative + ovg_percent;  
        if (random_num <= cumulative) {
            return "OVG";
        }
        
        return "OVN";
        
    }

public:
    Order* generate_random_order(int order_id, int request_time) {
        string order_type = get_random_order_type();

        int order_size = get_random(1, 10);      
        double order_price = get_random(200, 2000); 

        
        if (order_type == "OT") {
            return new Order(order_id, order_type, request_time,
                order_size, order_price);
        }
        
        else if (order_type == "ODG" || order_type == "ODN") {
            int num_seats = get_random(1, 8);           
            int duration = get_random(10, 60);          
            bool shareable = (get_random(1, 100) > 50);
            return new Order(order_id, order_type, request_time,
                order_size, order_price, num_seats, duration, shareable);
        }
       
        else {
            int distance_meters = get_random(500, 5000); // 500 to 5000 meters
            return new Order(order_id, order_type, request_time,
                order_size, order_price, distance_meters);
        }
    }

    // generate queues
    void generate_and_distribute_orders(
        LinkedQueue<Order*>& pending_odg,   
        LinkedQueue<Order*>& pending_odn,   
        LinkedQueue<Order*>& pending_ot,    
        LinkedQueue<Order*>& pending_ovc,   
        LinkedQueue<Order*>& pending_ovg,  
        LinkedQueue<Order*>& pending_ovn,   
        int number_of_orders = 500)
    {
        srand(static_cast<unsigned>(time(nullptr))); //makes the random func give different values every time

        cout << number_of_orders << " Orders have been generated";

        int count_odg = 0;
        int count_odn = 0;
        int count_ot = 0;
        int count_ovc = 0;
        int count_ovg = 0;
        int count_ovn = 0;

        // Create order
        for (int order_id = 1; order_id <= number_of_orders; order_id++) {
            int request_time = get_random(0, 200);
            Order* new_order = generate_random_order(order_id, request_time);

            string order_type = new_order->get_type();

            // Put the order in the correct pending queue
            if (order_type == "ODG") {
                pending_odg.enqueue(new_order);
                count_odg = count_odg + 1;
            }
            else if (order_type == "ODN") {
                pending_odn.enqueue(new_order);
                count_odn = count_odn + 1;
            }
            else if (order_type == "OT") {
                pending_ot.enqueue(new_order);
                count_ot = count_ot + 1;
            }
            else if (order_type == "OVC") {
                pending_ovc.enqueue(new_order);
                count_ovc = count_ovc + 1;
            }
            else if (order_type == "OVG") {
                pending_ovg.enqueue(new_order);
                count_ovg = count_ovg + 1;
            }
            else if (order_type == "OVN") {
                pending_ovn.enqueue(new_order);
                count_ovn = count_ovn + 1;
            }
        }

        // Print the distribution 
        cout << "Order Distribution" << "\n";
        cout << "  ODG (Dine in Grilled) " << count_odg << "\n";
        cout << "  ODN (Dine in Normal) " << count_odn << "\n";
        cout << "  OT (Takeaway) " << count_ot << "\n";
        cout << "  OVC (Delivery Cold) " << count_ovc << "\n";
        cout << "  OVG (Delivery Grilled) " << count_ovg << "\n";
        cout << "  OVN (Delivery Normal) " << count_ovn << "\n";

        int total = count_odg + count_odn + count_ot + count_ovc + count_ovg + count_ovn;
        cout << "  TOTAL: " << total << "\n";
    }
};

#endif