package inet.examples.hitachi_test;

import inet.networks.base.TsnNetworkBase;
import inet.networks.base.WiredNetworkBase;
import inet.node.tsn.TsnSwitch;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnClock;
import inet.node.inet.StandardHost;
import inet.node.ethernet.EthernetSwitch;
import inet.node.ethernet.Eth100M;

network TwoHosts2 extends TsnNetworkBase
{
    submodules:
        hostA: TsnDevice {
            parameters:
                @display("p=298,77");
        }
        hostB: TsnDevice {
            parameters:
                @display("p=493,77");
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
            @display("p=226,170");
        }
    connections:
        hostA.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
        hostB.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
        hostC.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
        gPTPclock.ethg++ <--> Eth100M <--> tsnSwitch.ethg++;
}

network SimpleSubstationSetupEthernet extends TsnNetworkBase
{
    submodules:
        IDE_A: TsnDevice {
            parameters:
                @display("p=298,77");
        }
        IDE_B: TsnDevice {
            parameters:
                @display("p=493,77");
        }
        MU: TsnDevice {
            parameters:
                @display("p=391,272");
        }
        switch: TsnSwitch {
            parameters:
                @display("p=391,169");
        }
    connections:
        IDE_A.ethg++ <--> Eth100M <--> switch.ethg++;
        IDE_B.ethg++ <--> Eth100M <--> switch.ethg++;
        MU.ethg++ <--> Eth100M <--> switch.ethg++;
}

network MultiCast extends WiredNetworkBase
{
    submodules:
        IDE_A: StandardHost {
            parameters:
                @display("p=298,77");
        }
        IDE_B: StandardHost {
            parameters:
                @display("p=391,77");
        }
        IDE_C: StandardHost {
            parameters:
                @display("p=501,77");
        }
        MU: StandardHost {
            parameters:
                @display("p=391,272");
        }
        switch: EthernetSwitch {
            parameters:
                @display("p=391,169");
        }
    connections:
        IDE_A.ethg++ <--> Eth100M <--> switch.ethg++;
        IDE_B.ethg++ <--> Eth100M <--> switch.ethg++;
        IDE_C.ethg++ <--> Eth100M <--> switch.ethg++;
        MU.ethg++ <--> Eth100M <--> switch.ethg++;
}
