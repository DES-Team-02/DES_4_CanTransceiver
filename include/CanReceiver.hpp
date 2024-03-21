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
#include <thread>
#include <math.h>
#include <vector>

class CanReceiver
{

public:
    CanReceiver(const std::string &interface);
    ~CanReceiver();
    bool start();
    void stop();
    std::vector<uint8_t> getReceivedData();

private:
    std::string _interface;
    int _soc;

    std::atomic<bool> _running;
    std::mutex _mutex; 
    std::vector<uint8_t> _dataBuffer;
    std::thread _dataThread;

    int openPort();
    void readData();
    void closePort();
};

#endif