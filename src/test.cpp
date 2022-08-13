// Used for test.cpp
#include <iostream>
#include <ctime>
#include <mutex>
std::mutex(time_mutex);
std::mutex(print_mutex);


// Include necessary class
#include "Publisher.h"
#include "Subscriber.h"

// Include Topic
#include "iso.h"

// test.cpp get_time method
uint64_t get_time()
{
    std::unique_lock<std::mutex> lock(time_mutex);
    std::time_t time = std::time(nullptr);
    return time;
}



int main(int argc, char** argv) {

    std::cout << "Start of test\n";

    // Topic structs
    iso_s my_iso_struct_send{};
    iso_s my_iso_struct_recv{};

    // Creating Subscriber 
    Subscriber<iso_s> subiso{TOPIC_ID(iso)};

    // "iso" topic should not be advertised now
    std::cout << "Topic advertised? (no)" << ((subiso.advertised()) ? "Yes" : "No") <<std::endl;

    // Creating Publisher
    Publisher<iso_s> pubiso{TOPIC_ID(iso)};

    // "iso" topic should be advertised, constructor of publisher advertise to topic
    std::cout << "Topic advertised: (yes)" << ((subiso.advertised()) ? "Yes" : "No")<<std::endl;

    // We can check from the Publisher perspective
    std::cout << "Is this Publisher advertised the topic?: (yes)" << ((pubiso.advertised()) ? "Yes" : "No")<<std::endl;

    // Creating 2nd Publisher
    Publisher<iso_s> pubiso2{TOPIC_ID(iso)};

    // Unadvertise 2nd publisher
    pubiso2.unadvertise();

    // Checking from publisher side
    std::cout << "Is 2nd Publisher advertised the topic?: (no)" << ((pubiso2.advertised()) ? "Yes" : "No") <<std::endl;

    // Since first Publisher still a advertiser to topic we can see that topic is advertised
    std::cout << "Topic advertised: (yes)" << ((subiso.advertised()) ? "Yes" : "No") <<std::endl;

    // Checking if topic updated since our last copy, nothing published so no.
    std::cout << "Topic updated: (no)" << ((subiso.updated()) ? "Yes" : "No") <<std::endl;

    // Filling the struct
    my_iso_struct_send.timestamp = get_time();
    my_iso_struct_send.iso = 5;

    // Publishing the struct
    pubiso.publish(&my_iso_struct_send);

    // Another method to publish
    pubiso2.set(my_iso_struct_send);
    // When this publisher publising, it is advertising first to publish
    pubiso2.update();
    std::cout << "Is 2nd Publisher advertised the topic again?: (yes)" << ((pubiso2.advertised()) ? "Yes" : "No") <<std::endl;


    std::cout << "Published timestamp = " << unsigned(my_iso_struct_send.timestamp) <<std::endl;
    std::cout << "Published val = " << unsigned(my_iso_struct_send.iso) <<std::endl;

    // Checking if topic updated. We just published so yes.
    std::cout << "Topic updated: (yes)" << ((subiso.updated()) ? "Yes" : "No") <<std::endl;

    // Copy to my_iso_struct_recv with subscriber 
    subiso.copy(&my_iso_struct_recv);
    std::cout << "Received timestamp = " << unsigned(my_iso_struct_recv.timestamp) <<std::endl;
    std::cout << "Received val = " << unsigned(my_iso_struct_recv.iso) <<std::endl;

    // Checking if topic is updated. We just copied recent published data. So no, we are up to date.
    std::cout << "Topic updated: (no)" << ((subiso.updated()) ? "Yes" : "No") <<std::endl;


    // Unadvertise topic
    pubiso.unadvertise();
    pubiso2.unadvertise();

    // We can check if topic advertised from both sides. 
    // There are no publisher left to advertise
    std::cout << "Is first Publisher advertised the topic?: (no)" << ((pubiso.advertised()) ? "Yes" : "No")<<std::endl;
    std::cout << "Topic advertised? (no)" << ((subiso.advertised()) ? "Yes" : "No") <<std::endl;

/* ------------------------------------------------------------------------- */

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

/* ------------------------------------------------------------------------- */

    
    std::thread t_sub([](){
        std::cout << "Subsriber thread started"<<std::endl;

        Subscriber<iso_s> sub_iso{TOPIC_ID(iso)};

        iso_s iso_struct_sub{};

        for(int i = 0; i < 20; i++){
            sub_iso.copy(&iso_struct_sub);
            print_mutex.lock();
            std::cout << "Received timestamp = " << unsigned(iso_struct_sub.timestamp) <<std::endl;
            std::cout << "Received val = " << unsigned(iso_struct_sub.iso) <<std::endl;
            print_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
    });

    std::thread t_pub([](){
        std::cout << "Publisher thread started"<<std::endl;

        Publisher<iso_s> pub_iso{TOPIC_ID(iso)};

        iso_s iso_struct{};

        iso_struct.timestamp = 0xEC;
        iso_struct.iso = 1;

        for(int i = 0; i < 20; i++){
            // Increase iso value
            iso_struct.iso++;
            iso_struct.timestamp = get_time();

            pub_iso.publish(&iso_struct);
            print_mutex.lock();
            std::cout << "Published timestamp = " << unsigned(iso_struct.timestamp) <<std::endl;
            std::cout << "Published val = " << unsigned(iso_struct.iso) <<std::endl;
            print_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
    });

    t_sub.join();
    t_pub.join();

    return (my_iso_struct_send.timestamp == my_iso_struct_recv.timestamp);
}
