package inet.examples.hitachi_test;

import inet.networks.base.TsnNetworkBase;
import inet.node.contract.IEthernetNetworkNode;
import inet.node.ethernet.EthernetLink;


import inet.node.ethernet.Eth100M;

import ned.DatarateChannel;
import inet.node.tsn.TsnSwitch;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnClock;


network TwoHosts2 extends TsnNetworkBase
{
    submodules:
        hostA: TsnDevice {
            parameters:
                @display("p=300,100");
        }
        hostB: TsnDevice {
            parameters:
                @display("p=500,100");
        }
        hostC: TsnDevice {
            parameters:
                @display("p=391,272");
        }
        tsnSwitch: TsnSwitch {
            parameters:
                @display("p=391,169");
        }
        gPTPclock: TsnClock {
            @display("p=191,37");
        }
    connections:
        hostA.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
        hostB.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
        hostC.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
        gPTPclock.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
}
