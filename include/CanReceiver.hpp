#ifndef CANRECEIVER_HPP
#define CANRECEIVER_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <atomic> 
#include <chrono>
#include <mutex>

class CanReceiver{

public:
    CanReceiver();
    CanReceiver(std::string ifn);
    ~CanReceiver();
    int run();

private:

    // const char* CAN_INTERFACE_0 = "can0";
    // const char* CAN_INTERFACE_1 = "can1";
    const std::string _interface_name;

    int _soc;

    short _rawRpm;
    short _sensorFrontLeft;
    short _sensorFrontRight;
    short _sensorFrontMiddle;

    std::mutex _dataMutex;
    std::atomic<bool> _running;
    std::chrono::steady_clock::time_point _last_time;

    


    int openPort(const char* interface);
    void readData();
    void filteringData();
    void sendData();
    void closePort();


};



#endif