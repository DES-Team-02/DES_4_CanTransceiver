#ifndef CAN_RECEIVER_HPP
#define CAN_RECEIVER_HPP

#include "MovingAverageFilter.hpp"
#include "CanDataRegister.hpp"

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

class CanReceiver
{

public:
    CanReceiver(const std::string &interface);
    ~CanReceiver();
    int run();

private:
    CanDataRegister dataRegister;

    const double FACTOR = 0.025;
    const double WHEEL_RADIUS = 0.065;

    std::string _interface;
    int _soc;

    std::atomic<bool> _running;
    std::mutex _rpmDataMutex;
    std::mutex _sonarDataMutex;

    int _rawRpm;
    double _filteredRpm;
    double _filteredSpeed;
    int _sensorFrontLeft;
    int _sensorFrontMiddle;
    int _sensorFrontRight;
    

    std::chrono::steady_clock::time_point _last_time;

    int openPort(const char *interface);
    void readData(int socket, const std::string &interface);
    void rpmDataFilter(int currentRpm);
    void registerRpm();
    void registerSonar();
    void closePort();
};

#endif