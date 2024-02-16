package inet.showcases.tsn.andreas_test;

import inet.node.ethernet.EthernetHub;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import inet.node.ethernet.EthernetHost;
import inet.node.ethernet.EthernetSwitch;
import inet.node.inet.StandardHost2;
import inet.physicallayer.wired.common.WireJunction;
import ned.DatarateChannel;
import inet.node.tsn.TsnSwitch;


network TwoHosts2
{
    submodules:
        hostA: StandardHost2 {
            parameters:
                @display("p=100,100");
        }
        hostB: StandardHost2 {
            parameters:
                @display("p=300,100");
        }
        hostC: StandardHost2 {
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