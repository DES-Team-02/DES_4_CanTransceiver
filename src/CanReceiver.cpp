#include "CanReceiver.hpp"


CanReceiver::CanReceiver(const std::string& interface):
    _interface(interface),
    _soc(-1),
    _running(false){  
}


CanReceiver::~CanReceiver() {
    stop();
}

bool CanReceiver::start() {
    if (openPort() < 0) {
        return false;
    }
    _running = true;
    _dataThread = std::thread(&CanReceiver::readData, this);
    return true;
}

void CanReceiver::stop() {
    _running = false;
    if (_dataThread.joinable()) {
        _dataThread.join();
    }
    closePort();
}


int CanReceiver::openPort() {
    struct ifreq ifr;           // Interface request structure for socket ioctls
    struct sockaddr_can addr;   // Address structure for the CAN socket

    // Open a CAN socket
    _soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);  
    // Check if socket is created successfully
    if (_soc < 0) 
    {
	    std::cerr << "Socket Creation Error!" << std::endl;
        return -1;
    }
    addr.can_family = AF_CAN;  
    strcpy(ifr.ifr_name, _interface.c_str());  

    if (ioctl(_soc, SIOCGIFINDEX, &ifr) < 0) 
    {
	    std::cerr << "I/O Control Error." << std::endl;
        // std::cerr << errno << std::endl;
        close(_soc);
        return (-1);
    }
    // Set the family type for the address to CAN
    addr.can_ifindex = ifr.ifr_ifindex;  
    fcntl(_soc, F_SETFL, O_NONBLOCK);  
    // Bind the socket to the CAN interface
    if (bind(_soc, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    {
	    std::cerr << "Binding Error." << std::endl;
        close(_soc);
        return -1;
    }
    return 0;
}


void CanReceiver::readData() {
    struct can_frame frame;
    while (_running) {
        ssize_t nbytes = recv(_soc, &frame, sizeof(struct can_frame), MSG_WAITALL); // Blocking call
        if (nbytes == sizeof(struct can_frame)) {
            std::lock_guard<std::mutex> lock(_mutex);
            _dataBuffer = std::vector<uint8_t>(frame.data, frame.data + frame.can_dlc);
            // std::cout << "Received data: ";
            // for (auto byte : _dataBuffer) {
            //     std::cout << std::hex << static_cast<int>(byte) << " ";
            // }
            // std::cout << std::dec << "\n";
        }
    }

}

std::vector<uint8_t> CanReceiver::getReceivedData() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _dataBuffer; // Return a copy of the data buffer
}

void CanReceiver::closePort() {
        close(_soc);
}