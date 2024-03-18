#include "CanReceiver.hpp"

CanReceiver::CanReceiver(const std::string& interface):
    _interface(interface),
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
    strcpy(ifr.ifr_name, _interface);  
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
                int receivedRpm;
                int currentRpm;
                {
                    std::lock_guard<std::mutex> lock(_rpmDataMutex);
                    receivedRpm = frame.data[0] << 8 | frame.data[1];
                }
                // print received data
                std::cout << "----------------------------------" << std::endl;
                std::cout << "Received rawRpm   : " << receivedRpm << std::endl;
                std::cout << "----------------------------------" << std::endl;
                _rawRpm = receivedRpm;
            } else if (interface == "can1"){
                // read raw data from frame & store
                std::lock_guard<std::mutex> lock(_sonarDataMutex);
                int receivedSensor0 = frame.data[0] << 8 | frame.data[1]; 
                int receivedSensor1 = frame.data[2] << 8 | frame.data[3]; 
                int receivedSensor2 = frame.data[4] << 8 | frame.data[5]; 
                // print received data
                std::cout << "----------------------------------" << std::endl;
                std::cout << "Received Sensor0   : " << receivedSensor0 << std::endl;
                std::cout << "Received Sensor1   : " << receivedSensor1 << std::endl; 
                std::cout << "Received Sensor2   : " << receivedSensor2 << std::endl;
                std::cout << "----------------------------------" << std::endl;
                // store data in class variables
                _sensorFrontLeft = receivedSensor0;
                _sensorFrontMiddle = receivedSensor1;
                _sensorFrontRight = receivedSensor2;
            }
        }
    }        
}

void CanReceiver::rpmDataFilter(int currentRpm){

    MovingAverageFilter(10, 5);
    double filteredRpm;
    double filteredSpeed;
    while(_running) {
        // filtered rpm
        filteredRpm = rpmFilter.filter(currentRpm);
        // calculated speed (sensor wheel on wheel shaft )
        filteredSpeed = ((filteredRpm) / (WHEEL_RADIUS * 2 * M_PI)) / 6;
        
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Filtered RPM      : " << filteredRpm     << std::endl; 
        std::cout << "Filtered Speed    : " << filteredSpeed   << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
    {
        std::lock_guard<std::mutex> lock(_rpmDataMutex);
        _filteredRpm = filteredRpm;
        _filteredSpeed = filteredSpeed;
        registerRpm();
    }
}

void CanReceiver::registerRpm(){
    dataRegister.setServiceRpmAttributes(
        static_cast<uint32_t>(_filteredRpm), 
        static_cast<uint32_t>(_filteredSpeed)
    )
}

void CanReceiver::registerSonar(){
    dataRegister.setServiceSonarAttributes(
        static_cast<uint32_t>(_sensorFrontLeft),
        static_cast<uint32_t>(_sensorFrontMiddle),
        static_cast<uint32_t>(_sensorFrontRight)
    )
}

void CanReceiver::closePort() {
        close(_soc);
}

int CanReceiver::run(){
    while(openPort(_interface) < 0){
        std::cerr << "Open port retrying >>>" << std::endl;
        sleep_for(1); 
    }

    _running = true;
    _last_time = std::chrono::steady_clock::now();

    std::thread readerThread(&CanReceiver::readData, this);
    std::thread filterThread(&CanReceiver::rpmDataFilter, this);
    std::thread registerRpmThread(&CanReceiver::registerRpm , this);
    std::thread registerSonarThread(&CanReceiver::registerSonar , this);
    

    readerThread.join();
    filterThread.join();
    registerRpmThread.join();
    registerSonarThread.join();


    closePort();
    return 0;
}