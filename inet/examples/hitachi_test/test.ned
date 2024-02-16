package inet.examples.hitachi_test;

import inet.node.ethernet.EthernetHub;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import inet.physicallayer.wired.common.WireJunction;
import ned.DatarateChannel;
import inet.node.tsn.TsnSwitch;
import inet.node.tsn.TsnDevice;


network TwoHosts2
{
    submodules:
        hostA: TsnDevice {
            parameters:
                @display("p=100,100");
        }
        hostB: TsnDevice {
            parameters:
                @display("p=300,100");
        }
        hostC: TsnDevice {
            parameters:
                @display("p=191,272");
        }
        tsnSwitch: TsnSwitch {
            parameters:
                @display("p=191,169");
        }
    connections:
        hostA.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
        hostB.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
        hostC.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
}