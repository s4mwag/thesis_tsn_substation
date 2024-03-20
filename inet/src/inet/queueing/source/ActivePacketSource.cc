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
            gooseCopyPeriods.reserve(4);
            gooseCopyPeriods = {0.001, 0.001, 0.1, 0.25};

            heartbeatTimes.reserve(25);
            clocktime_t heartbeatInterval = 0;
            for(int i = 0; i < 21; i++){
                heartbeatTimes.push_back(heartbeatInterval);
                heartbeatInterval = heartbeatInterval + 0.5;
            }


            std::sort(randomTimes.begin(), randomTimes.end()); // Sort the times
            //scheduleClockEventAt(randomTimes.front(), productionTimer); // Schedule first event
            //pastEventTime = randomTimes.front();
            //randomTimes.erase(randomTimes.begin()); // Remove the scheduled event

            //scheduleProductionTimer(0);
            scheduleClockEventAt(heartbeatTimes.front(), productionTimer);
            lastHeartbeatTime = nextHeartbeatTime;
            heartbeatTimes.erase(heartbeatTimes.begin());

            //scheduleClockEventAt(randomTimes.front(), productionTimer); // Schedule first event
            //pastEventTime = randomTimes.front();
            //randomTimes.erase(randomTimes.begin()); // Remove the scheduled event

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
    clocktime_t nextHeartbeatTime = heartbeatTimes.front(); // Schedule heartbeats every 0.5 seconds

    bool shouldScheduleHeartbeat = true;


    // randomTimes will only be populated if useGoose is set to "true"
    if(!randomTimes.empty()){
        clocktime_t nextEventTime = randomTimes.front(); // Get the next scheduled absolute time
        clocktime_t timeBetweenEvents = nextEventTime - pastEventTime; // Calculate time since last event

        if (!randomTimes.empty()) {

            // For debugging
            //EV_INFO << "nextEventTime: " << nextEventTime << " nextHeartbeatTime: " << nextHeartbeatTime << EV_ENDL;

            if (nextEventTime <= nextHeartbeatTime) {
                // Next event is scheduled before the next heartbeat

                // 0.352 is the sum the vector gooseCopyPeriods
                if((nextEventTime + 0.352) >= nextHeartbeatTime){
                    heartbeatTimes.erase(heartbeatTimes.begin());
                }
                EV_INFO << "Heartbeat disabled" << EV_ENDL;
                shouldScheduleHeartbeat = false;
            }
            else{
                EV_INFO << "Heartbeat enabled" << EV_ENDL;
                shouldScheduleHeartbeat = true;
            }
        }

        if (scheduleForAbsoluteTime) {

            // Do not create a copy if copy limit is reached or if previous message was heartbeat
            if (gooseCopiesSent < 4 && !previousWasHeartbeat) {

                currentCopyDelay += gooseCopyPeriods[gooseCopiesSent];

                if (timeBetweenEvents > currentCopyDelay) {
                    // Schedule a copy if the time since the last event is larger than the current copy delay
                    clocktime_t copyScheduledTime = pastEventTime + currentCopyDelay;
                    EV_INFO << "Copy scheduled at: " << copyScheduledTime << " with delay from event of: " << currentCopyDelay << EV_ENDL;
                    gooseCopiesSent++;
                    scheduleClockEventAt(copyScheduledTime, productionTimer);
                    previousWasHeartbeat = false;

                }
                else {
                    // Schedule next event and reset gooseCopiesSent if the time to the next event is not sufficient for another copy
                    scheduleClockEventAt(nextEventTime, productionTimer);
                    pastEventTime = nextEventTime;
                    randomTimes.erase(randomTimes.begin()); // Remove the scheduled time
                    gooseCopiesSent = 0; //Reset the variables for calculating the use of copied messages
                    currentCopyDelay = 0;
                    EV_INFO << "Event scheduled at: " << nextEventTime << EV_ENDL;
                    previousWasHeartbeat = false;
                }
            }
            else if (shouldScheduleHeartbeat){
                scheduleClockEventAt(nextHeartbeatTime, productionTimer);
                lastHeartbeatTime = nextHeartbeatTime;
                heartbeatTimes.erase(heartbeatTimes.begin());
                EV_INFO << "Heartbeat scheduled at: " << nextHeartbeatTime << EV_ENDL;
                previousWasHeartbeat = true;
            }

            else {
                // Directly schedule next event if maximum copies have been sent
                scheduleClockEventAt(nextEventTime, productionTimer);
                pastEventTime = nextEventTime;
                randomTimes.erase(randomTimes.begin()); // Remove the scheduled event
                gooseCopiesSent = 0; //Reset the variables for calculating the use of copied messages
                currentCopyDelay = 0;
                EV_INFO << "Event scheduled at: " << nextEventTime << EV_ENDL;
                previousWasHeartbeat = false;
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
        if(heartbeat){
            EV_INFO << simTime().dbl() << " GOOSE Heartbeat Packet Produced" << EV_FIELD(packet) << EV_ENDL;
        }
        else if(oldIndex == 0){
            EV_INFO << simTime().dbl() << " GOOSE Event Packet Produced" << EV_FIELD(packet) << EV_ENDL;
        }
        else{
            EV_INFO << simTime().dbl() << " GOOSE Copy Packet Produced " << (oldIndex) << EV_FIELD(packet) << EV_ENDL;
        }
        heartbeat = previousWasHeartbeat;
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

