//
//  main.cpp
//  C++_thread
//
//  Created by Tyrone Hou on 01/12/2020.
//  Copyright © 2020 Tyrone Hou. All rights reserved.
//

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <condition_variable>
using namespace std;

void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "F1: Thread " << n << " executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void f2(int& n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "F2: Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}




int readPosition = 0;
int writePosition = 0;
int bufferSize = 10;
condition_variable buffer_not_full;
condition_variable buffer_not_empty;
mutex mtx;
int productSize = 1000;

void producer()
{
   while(true)
    //for(int i=0;i<productSize;i++)
    {
        unique_lock<mutex> lck(mtx);
        while( (writePosition+1) % bufferSize == readPosition)    // writing too fast, wait here
        {
            cout<<"[PRODUCER] Write too fast, wait a moment..."<<endl;
            buffer_not_full.wait(lck);
        }
        
        // write something
        cout<<"[PRODUCER] Producing the "<<writePosition<<"th product."<<endl;
        writePosition++;
        writePosition %= bufferSize;
        
        lck.unlock();
        buffer_not_empty.notify_all();
        
        
    }

}


void consumer()
{
//    unique_lock<mutex> lck(mtx);
    int cnt=0;
    
    while(true)
    {
        unique_lock<mutex> lck(mtx);
        while(readPosition == writePosition)
        {
            cout<<"[CONSUMER] Read too fast, wait a moment..."<<endl;
            buffer_not_empty.wait(lck);
        }
        
        cout<<"[CONSUMER] Consuming the "<<readPosition<<"th product."<<endl;
        readPosition++;
        readPosition %= bufferSize;
        cnt++;
        
        lck.unlock();
        buffer_not_full.notify_all();
        
        //if(cnt==productSize) break;
    }
    
}



int main()
{
    thread p(producer);
    thread c(consumer);
    
    p.join();
    c.join();
    
    

//    int n = 0;
//    std::thread t1; // t1 is not a thread
//    std::thread t2(f1, n + 1); // pass by value
//    std::thread t3(f2, std::ref(n)); // pass by reference
//    std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread
//    t2.join();
//    t4.join();
//    std::cout << "Final value of n is " << n << '\n';
//
    
    
    return 0;
}
