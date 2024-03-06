package inet.examples.hitachi_test;

import inet.networks.base.TsnNetworkBase2;
import inet.node.tsn.TsnSwitch;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnClock;
import inet.node.ethernet.Eth100M;
import inet.node.inet.StandardHost;

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

network ThreeBays extends TsnNetworkBase2
{
    @display("bgb=1200,800");
    submodules:
        IED_1_A: TsnDevice {
            parameters:
                @display("p=319,222");
        }
        IED_1_B: TsnDevice {
            parameters:
                @display("p=378,222");
        }
        IED_1_C: TsnDevice {
            parameters:
                @display("p=442,222");
        }
        IED_2_A: TsnDevice {
            parameters:
                @display("p=523,351");
        }
        IED_2_B: TsnDevice {
            parameters:
                @display("p=582,351");
        }
        IED_2_C: TsnDevice {
            parameters:
                @display("p=646,351");
        }
        IED_3_A: TsnDevice {
            parameters:
                @display("p=732,222");
        }
        IED_3_B: TsnDevice {
            parameters:
                @display("p=791,222");
        }
        IED_3_C: TsnDevice {
            parameters:
                @display("p=855,222");
        }
        MU_1: TsnDevice {
            parameters:
                @display("p=378,376");
        }
        MU_2: TsnDevice {
            parameters:
                @display("p=582,541");
        }
        MU_3: TsnDevice {
            parameters:
                @display("p=791,376");
        }
        switch_1: TsnSwitch {
            parameters:
                @display("p=378,293");
        }
        switch_2: TsnSwitch {
            parameters:
                @display("p=582,475");
        }
        switch_3: TsnSwitch {
            parameters:
                @display("p=791,293");
        }
        gPTPclock: TsnClock {
            @display("p=242,341");
        }
        log_server: StandardHost {
            @display("p=242,259");
        }
    connections:
        gPTPclock.ethg++ <--> Eth100M <--> switch_1.ethg++;
        IED_1_A.ethg++ <--> Eth100M <--> switch_1.ethg++;
        IED_1_B.ethg++ <--> Eth100M <--> switch_1.ethg++;
        IED_1_C.ethg++ <--> Eth100M <--> switch_1.ethg++;
        MU_1.ethg++ <--> Eth100M <--> switch_1.ethg++;
        IED_2_A.ethg++ <--> Eth100M <--> switch_2.ethg++;
        IED_2_B.ethg++ <--> Eth100M <--> switch_2.ethg++;
        IED_2_C.ethg++ <--> Eth100M <--> switch_2.ethg++;
        MU_2.ethg++ <--> Eth100M <--> switch_2.ethg++;
        IED_3_A.ethg++ <--> Eth100M <--> switch_3.ethg++;
        IED_3_B.ethg++ <--> Eth100M <--> switch_3.ethg++;
        IED_3_C.ethg++ <--> Eth100M <--> switch_3.ethg++;
        MU_3.ethg++ <--> Eth100M <--> switch_3.ethg++;
        switch_1.ethg++ <--> Eth100M <--> switch_2.ethg++;
        switch_1.ethg++ <--> Eth100M <--> switch_3.ethg++;
        switch_2.ethg++ <--> Eth100M <--> switch_3.ethg++;
        log_server.ethg++ <--> Eth100M <--> switch_1.ethg++;

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
