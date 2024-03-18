#include "CanReceiver.hpp"

int main(){
    CanReceiver receiverCan0("can0");
    CanReceiver receiverCan1("can1");
    
    receiverCan0.run();
    receiverCan1.run();

    return 0
}