#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <thread>
#include "CanTransceiverStubImpl.hpp"
#include "RpmProcess.hpp"
#include "SonarProcess.hpp"

#define NUMBER_OF_CAN_INTERFACES 1
#define SPEED_SENSOR_CAN_ID      0x100
#define SONAR_SENSOR_CAN_ID      0x200

int create_and_bind_socket(const std::string& interface)
{
    struct ifreq ifr;           // Interface request structure for socket ioctls
    struct sockaddr_can addr;   // Address structure for the CAN socket

    int soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (soc < 0)
    {
        std::cerr << "Socket Creation Error!" << std::endl;
        return -1;
    }
    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, interface.c_str());

    if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0)
    {
        std::cerr << "I/O Control Error." << std::endl;
        // std::cerr << errno << std::endl;
        close(soc);
        return (-1);
    }
    addr.can_ifindex = ifr.ifr_ifindex;
    // fcntl(soc, F_SETFL, O_NONBLOCK);

    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Binding Error." << std::endl;
        close(soc);
        return -1;
    }
    return soc;
}

int add_interface_to_epoll(int epoll_fd, const std::string& interface)
{
    struct epoll_event event;
    int fd;

    fd = create_and_bind_socket(interface);
    if (fd < 0)
        return -1;

    event.events = EPOLLIN;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        std::cerr << "epoll_ctl failed for fd: " << fd << std::endl;
        close(fd);
        return -1;
    }
    return 0;
}

int main(void)
{
    int epoll_fd, nfds;
    struct epoll_event events[NUMBER_OF_CAN_INTERFACES];

    // Create Epoll Instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        std::cerr << "epoll_create1 failed" << std::endl;
        return 1;
    }

    if (add_interface_to_epoll(epoll_fd, "can0") < 0)
    {
        close(epoll_fd);
        return 1;
    }

    // Create CommonAPI Service
    std::shared_ptr<CanTransceiverStubImpl> myService = std::make_shared<CanTransceiverStubImpl>();
    myService->init();

    while (true)
    {
        nfds = epoll_wait(epoll_fd, events, NUMBER_OF_CAN_INTERFACES, 100); // 100 millisecond timeout
        if (nfds == -1)
        {
            std::cerr << "epoll_wait failed" << std::endl;
            break;
        }

        for (int n = 0; n < nfds; ++n)
        {
            if (events[n].events & EPOLLIN)
            {
                struct can_frame frame;
                ssize_t nbytes = recv(events[n].data.fd, &frame, sizeof(struct can_frame), 0);
                switch (frame.can_id)
                {
                case SPEED_SENSOR_CAN_ID:
                {
                    RpmData processed = RpmProcess::process(frame);
                    // std::cout << "rpm: " << processed.rpm
                    //         << ", speed: " << processed.speed
                    //         << std::endl;
                    myService->setRpmAttribute(processed.rpm);
                    myService->setSpeedAttribute(processed.speed);
                    break;
                }
                case SONAR_SENSOR_CAN_ID:
                {
                    SonarData processed = SonarProcess::process(frame);
                    // std::cout << "Sonar front: " << processed.getSonarFront()
                    //         << ", rear: " << processed.getSonarRear()
                    //         << std::endl;
                    myService->setDistancesAttribute(processed);
                    myService->setSonarFrontAttribute(processed.getSonarFront());
                    myService->setSonarRearAttribute(processed.getSonarRear());
                    break;
                }
                default:
                    std::cout << "Unknown CAN ID: 0x" << std::hex << frame.can_id << std::endl;
                    break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    close(epoll_fd);
    return 0;
}
