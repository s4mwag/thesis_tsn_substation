package inet.examples.hitachi_test;

import inet.networks.base.TsnNetworkBase2;
import inet.node.tsn.TsnSwitch;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnClock;
import inet.node.ethernet.Eth100M;

network IntegrationExampleSmall extends TsnNetworkBase2
{
    submodules:
        IED_A: TsnDevice {
            parameters:
                @display("p=298,77");
        }
        IED_B: TsnDevice {
            parameters:
                @display("p=391,77");
        }
        IED_C: TsnDevice {
            parameters:
                @display("p=501,77");
        }
        MU: TsnDevice {
            parameters:
                @display("p=391,272");
        }
        switch: TsnSwitch {
            parameters:
                @display("p=391,169");
        }
        gPTPclock: TsnClock {
            @display("p=226,170");
        }
    connections:
        gPTPclock.ethg++ <--> Eth100M <--> switch.ethg++;
        IED_A.ethg++ <--> Eth100M <--> switch.ethg++;
        IED_B.ethg++ <--> Eth100M <--> switch.ethg++;
        IED_C.ethg++ <--> Eth100M <--> switch.ethg++;
        MU.ethg++ <--> Eth100M <--> switch.ethg++;
        
}

network MultiCast extends TsnNetworkBase2
{
    submodules:
        IED_A: TsnDevice {
            parameters:
                @display("p=298,77");
        }
        IED_B: TsnDevice {
            parameters:
                @display("p=391,77");
        }
        IED_C: TsnDevice {
            parameters:
                @display("p=501,77");
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
        IED_A.ethg++ <--> Eth100M <--> switch.ethg++;
        IED_B.ethg++ <--> Eth100M <--> switch.ethg++;
        IED_C.ethg++ <--> Eth100M <--> switch.ethg++;
        MU.ethg++ <--> Eth100M <--> switch.ethg++;
}
