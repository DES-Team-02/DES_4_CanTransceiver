#include <CanReceiver.hpp>

CanReceiver::CanReceiver():
    _soc(-1),
    _rawRpm(0), 
    _sensorFrontLeft(0), 
    _sensorFrontMiddle(0), 
    _sensorFrontRight(0), 
    _running(false),
    _interface_name("can0")
{}


CanReceiver::CanReceiver(std::string ifn):
    _soc(-1),
    _rawRpm(0), 
    _sensorFrontLeft(0), 
    _sensorFrontMiddle(0), 
    _sensorFrontRight(0), 
    _running(false),
    _interface_name(ifn)
{}


CanReceiver::~CanReceiver() {
    closePort();
}

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


void CanReceiver::readData(){
    struct can_frame frame;
    while(_running){
        
        //receive data
        ssize_t nbytes = recv(_soc, &frame, sizeof(struct can_frame), MSG_WAITALL);

        if(nbytes == sizeof(struct can_frame)){
            //save timestamp if can frame received
            _last_time = std::chrono::steady_clock::now();
        }
        switch(frame.can_id){
            case 0x100: {
                std::lock_guard<std::mutex> lock(_dataMutex);
                int received_raw_rpm = frame.data[0] << 8 | frame.data[1];
                 raw_rpm = received_raw_rpm;
            }
            case 0x200: {
            // read raw data from frame & store
            std::lock_guard<std::mutex> lock(dataMutex);
            short received_sensor_0 = frame.data[0] << 8 | frame.data[1]; 
            short received_sensor_1 = frame.data[2] << 8 | frame.data[3]; 
            short received_sensor_2 = frame.data[4] << 8 | frame.data[5]; 
            // print received data
            std::cout << "----------------------------------" << std::endl;
            std::cout << "Received Sensor0   : " << sensor0   << std::endl;
            std::cout << "Received Sensor1   : " << sensor1   << std::endl; 
            std::cout << "Received Sensor2   : " << sensor2   << std::endl;
            // store data in class variables
            sensor0 = received_sensor_0;
            sensor1 = received_sensor_1;
            sensor2 = received_sensor_2;
            }
        }
    }        
}


void CanReceiver::closePort() {
    close(_soc);
}

int CanReceiver::run(){
    while(openPort(_interface_name) < 0){
        std::cerr << "Open port retrying >>>" << std::endl;

        else : break;
    }

    _running = true;
    _last_time = std::chrono::steady_clock::now();

    std::thread readerThread(&CanReceiver::readData, this);
    std::thread processThread(&CanReceiver::filteringData, this);

    readerThread.join();
    processThread.join();


    closePort();
    
}