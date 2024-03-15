//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#ifndef __INET_ACTIVEPACKETSOURCE_H
#define __INET_ACTIVEPACKETSOURCE_H

#include "inet/common/clock/ClockUserModuleMixin.h"
#include "inet/queueing/base/ActivePacketSourceBase.h"
#include <numeric> // For std::accumulate

namespace inet {

extern template class ClockUserModuleMixin<queueing::ActivePacketSourceBase>;

namespace queueing {

class INET_API ActivePacketSource : public ClockUserModuleMixin<ActivePacketSourceBase>
{
  protected:
    bool initialProductionOffsetScheduled = false;
    clocktime_t initialProductionOffset;
    cPar *productionIntervalParameter = nullptr;
    ClockEvent *productionTimer = nullptr;
    bool scheduleForAbsoluteTime = false;
    bool useGoose = false;
    int numberOfGooseEventsPerSec;
    clocktime_t nextPacketDelay;
    double gooseCopiesSent = 0;
    double gooseCopyDelay = 0.00025;
    clocktime_t pastEventTime = 0;
    std::vector<clocktime_t> randomTimes; // Member variable to store the sorted random times
    int oldIndex = 0;
    clocktime_t currentCopyDelay = 0;
    std::vector<clocktime_t> gooseCopyPeriods;
    clocktime_t sumGoosePeriods;

    clocktime_t nextHeartbeatTime = 0;
    clocktime_t lastHeartbeatTime = 0;
    std::vector<clocktime_t> heartbeatTimes;
    bool shouldScheduleHeartbeat;

    bool previousWasHeartbeat = true;
    bool heartbeat = true;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;
    virtual void handleParameterChange(const char *name) override;

    virtual void scheduleProductionTimer(clocktime_t delay);
    virtual void scheduleProductionTimerAndProducePacket();
    virtual void producePacket();

  public:
    virtual ~ActivePacketSource() { cancelAndDeleteClockEvent(productionTimer); }

    virtual void handleCanPushPacketChanged(cGate *gate) override;
    virtual void handlePushPacketProcessed(Packet *packet, cGate *gate, bool successful) override;
};

} // namespace queueing
} // namespace inet

#endif

