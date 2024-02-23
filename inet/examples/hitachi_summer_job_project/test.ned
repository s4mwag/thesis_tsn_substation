package inet.examples.hitachi_summer_job_project;

import inet.common.scenario.ScenarioManager;
import inet.networks.base.TsnNetworkBase;
import inet.node.contract.IEthernetNetworkNode;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.EthernetLink;
import inet.node.tsn.TsnClock;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnSwitch;

network Ethernet extends TsnNetworkBase
{
    @display("bgb=1313,865.8");
    submodules:
		node13_line1_IED: TsnDevice {
            @display("p=500,100");
        }
        node14_BFP_IED: TsnDevice {
            @display("p=750,100");
        }
        node15_line2_IED: TsnDevice {
            @display("p=1000,100");
        }
        MC: TsnClock {
            @display("p=350,300");
        }
        switch1: TsnSwitch {
            @display("p=500,300");
        }
        switch2: TsnSwitch {
            @display("p=1000,300");
        }
        node1: TsnDevice {
            @display("p=350,500");
        }
        node2: TsnDevice {
            @display("p=473.19998,499.19998");
        }
        node3: TsnDevice {
            @display("p=646.1,499.19998");
        }
        node4: TsnDevice {
            @display("p=367.9,600");
        }
        node5: TsnDevice {
            @display("p=516.1,599.3");
        }
        node6: TsnDevice {
            @display("p=646.1,599.3");
        }
        node7: TsnDevice {
            @display("p=850.2,495.3");
        }
        node8: TsnDevice {
            @display("p=977.6,495.3");
        }
        node9: TsnDevice {
            @display("p=1170,495.3");
        }
        node10: TsnDevice {
            @display("p=868.39996,594.1");
        }
        node11: TsnDevice {
            @display("p=1020.5,594.1");
        }
        node12: TsnDevice {
            @display("p=1171.2999,594.1");
        }
    connections:
        MC.ethg++ <--> Eth100M <--> switch1.ethg++;
        node13_line1_IED.ethg++ <--> Eth100M <--> switch1.ethg++;
        node14_BFP_IED.ethg++ <--> Eth100M <--> switch1.ethg++;
        node15_line2_IED.ethg++ <--> Eth100M <--> switch2.ethg++;
        switch1.ethg++ <--> Eth100M <--> node1.ethg++;
        switch1.ethg++ <--> Eth100M <--> node2.ethg++;
        switch1.ethg++ <--> Eth100M <--> node3.ethg++;
        switch1.ethg++ <--> Eth100M <--> node4.ethg++;
        switch1.ethg++ <--> Eth100M <--> node5.ethg++;
        switch1.ethg++ <--> Eth100M <--> node6.ethg++;
        switch2.ethg++ <--> Eth100M <--> node7.ethg++;
        switch2.ethg++ <--> Eth100M <--> node8.ethg++;
        switch2.ethg++ <--> Eth100M <--> node9.ethg++;
        switch2.ethg++ <--> Eth100M <--> node10.ethg++;
        switch2.ethg++ <--> Eth100M <--> node11.ethg++;
        switch2.ethg++ <--> Eth100M <--> node12.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch2.ethg++;
}

network CreditBased extends TsnNetworkBase
{
    @display("bgb=1313,865.8");
    submodules:
		node13_line1_IED: TsnDevice {
            @display("p=500,100");
        }
        node14_BFP_IED: TsnDevice {
            @display("p=750,100");
        }
        MC: TsnClock {
            @display("p=350,300");
        }
        switch1: TsnSwitch {
            @display("p=500,300");
        }
        node1: TsnDevice {
            @display("p=350,500");
        }
    connections:
        MC.ethg++ <--> Eth100M <--> switch1.ethg++;
        node13_line1_IED.ethg++ <--> Eth100M <--> switch1.ethg++;
        node14_BFP_IED.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch1.ethg++ <--> Eth100M <--> node1.ethg++;
}
