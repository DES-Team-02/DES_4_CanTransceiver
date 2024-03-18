#include "CanReceiver.hpp"

CanReceiver::CanReceiver(const std::vector<std::string>& interfaces):
    _interfaces(std::vector<std::string> _interfaces = { "can0", "can1"};),
    _soc(-1),
    _running(false),
    _rawRpm(0), 
    _sensorFrontLeft(0), 
    _sensorFrontMiddle(0), 
    _sensorFrontRight(0) {  
}


CanReceiver::~CanReceiver() {}

int CanReceiver::openPort(const char* interface) {
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
    // Set the family type for the address to CAN
    addr.can_family = AF_CAN;  
    // Copy the port name to the ifreq structure
    strcpy(ifr.ifr_name, _interface_name.c_str());  
    // Get the interface index of the CAN device
    if (ioctl(_soc, SIOCGIFINDEX, &ifr) < 0) 
    {
	    std::cerr << "I/O Control Error." << std::endl;
        close(_soc);
        return (-1);
    }
    // Get the interface index from the ifreq structure
    addr.can_ifindex = ifr.ifr_ifindex;  
    // Set the socket to non-blocking mode
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


void CanReceiver::readData(int socket, const std::string& interface){
    while(_running){
        struct can_frame frame;
        ssize_t nbytes = recv(socket, &frame, sizeof(frame), MSG_WAITALL);

        if(nbytes == sizeof(frame)){
            //save timestamp if can frame received
            _last_time = std::chrono::steady_clock::now();
            if(interface == "can0"){
                std::lock_guard<std::mutex> lock(_dataMutex);
                short received_raw_rpm = frame.data[0] << 8 | frame.data[1];
                // print received data
                std::cout << "Received rawRpm   : " << short received_raw_rpm << std::endl;
                _rawRpm = received_raw_rpm;
                } else if (interface == "can1"){
                // read raw data from frame & store
                std::lock_guard<std::mutex> lock(dataMutex);
                short received_sensor_0 = frame.data[0] << 8 | frame.data[1]; 
                short received_sensor_1 = frame.data[2] << 8 | frame.data[3]; 
                short received_sensor_2 = frame.data[4] << 8 | frame.data[5]; 
                // print received data
                std::cout << "----------------------------------" << std::endl;
                std::cout << "Received Sensor0   : " << short received_sensor0 << std::endl;
                std::cout << "Received Sensor1   : " << short received_sensor1 << std::endl; 
                std::cout << "Received Sensor2   : " << short received_sensor2 << std::endl;
                std::cout << "----------------------------------" << std::endl;
                // store data in class variables
                _sensorFrontLeft = received_sensor_0;
                _sensorFrontMiddle = received_sensor_1;
                _sensorFrontRight = received_sensor_2;
            }
        }
    }        
}


void CanReceiver::closePort(int soc) {
    if (soc != -1){
        close(soc);
    };
}

int CanReceiver::run(){
    while(openPort(_interface_name) < 0){
        std::cerr << "Open port retrying >>>" << std::endl;
        sleep_for(1); 
    }

    _running = true;
    _last_time = std::chrono::steady_clock::now();

    std::thread readerThread(&CanReceiver::readData, this);
    std::thread processThread(&CanReceiver::filteringData, this);

    readerThread.join();
    processThread.join();


    closePort();
    
}