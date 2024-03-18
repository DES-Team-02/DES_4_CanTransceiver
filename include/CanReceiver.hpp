#ifndef CAN_RECEIVER_HPP
#define CAN_RECEIVER_HPP

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
#include <fcntl.h>
#include <mutex>

class CanReceiver{

public:
    CanReceiver(const std::vector<std::string>& interfaces);
    ~CanReceiver();
    int run();

private:

    std::vector<std::string> _interfaces;
    int _soc;

    std::atomic<bool> _running;
    std::mutex _dataMutex;

    short _rawRpm;
    short _sensorFrontLeft;
    short _sensorFrontMiddle;
    short _sensorFrontRight;

    std::chrono::steady_clock::time_point _last_time;

    int openPort(const char* interface);
    void readData(int socket, const std::string& interface);
    void filteringData();
    void sendData();
    void closePort();

};



#endif