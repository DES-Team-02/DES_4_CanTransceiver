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
#include "CanTransceiverStubImpl.hpp"

#define NUMBER_OF_CAN_INTERFACES 2

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
    fcntl(soc, F_SETFL, O_NONBLOCK);

    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    {
        std::cerr << "Binding Error." << std::endl;
        close(soc);
        return -1;
    }
    return soc;
}

int main(void)
{
    int epoll_fd, nfds;
    struct epoll_event event, events[NUMBER_OF_CAN_INTERFACES];

    // Create Epoll Instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) 
    {
        std::cerr << "epoll_create1 failed" << std::endl;
        return 1;
    }

    int can0 = create_and_bind_socket("can0");
    if (can0 < 0)
    {
        close(epoll_fd);
        return 1;
    }

    event.events = EPOLLIN;
    event.data.fd = can0;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, can0, &event) == -1) {
        std::cerr << "epoll_ctl failed for can0" << std::endl;
        close(can0);
        close(epoll_fd);
        return 1;
    }

    while (true) {
        nfds = epoll_wait(epoll_fd, events, NUMBER_OF_CAN_INTERFACES, -1);
        if (nfds == -1) 
        {
            std::cerr << "epoll_wait failed" << std::endl;
            break;
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].events & EPOLLIN) 
            {
                struct can_frame frame;
                ssize_t nbytes = recv(soc, &frame, sizeof(struct can_frame), 0);
                switch (frame.can_id)
                {
                case 0x100:
                {
                    std::cout << "speed" << std::endl;
                    break;
                }
                case 0x200:
                {
                    std::cout << "sonar" << std::endl;
                    break;
                }
                default:
                    std::cout << "Unknown CAN ID" << std::endl;
                    break;
                }
            }
        }
    }

    return 0;
}
