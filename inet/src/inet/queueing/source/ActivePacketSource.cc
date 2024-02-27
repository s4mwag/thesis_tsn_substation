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
double gooseCopiesSent = 0;
double gooseCopyDelay = 0.025;
bool firstPacket = true;
double firstScheduledTime;
double pastRandomScheduledTime = 0;
// Add a member variable to store the sorted random times
std::vector<double> randomTimes;
int oldIndex = 0;


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
            //firstScheduledTime = randomTimes.front();
            //randomTimes.erase(unique(randomTimes.begin(), randomTimes.end()), randomTimes.end());
        }
        scheduleClockEventAt(randomTimes.front(), productionTimer);
        pastRandomScheduledTime = randomTimes.front();
        randomTimes.erase(randomTimes.begin()); // Remove the scheduled time
        EV_INFO << "First event scheduled: " << gooseCopiesSent << EV_ENDL;
        updateDisplayString();
    }
    else if (stage == INITSTAGE_QUEUEING) {
        //checkPacketOperationSupport(outputGate);
        //if (!productionTimer->isScheduled() && useGoose == true)
        //    scheduleProductionTimerAndProducePacket();
        if (!randomTimes.empty() && useGoose == true) {
            //scheduleProductionTimer(randomTimes.front());
            //scheduleProductionTimerAndProducePacket();

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
    if (productionTimer->isScheduled()) {
        cancelEvent(productionTimer);
    }
    if(!randomTimes.empty()){
        double nextRandomTime = randomTimes.front(); // Get the next scheduled absolute time
        double timeSinceLastEvent = nextRandomTime - pastRandomScheduledTime; // Calculate time since last event

        if (scheduleForAbsoluteTime) {
            if (gooseCopiesSent < 3) {
                double currentCopyDelay = gooseCopyDelay * (std::pow(2, gooseCopiesSent));
                if (timeSinceLastEvent > currentCopyDelay) {
                    if(gooseCopiesSent > 1){
                        double copyScheduledTime = pastRandomScheduledTime + currentCopyDelay;
                        scheduleClockEventAt(copyScheduledTime, productionTimer);
                        gooseCopiesSent++;
                        EV_INFO << "Copy scheduled at: " << copyScheduledTime << " with index " << gooseCopiesSent << EV_ENDL;
                    }
                    else{
                        double copyScheduledTime = pastRandomScheduledTime + currentCopyDelay;
                        scheduleClockEventAt(copyScheduledTime, productionTimer);
                        gooseCopiesSent++;
                        EV_INFO << "Copy scheduled at: " << copyScheduledTime << " with index " << gooseCopiesSent << EV_ENDL;
                    }
                    // Schedule a copy if the time since the last event is larger than the current copy delay

                } else {
                    // Schedule next event and reset gooseCopiesSent if the time since the last event is not sufficient for another copy
                    scheduleClockEventAt(nextRandomTime, productionTimer);
                    pastRandomScheduledTime = nextRandomTime;
                    randomTimes.erase(randomTimes.begin()); // Remove the scheduled time
                    gooseCopiesSent = 0;
                    EV_INFO << "Event scheduled at: " << nextRandomTime << EV_ENDL;
                }
            } else {
                // Directly schedule next event if maximum copies have been sent
                scheduleClockEventAt(nextRandomTime, productionTimer);
                pastRandomScheduledTime = nextRandomTime;
                randomTimes.erase(randomTimes.begin()); // Remove the scheduled time
                gooseCopiesSent = 0;
                EV_INFO << "Event scheduled at: " << nextRandomTime << " after max copies" << EV_ENDL;
            }
        }
    }
}

            /*
            else{
                if((nextRandomTime > currentCopyDelay) && (gooseCopiesSent < 4)){
                    scheduleClockEventAfter(currentCopyDelay, productionTimer); // Schedule after the difference to now
                    gooseCopiesSent++;

                }
                else{
                    scheduleClockEventAfter(nextRandomTime, productionTimer);
                    gooseCopiesSent = 1;
                    randomTimes.erase(randomTimes.begin()); // Remove the scheduled time

                }
            }
            */

        /*
        else{
            if (scheduleForAbsoluteTime)
                scheduleClockEventAt(randomTimes.front(), productionTimer);
            //else
            //    scheduleClockEventAfter((randomTimes.front() - simTime().dbl()), productionTimer);

            pastRandomScheduledTime = randomTimes.front();
            randomTimes.erase(randomTimes.begin());
            gooseCopiesSent = 0;
            firstPacket = false;
        }
        */

    //}
    /*
    else{
        if (scheduleForAbsoluteTime)
            scheduleClockEventAt(getClockTime() + delay, productionTimer);
        else
            scheduleClockEventAfter(delay, productionTimer);
    }
    */
//}

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
    if(oldIndex == 0){
        EV_INFO << simTime().dbl() << " GOOSE Packet Produced" << EV_FIELD(packet) << EV_ENDL;
    }
    else{
        EV_INFO << simTime().dbl() << " GOOSE Copy Produced " << (oldIndex) << EV_FIELD(packet) << EV_ENDL;
    }
    oldIndex = gooseCopiesSent;
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

