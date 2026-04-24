#ifndef RESTAURANT_H
#define RESTAURANT_H

#include "Order.h"
#include "Chef.h"
#include "Scooter.h"
#include "Table.h"
#include "LinkedQueue.h"
#include "PriQueue.h"
#include "RandomGenerator.h"
#include "UI.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Restaurant {
private:
    // Pending orders
    LinkedQueue<Order*> pending_odg;
    LinkedQueue<Order*> pending_odn;
    LinkedQueue<Order*> pending_ot;
    LinkedQueue<Order*> pending_ovn;
    LinkedQueue<Order*> pending_ovc;
    priQueue<Order*> pending_ovg;

    // Cooking and ready
    LinkedQueue<Order*> cooking_orders;
    LinkedQueue<Order*> ready_od;
    LinkedQueue<Order*> ready_ot;
    LinkedQueue<Order*> ready_ov;

    // Completed
    LinkedQueue<Order*> finished_orders;
    LinkedQueue<Order*> cancelled_orders;

    // Resources
    LinkedQueue<Chef*> free_cn;
    LinkedQueue<Chef*> free_cs;
    LinkedQueue<Chef*> busy_chefs;
    LinkedQueue<Scooter*> free_scooters;
    LinkedQueue<Table*> free_tables;

    // Stats
    int finished_count;
    int cancelled_count;
    int total_orders;
    int current_time;

    UI ui;

public:
    Restaurant() : finished_count(0), cancelled_count(0), total_orders(500), current_time(0) {
        srand(time(nullptr));

        // Create resources
        for (int i = 1; i <= 10; i++) free_cn.enqueue(new Chef(i, "CN", 3));
        for (int i = 11; i <= 15; i++) free_cs.enqueue(new Chef(i, "CS", 5));
        for (int i = 1; i <= 10; i++) free_tables.enqueue(new Table(i, rand() % 5 + 2));
        for (int i = 1; i <= 8; i++) free_scooters.enqueue(new Scooter(i, 50, 8, 5));

        // Generate orders
        RandomGenerator rng;
        for (int i = 1; i <= 500; i++) {
            Order* order = rng.generate_random_order(i, rand() % 100);
            string type = order->get_type();

            if (type == "ODG") pending_odg.enqueue(order);
            else if (type == "ODN") pending_odn.enqueue(order);
            else if (type == "OT") pending_ot.enqueue(order);
            else if (type == "OVN") pending_ovn.enqueue(order);
            else if (type == "OVC") pending_ovc.enqueue(order);
            else if (type == "OVG") pending_ovg.enqueue(order, rand() % 10);
        }
    }

    void simulate() {
        while ((finished_count + cancelled_count) < total_orders && current_time < 200) {

            // Chefs work and finish orders
            Chef* chef;
            LinkedQueue<Chef*> temp_chefs;

            while (busy_chefs.dequeue(chef)) {
                chef->working();

                if (chef->is_finished()) {
                    Order* done = chef->get_current_order();
                    string type = done->get_type();

                    // Move from cooking to ready
                    removeFromCooking(done->get_id());

                    if (type == "OT") ready_ot.enqueue(done);
                    else if (type == "ODG" || type == "ODN") ready_od.enqueue(done);
                    else ready_ov.enqueue(done);

                    // Free chef
                    if (chef->get_specialization() == "CS") free_cs.enqueue(chef);
                    else free_cn.enqueue(chef);
                }
                else {
                    temp_chefs.enqueue(chef);
                }
            }

            while (temp_chefs.dequeue(chef)) {
                busy_chefs.enqueue(chef);
            }

            // Assigned left pending to chefs 30 times
            for (int i = 0; i < 30; i++) {
                Order* order = nullptr;
                int choice = rand() % 6;

                if (choice == 0) pending_odg.dequeue(order);
                else if (choice == 1) pending_odn.dequeue(order);
                else if (choice == 2) pending_ot.dequeue(order);
                else if (choice == 3) pending_ovn.dequeue(order);
                else if (choice == 4) pending_ovc.dequeue(order);
                else {
                    int p;
                    pending_ovg.dequeue(order, p);
                }

                if (order != nullptr) {
                    Chef* chef_assigned = nullptr;
                    string type = order->get_type();

                    if (type == "ODG" || type == "OVG") {
                        if (free_cs.dequeue(chef_assigned)) {
                            chef_assigned->assign_order(order);
                            busy_chefs.enqueue(chef_assigned);
                            cooking_orders.enqueue(order);
                        }
                        else {
                            if (type == "ODG") pending_odg.enqueue(order);
                            else pending_ovg.enqueue(order, rand() % 10);
                        }
                    }
                    else {
                        if (free_cn.dequeue(chef_assigned)) {
                            chef_assigned->assign_order(order);
                            busy_chefs.enqueue(chef_assigned);
                            cooking_orders.enqueue(order);
                        }
                        else if (free_cs.dequeue(chef_assigned)) {
                            chef_assigned->assign_order(order);
                            busy_chefs.enqueue(chef_assigned);
                            cooking_orders.enqueue(order);
                        }
                        else {
                            if (type == "ODN") pending_odn.enqueue(order);
                            else if (type == "OT") pending_ot.enqueue(order);
                            else if (type == "OVC") pending_ovc.enqueue(order);
                            else pending_ovn.enqueue(order);
                        }
                    }
                }
            }

            // take ready orders
            Order* ready;

            
            while (ready_ot.dequeue(ready)) {
                finished_orders.enqueue(ready);
                finished_count++;
            }

            //resources: (mazen)

            // Dine-in  needs table
            Table* table;
            while (ready_od.dequeue(ready)) {
                if (free_tables.dequeue(table)) {
                    finished_orders.enqueue(ready);
                    finished_count++;
                    free_tables.enqueue(table);
                }
                else {
                    ready_od.enqueue(ready);
                    break;
                }
            }

            // Delivery needs scooter
            Scooter* scooter;
            while (ready_ov.dequeue(ready)) {
                if (free_scooters.dequeue(scooter)) {
                    finished_orders.enqueue(ready);
                    finished_count++;
                    free_scooters.enqueue(scooter);
                }
                else {
                    ready_ov.enqueue(ready);
                    break;
                }
            }

            // Random 10% 
            if (rand() % 100 < 10) {
                Order* cancelled = nullptr;
                if (pending_ovc.dequeue(cancelled)) {
                    cancelled_orders.enqueue(cancelled);
                    cancelled_count++;
                }
            }

            // Print
            ui.printStatus(
                current_time, finished_count, cancelled_count, total_orders,
                pending_odg, pending_odn, pending_ot, pending_ovn, pending_ovc, pending_ovg,
                free_cn, free_cs, busy_chefs,
                cooking_orders,
                ready_od, ready_ot, ready_ov,
                free_tables, free_scooters,
                finished_orders, cancelled_orders
            );

            current_time++;

            cout << "Press Enter...";
            cin.get();
        }

        cout << "\n========== DONE! ==========\n";
        cout << "Simulation finished at time " << current_time << endl;
        cout << "Finished: " << finished_count << " | Cancelled: " << cancelled_count << endl;
    }

    void removeFromCooking(int order_id) {
        LinkedQueue<Order*> temp;
        Order* o;

        while (cooking_orders.dequeue(o)) {
            if (o->get_id() != order_id) {
                temp.enqueue(o);
            }
        }

        while (temp.dequeue(o)) {
            cooking_orders.enqueue(o);
        }
    }
};

#endif