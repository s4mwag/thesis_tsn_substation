//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/queueing/source/ActivePacketSource.h"
#include <random>
#include <vector>
#include <algorithm>

namespace inet {
namespace queueing {

Define_Module(ActivePacketSource);

clocktime_t nextPacketDelay;
bool firstGooseMessage = true;
int goosePacketSent = 0;

// Add a member variable to store the sorted random times
std::vector<double> randomTimes;


void ActivePacketSource::initialize(int stage)
{

    ClockUserModuleMixin::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        initialProductionOffset = par("initialProductionOffset");
        productionIntervalParameter = &par("productionInterval");
        productionTimer = new ClockEvent("ProductionTimer");
        scheduleForAbsoluteTime = par("scheduleForAbsoluteTime");
        useGoose = par("useGoose");
        randomTimes.reserve(100); // Reserve space for 100 elements
        if(useGoose == true){
            for (int i = 0; i < 100; ++i) { // Generate 100 float values between 0 - 10
                double randomTime = uniform(0.0, 10.0);
                randomTimes.push_back(randomTime);
            }
            std::sort(randomTimes.begin(), randomTimes.end()); // Sort the times
        }
    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPacketOperationSupport(outputGate);
        if (!productionTimer->isScheduled())
            scheduleProductionTimerAndProducePacket();
        if (!randomTimes.empty() && useGoose == true) {
            scheduleProductionTimer(randomTimes.front());
        }
    }

}

void ActivePacketSource::handleMessage(cMessage *message)
{
    if (message == productionTimer) {
        if (consumer == nullptr || consumer->canPushSomePacket(outputGate->getPathEndGate())) {
            scheduleProductionTimer(productionIntervalParameter->doubleValue());
            producePacket();
        }
    }
    else
        throw cRuntimeError("Unknown message");
}

void ActivePacketSource::handleParameterChange(const char *name)
{
    if (!strcmp(name, "initialProductionOffset"))
        initialProductionOffset = par("initialProductionOffset");
}
/*
void ActivePacketSource::scheduleProductionTimer(clocktime_t delay)
{
    if(useGoose){

        int gooseEvent = rand() % 10;

        if (firstGooseMessage && gooseEvent < 2){
            nextPacketDelay = (delay / 100);
            firstGooseMessage = false;
            goosePacketSent = 1;
        }
        else if(goosePacketSent > 0){
            nextPacketDelay = nextPacketDelay * 2;
            goosePacketSent += 1;

            if (goosePacketSent >= 3){
                firstGooseMessage = true;
                goosePacketSent = 0;
            }
        }
        else{
            nextPacketDelay = delay;
        }

        if (scheduleForAbsoluteTime)
            scheduleClockEventAt(getClockTime() + nextPacketDelay, productionTimer);
        else
            scheduleClockEventAfter(nextPacketDelay, productionTimer);
    }
    else{
        if (scheduleForAbsoluteTime)
            scheduleClockEventAt(getClockTime() + delay, productionTimer);
        else
            scheduleClockEventAfter(delay, productionTimer);
    }
}
*/
void ActivePacketSource::scheduleProductionTimer(clocktime_t delay)
{
    int totalPackets = 100;
    clocktime_t totalDuration = 10;
    // Calculate a random delay for the next packet
    //std::default_random_engine generator(std::random_device{}()); // Random number generator
    //std::uniform_real_distribution<double> distribution(0.0, totalDuration); // Uniform distribution between 0 and totalDuration (10 seconds)
    nextPacketDelay = uniform(0.0, 10.0);
    // Generate a random delay for the next packet
    //nextPacketDelay = distribution(generator);

    if (productionTimer->isScheduled()) {
        cancelEvent(productionTimer);
    }
    if (!randomTimes.empty()) {
        double nextTime = randomTimes.front(); // Get the next scheduled time
        randomTimes.erase(randomTimes.begin());
        if (scheduleForAbsoluteTime)
            scheduleClockEventAt(nextTime, productionTimer);
        else
            scheduleClockEventAfter(nextTime - simTime().dbl(), productionTimer); // Schedule after the difference to now

        //randomTimes.erase(randomTimes.begin()); // Remove the scheduled time
    }
    else{
        if (scheduleForAbsoluteTime)
            scheduleClockEventAt(getClockTime() + delay, productionTimer);
        else
            scheduleClockEventAfter(delay, productionTimer);
    }
}

void ActivePacketSource::scheduleProductionTimerAndProducePacket()
{
    if (!initialProductionOffsetScheduled && initialProductionOffset >= CLOCKTIME_ZERO) {
        scheduleProductionTimer(initialProductionOffset);
        initialProductionOffsetScheduled = true;
    }
    else if (consumer == nullptr || consumer->canPushSomePacket(outputGate->getPathEndGate())) {
        scheduleProductionTimer(productionIntervalParameter->doubleValue());
        producePacket();
    }
}
/*
void ActivePacketSource::scheduleProductionTimerAndProducePacket()
{
    simtime_t totalTime = 10.0;  // Total time period in seconds
    int numPackets = 100;  // Total number of packets to distribute
    simtime_t currentTime = simTime();

    // Cancel any previous scheduling of productionTimer
    cancelEvent(productionTimer);

    for (int i = 0; i < numPackets; ++i) {
        simtime_t packetDelay = uniform(0, totalTime);  // Generate random delay within the total time period
        scheduleAt(currentTime + packetDelay, productionTimer);  // Schedule packet production after the delay
        currentTime += packetDelay;  // Update current time
    }
}
*/
void ActivePacketSource::producePacket()
{
    auto packet = createPacket();
    EV_INFO << "Producing packet" << EV_FIELD(packet) << EV_ENDL;
    emit(packetPushedSignal, packet);
    pushOrSendPacket(packet, outputGate, consumer);
    updateDisplayString();
}

void ActivePacketSource::handleCanPushPacketChanged(cGate *gate)
{
    Enter_Method("handleCanPushPacketChanged");
    if (!productionTimer->isScheduled())
        scheduleProductionTimerAndProducePacket();
}

void ActivePacketSource::handlePushPacketProcessed(Packet *packet, cGate *gate, bool successful)
{
    Enter_Method("handlePushPacketProcessed");
}

} // namespace queueing
} // namespace inet

