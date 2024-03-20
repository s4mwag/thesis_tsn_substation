package inet.examples.hitachi_test;

import inet.networks.base.TsnNetworkBase2;
import inet.node.tsn.TsnSwitch;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnClock;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import inet.node.inet.StandardHost;

network ThreeBaysOverProvisioned extends TsnNetworkBase2
{
    @display("bgb=1200,800");
    submodules:
        IED_1_A: TsnDevice {
            parameters:
                @display("p=502,229");
        }
        IED_1_B: TsnDevice {
            parameters:
                @display("p=561,229");
        }
        IED_1_C: TsnDevice {
            parameters:
                @display("p=625,229");
        }
        IED_2_A: TsnDevice {
            parameters:
                @display("p=706,358");
        }
        IED_2_B: TsnDevice {
            parameters:
                @display("p=765,358");
        }
        IED_2_C: TsnDevice {
            parameters:
                @display("p=829,358");
        }
        IED_3_A: TsnDevice {
            parameters:
                @display("p=915,229");
        }
        IED_3_B: TsnDevice {
            parameters:
                @display("p=974,229");
        }
        IED_3_C: TsnDevice {
            parameters:
                @display("p=1038,229");
        }
        MU_1_A: TsnDevice {
            parameters:
                @display("p=517.6923,382.3077");
        }
        MU_1_B: TsnDevice {
            parameters:
                @display("p=583.8462,382.3077");   
        }
        MU_2_A: TsnDevice {
            parameters:
                @display("p=724.6154,547.6923");
        }
        MU_2_B: TsnDevice {
            parameters:
                @display("p=794.6154,547.6923");
        }
        MU_3_A: TsnDevice {
            parameters:
                @display("p=944.6154,382.3077");
        }
        MU_3_B: TsnDevice {
            parameters:
                @display("p=1003.8462,382.3077");
        }
        switch_1: TsnSwitch {
            parameters:
                @display("p=561,300");
        }
        switch_2: TsnSwitch {
            parameters:
                @display("p=765,482");
        }
        switch_3: TsnSwitch {
            parameters:
                @display("p=974,300");
        }
        gPTPclock: TsnClock {
            @display("p=407.69232,386.9231;i=device/clock");
        }
        log_server: StandardHost {
            @display("p=328,200");
        }
        internet_host_1: StandardHost {
            @display("p=328,292");
        }
        internet_host_2: StandardHost {
            @display("p=322,360");
        }
        internet_host_3: StandardHost {
            @display("p=1112,278");
        }
        internet_host_4: StandardHost {
            @display("p=1112,358");
        }
    connections:
        gPTPclock.ethg++ <--> Eth100M <--> switch_1.ethg++;
        IED_1_A.ethg++ <--> Eth100M <--> switch_1.ethg++;
        IED_1_B.ethg++ <--> Eth100M <--> switch_1.ethg++;
        IED_1_C.ethg++ <--> Eth100M <--> switch_1.ethg++;
        MU_1_A.ethg++ <--> Eth100M <--> switch_1.ethg++;
        MU_1_B.ethg++ <--> Eth100M <--> switch_1.ethg++;
        IED_2_A.ethg++ <--> Eth100M <--> switch_2.ethg++;
        IED_2_B.ethg++ <--> Eth100M <--> switch_2.ethg++;
        IED_2_C.ethg++ <--> Eth100M <--> switch_2.ethg++;
        MU_2_A.ethg++ <--> Eth100M <--> switch_2.ethg++;
        MU_2_B.ethg++ <--> Eth100M <--> switch_2.ethg++;
        IED_3_A.ethg++ <--> Eth100M <--> switch_3.ethg++;
        IED_3_B.ethg++ <--> Eth100M <--> switch_3.ethg++;
        IED_3_C.ethg++ <--> Eth100M <--> switch_3.ethg++;
        MU_3_A.ethg++ <--> Eth100M <--> switch_3.ethg++;
        MU_3_B.ethg++ <--> Eth100M <--> switch_3.ethg++;
        switch_1.ethg++ <--> Eth100M <--> switch_2.ethg++;
        switch_1.ethg++ <--> Eth100M <--> switch_3.ethg++;
        switch_2.ethg++ <--> Eth100M <--> switch_3.ethg++;
        log_server.ethg++ <--> Eth100M <--> switch_1.ethg++;
        internet_host_1.ethg++ <--> Eth100M <--> switch_1.ethg++;
        internet_host_2.ethg++ <--> Eth100M <--> switch_1.ethg++;
        internet_host_3.ethg++ <--> Eth100M <--> switch_3.ethg++;
        internet_host_4.ethg++ <--> Eth100M <--> switch_3.ethg++;
}

network ThreeBaysUnderProvisioned extends TsnNetworkBase2
{
    @display("bgb=1200,800");
    submodules:
        IED_1_A: TsnDevice {
            parameters:
                @display("p=502,229");
        }
        IED_1_B: TsnDevice {
            parameters:
                @display("p=561,229");
        }
        IED_1_C: TsnDevice {
            parameters:
                @display("p=625,229");
        }
        IED_2_A: TsnDevice {
            parameters:
                @display("p=706,358");
        }
        IED_2_B: TsnDevice {
            parameters:
                @display("p=765,358");
        }
        IED_2_C: TsnDevice {
            parameters:
                @display("p=829,358");
        }
        IED_3_A: TsnDevice {
            parameters:
                @display("p=915,229");
        }
        IED_3_B: TsnDevice {
            parameters:
                @display("p=974,229");
        }
        IED_3_C: TsnDevice {
            parameters:
                @display("p=1038,229");
        }
        MU_1_A: TsnDevice {
            parameters:
                @display("p=517.6923,382.3077");
        }
        MU_1_B: TsnDevice {
            parameters:
                @display("p=583.8462,382.3077");   
        }
        MU_2_A: TsnDevice {
            parameters:
                @display("p=724.6154,547.6923");
        }
        MU_2_B: TsnDevice {
            parameters:
                @display("p=794.6154,547.6923");
        }
        MU_3_A: TsnDevice {
            parameters:
                @display("p=944.6154,382.3077");
        }
        MU_3_B: TsnDevice {
            parameters:
                @display("p=1003.8462,382.3077");
        }
        switch_1: TsnSwitch {
            parameters:
                @display("p=561,300");
        }
        switch_2: TsnSwitch {
            parameters:
                @display("p=765,482");
        }
        switch_3: TsnSwitch {
            parameters:
                @display("p=974,300");
        }
        gPTPclock: TsnClock {
            @display("p=407.69232,386.9231;i=device/clock");
        }
        log_server: StandardHost {
            @display("p=328,200");
        }
        internet_host_1: StandardHost {
            @display("p=328,292");
        }
        internet_host_2: StandardHost {
            @display("p=322,360");
        }
        internet_host_3: StandardHost {
            @display("p=1112,278");
        }
        internet_host_4: StandardHost {
            @display("p=1112,358");
        }
    connections:
        gPTPclock.ethg++ <--> Eth10M <--> switch_1.ethg++;
        IED_1_A.ethg++ <--> Eth10M <--> switch_1.ethg++;
        IED_1_B.ethg++ <--> Eth10M <--> switch_1.ethg++;
        IED_1_C.ethg++ <--> Eth10M <--> switch_1.ethg++;
        MU_1_A.ethg++ <--> Eth10M <--> switch_1.ethg++;
        MU_1_B.ethg++ <--> Eth10M <--> switch_1.ethg++;
        IED_2_A.ethg++ <--> Eth10M <--> switch_2.ethg++;
        IED_2_B.ethg++ <--> Eth10M <--> switch_2.ethg++;
        IED_2_C.ethg++ <--> Eth10M <--> switch_2.ethg++;
        MU_2_A.ethg++ <--> Eth10M <--> switch_2.ethg++;
        MU_2_B.ethg++ <--> Eth10M <--> switch_2.ethg++;
        IED_3_A.ethg++ <--> Eth10M <--> switch_3.ethg++;
        IED_3_B.ethg++ <--> Eth10M <--> switch_3.ethg++;
        IED_3_C.ethg++ <--> Eth10M <--> switch_3.ethg++;
        MU_3_A.ethg++ <--> Eth10M <--> switch_3.ethg++;
        MU_3_B.ethg++ <--> Eth10M <--> switch_3.ethg++;
        switch_1.ethg++ <--> Eth10M <--> switch_2.ethg++;
        switch_1.ethg++ <--> Eth10M <--> switch_3.ethg++;
        switch_2.ethg++ <--> Eth10M <--> switch_3.ethg++;
        log_server.ethg++ <--> Eth10M <--> switch_1.ethg++;
        internet_host_1.ethg++ <--> Eth10M <--> switch_1.ethg++;
        internet_host_2.ethg++ <--> Eth10M <--> switch_1.ethg++;
        internet_host_3.ethg++ <--> Eth10M <--> switch_3.ethg++;
        internet_host_4.ethg++ <--> Eth10M <--> switch_3.ethg++;
}