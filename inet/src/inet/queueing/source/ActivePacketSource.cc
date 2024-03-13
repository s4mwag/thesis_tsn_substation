//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/queueing/source/ActivePacketSource.h"


namespace inet {
namespace queueing {

Define_Module(ActivePacketSource);



void ActivePacketSource::initialize(int stage)
{

    ClockUserModuleMixin::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        initialProductionOffset = par("initialProductionOffset");
        productionIntervalParameter = &par("productionInterval");
        productionTimer = new ClockEvent("ProductionTimer");
        scheduleForAbsoluteTime = par("scheduleForAbsoluteTime");
        useGoose = par("useGoose");
        numberOfGooseEventsPerSec = par("numberOfGooseEventsPerSec");

        // Gather the "sim-time-limit" variable from the ini file
        cConfigOption simTimeConfig("sim-time-limit", true,cConfigOption::Type::CFG_DOUBLE, "s", "300", "");
        double maxSimTime = cSimulation::getActiveSimulation()->getEnvir()->getConfig()->getAsDouble(&simTimeConfig);

        if(useGoose == true){
            randomTimes.reserve(numberOfGooseEventsPerSec * 10); // Reserve space for the number of goose events

            // Generate 100 float values between 0 - "sim-time-limit"
            for (int j = 0, i = 1; j < maxSimTime; j++, i++){
                for(int k = 0; k < numberOfGooseEventsPerSec; k++){
                    clocktime_t randomTime = uniform(j, i);
                    randomTimes.push_back(randomTime);
                }
            }

            std::sort(randomTimes.begin(), randomTimes.end()); // Sort the times
            //scheduleClockEventAt(randomTimes.front(), productionTimer); // Schedule first event
            //pastEventTime = randomTimes.front();
            //randomTimes.erase(randomTimes.begin()); // Remove the scheduled event

            scheduleProductionTimer(0);
        }
    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPacketOperationSupport(outputGate);
        if (!productionTimer->isScheduled())
            scheduleProductionTimerAndProducePacket();
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

void ActivePacketSource::scheduleProductionTimer(clocktime_t delay)
{
    if (productionTimer->isScheduled()) {
        cancelEvent(productionTimer);
    }

    clocktime_t currentTime = getClockTime();
    clocktime_t nextHeartbeatTime = lastHeartbeatTime + 0.5; // Schedule heartbeats every 0.5 seconds

    bool shouldScheduleHeartbeat = true;


    // randomTimes will only be populated if useGoose is set to "true"
    if(!randomTimes.empty()){
        clocktime_t nextEventTime = randomTimes.front(); // Get the next scheduled absolute time
        clocktime_t timeBetweenEvents = nextEventTime - pastEventTime; // Calculate time since last event

        if (scheduleForAbsoluteTime) {

            if (!randomTimes.empty()) {
                clocktime_t nextEventTime = randomTimes.front();
                if (nextEventTime <= nextHeartbeatTime) {
                    // Next event is scheduled before the next heartbeat
                    shouldScheduleHeartbeat = false;
                }
            }

            if (gooseCopiesSent < 3) {

                currentCopyDelay += (gooseCopyDelay * (std::pow(2, gooseCopiesSent)));

                if (timeBetweenEvents > currentCopyDelay) {
                    // Schedule a copy if the time since the last event is larger than the current copy delay
                    clocktime_t copyScheduledTime = pastEventTime + currentCopyDelay;
                    gooseCopiesSent++;
                    scheduleClockEventAt(copyScheduledTime, productionTimer);
                    EV_INFO << "Copy scheduled at: " << copyScheduledTime << " with delay from event of: " << currentCopyDelay << EV_ENDL;
                }
                else {
                    // Schedule next event and reset gooseCopiesSent if the time to the next event is not sufficient for another copy
                    scheduleClockEventAt(nextEventTime, productionTimer);
                    pastEventTime = nextEventTime;
                    randomTimes.erase(randomTimes.begin()); // Remove the scheduled time
                    gooseCopiesSent = 0; //Reset the variables for calculating the use of copied messages
                    currentCopyDelay = 0;
                    EV_INFO << "Event scheduled at: " << nextEventTime << EV_ENDL;
                }
            }
            else if (nextEventTime <= nextHeartbeatTime && shouldScheduleHeartbeat) {
                scheduleClockEventAt(nextHeartbeatTime, productionTimer);
                lastHeartbeatTime = nextHeartbeatTime;
                EV_INFO << "Heartbeat scheduled at: " << nextHeartbeatTime << EV_ENDL;
            }

            else {
                // Directly schedule next event if maximum copies have been sent
                scheduleClockEventAt(nextEventTime, productionTimer);
                pastEventTime = nextEventTime;
                randomTimes.erase(randomTimes.begin()); // Remove the scheduled event
                gooseCopiesSent = 0; //Reset the variables for calculating the use of copied messages
                currentCopyDelay = 0;
                EV_INFO << "Event scheduled at: " << nextEventTime << " after max copies" << EV_ENDL;
            }
        }
    }
    // Scheduling normal periodic traffic
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

void ActivePacketSource::producePacket()
{
    auto packet = createPacket();
    if(useGoose){
        if(oldIndex == 0){
            EV_INFO << simTime().dbl() << " GOOSE Packet Produced" << EV_FIELD(packet) << EV_ENDL;
        }
        else{
            EV_INFO << simTime().dbl() << " GOOSE Copy Produced " << (oldIndex) << EV_FIELD(packet) << EV_ENDL;
        }
        oldIndex = gooseCopiesSent; //Used to keep track of the previous copied message index since it will be overwritten during scheduling
    }
    else
        EV_INFO << simTime().dbl() << " Packet Produced " << EV_FIELD(packet) << EV_ENDL;
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

