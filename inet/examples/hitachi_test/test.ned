package inet.examples.hitachi_test;

import inet.networks.base.WiredNetworkBase;
import inet.node.ethernet.Eth100M;
import inet.node.inet.StandardHost;
import inet.node.ethernet.EthernetSwitch;


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
