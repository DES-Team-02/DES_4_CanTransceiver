# include "CanReceiver.hpp"
# include "RpmProcess.hpp"
# include "SonarProcess.hpp"
# include "CanTransceiverStubImpl.hpp"

#define SPEED_SENSOR_CAN_ID 100
#define SONAR_SENSOR_CAN_ID 200

void processData(struct can_frame frame, std::shared_ptr<CanTransceiverStubImpl> transceiverStubImpl)
{
    std::vector<uint8_t> buffer = std::vector<uint8_t>(frame.data, frame.data + frame.can_dlc);
    switch (frame.can_id)
    {
    case SPEED_SENSOR_CAN_ID:
    {
        auto processedRpmData = RpmProcess::process(buffer);
        transceiverStubImpl->setRpmAttribute(processedRpmData.rpm);
        transceiverStubImpl->setSpeedAttribute(processedRpmData.speed);
        break;
    }
    case SONAR_SENSOR_CAN_ID:
    {
        auto processedSonarData = SonarProcess::process(buffer);
        transceiverStubImpl->setDistancesAttribute(processedSonarData);
        break;
    }
    default:
        std::cerr << "Unknown CAN message ID: " << frame.can_id << std::endl;
        break;
    }
}

int main(void)
{
    // Create and start CAN receivers for two interfaces.
    CanReceiver             can0("can0");
    CanReceiver             can1("can1");

    auto transceiverStubImpl = std::make_shared<CanTransceiverStubImpl>();

    transceiverStubImpl->init();
    // Start receiving data.
    can0.start();
    can1.start();
    const double wheel_radius = 0.065;
    const double disk_radius  = 0.025;

    // Example data receive and process loop.
    // shouldContinue() defines the condition to keep running.
    while (true)
    {
        // auto rpmData    = can0.getReceivedData();
        // printData("rpmData in main", rpmData);
        // auto sonarData  = can1.getReceivedData();
        // printData("sonarData in main", sonarData);

        struct can_frame frame0 = can0.getReceivedData();
        struct can_frame frame1 = can1.getReceivedData();

        processData(frame0, transceiverStubImpl);
        processData(frame1, transceiverStubImpl);

        // // Process each set of data.
        // auto processedRpmData   = RpmProcess::process(rpmData);
        // auto processedSonarData = SonarProcess::process(sonarData);

        // // Register processed data.
        // transceiverStubImpl->setRpmAttribute(processedRpmData.rpm);
        // transceiverStubImpl->setSpeedAttribute(processedRpmData.speed);

        // transceiverStubImpl->setDistancesAttribute(processedSonarData);

        // std::cout << "Filtered RPM: " << processedRpmData.rpm << ", Speed: " << processedRpmData.speed << std::endl;
        // std::cout << "Front Sensor Left: " << processedSonarData.getSensorfrontleft()
        //   << ", Front Sensor Middle: " << processedSonarData.getSensorfrontmiddle()
        //   << ", Front Sensor Right: " << processedSonarData.getSensorfrontright() << std::endl;
    }

    // Stop receiving data.
    can0.stop();
    can1.stop();

    return 0;
}
