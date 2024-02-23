package inet.examples.hitachi_test;

import inet.networks.base.TsnNetworkBase;
import inet.node.tsn.TsnSwitch;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnClock;
import inet.node.inet.StandardHost;
import inet.node.ethernet.Eth100M;

network IntegrationExampleSmall extends TsnNetworkBase
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
        switch: TsnSwitch {
            parameters:
                @display("p=391,169");
        }
        gPTPclock: TsnClock {
            @display("p=226,170");
        }
    connections:
        gPTPclock.ethg++ <--> Eth100M <--> switch.ethg++;
        IDE_A.ethg++ <--> Eth100M <--> switch.ethg++;
        IDE_B.ethg++ <--> Eth100M <--> switch.ethg++;
        IDE_C.ethg++ <--> Eth100M <--> switch.ethg++;
        MU.ethg++ <--> Eth100M <--> switch.ethg++;
        
}

network MultiCast extends TsnNetworkBase
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
        switch: TsnSwitch {
            parameters:
                @display("p=391,169");
        }
    connections:
        IDE_A.ethg++ <--> Eth100M <--> switch.ethg++;
        IDE_B.ethg++ <--> Eth100M <--> switch.ethg++;
        IDE_C.ethg++ <--> Eth100M <--> switch.ethg++;
        MU.ethg++ <--> Eth100M <--> switch.ethg++;
}
