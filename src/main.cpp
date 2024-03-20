#include "CanReceiver.hpp"
#include "RpmProcess.hpp"
#include "SonarProcess.hpp"
#include "CanTransceiverStubImpl.hpp"

int main() {
    // Create and start CAN receivers for two interfaces.
    CanReceiver canReceiverCan0("can0");
    CanReceiver canReceiverCan1("can1");
    CanTransceiverStubImpl transceiverStub;

    // Start receiving data.
    canReceiverCan0.start();
    canReceiverCan1.start();

    // Example data receive and process loop.
    while (shouldContinue()) { // shouldContinue() defines the condition to keep running.
        auto dataCan0 = canReceiverCan0.getReceivedData();
        auto dataCan1 = canReceiverCan1.getReceivedData();

        // Process each set of data.
        auto processedRpmData = RpmProcess::process(dataCan0, wheel_radius, disk_radius); // Assume wheel_radius and disk_radius are defined.
        auto processedSonarData = SonarProcess::process(dataCan1);

        // Register processed data.
        transceiverStub.registerRpmData(processedRpmData.rpm, processedRpmData.speed);
        transceiverStub.registerSonarData(processedSonarData.sensorFL, processedSonarData.sensorFM, processedSonarData.sensorFR);

        // Implement appropriate delay or logic for next data reception.
    }

    // Stop receiving data.
    canReceiverCan0.stop();
    canReceiverCan1.stop();

    return 0;
}
