# include "CanReceiver.hpp"
# include "RpmProcess.hpp"
# include "SonarProcess.hpp"
# include "CanTransceiverStubImpl.hpp"

int main(void)
{
    // Create and start CAN receivers for two interfaces.
    CanReceiver             can0("can0");
    CanReceiver             can1("can1");
    CanTransceiverStubImpl  transceiverStub;

    // Start receiving data.
    can0.start();
    can1.start();
    const double wheel_radius = 0.065;
    const double disk_radius  = 0.025;

    // Example data receive and process loop.
    // shouldContinue() defines the condition to keep running.
    while (true)
    {
        auto rpmData    = can0.getReceivedData();
        auto sonarData  = can1.getReceivedData();

        // Process each set of data.
        auto processedRpmData   = RpmProcess::process(rpmData);
        auto processedSonarData = SonarProcess::process(sonarData);

        // Register processed data.
        transceiverStub.setRpmAttribute(processedRpmData.rpm);
        transceiverStub.setSpeedAttribute(processedRpmData.speed);

        transceiverStub.setDistancesAttribute(processedSonarData);

        // Implement appropriate delay or logic for next data reception.
    }

    // Stop receiving data.
    can0.stop();
    can1.stop();

    return 0;
}
