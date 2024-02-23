//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


package inet.node.inet;


import inet.node.base.ApplicationLayerNodeBase;
import inet.applications.ethernet.EtherTrafGen;
import inet.applications.ethernet.EtherAppClient;
import inet.applications.ethernet.EtherAppServer;
import inet.applications.ethernet.EthernetSinkApp;
import inet.applications.ethernet.EthernetSourceApp;
import inet.applications.ieee8022.Ieee8022LlcSourceApp;
import inet.applications.ieee8022.Ieee8022LlcSinkApp;

//
// IPv4 host with SCTP, TCP, UDP layers and applications.
// IP forwarding is disabled by default (see forwarding).
// - Can be connected via ethernet interface to other nodes using
//   the ethg gate. By default full-duplex connections are supported
//   only (twisted pair). Set **.eth[*].typename="EthernetInterface" for
//   a full/half-duplex CSMA/CD implementation (coaxial cable)
// - By default contains no wireless cards, however it can be configured
//   by the numWlanInterfaces parameter. Wireless card type is configured by the
//   **.wlan[*].typename parameter. see:  inet.linklayer.ieee80211 or other
//   modules implementing ~IWirelessInterface
// - Also external interfaces can be configured for HW in the loop simulation
//   using the numEthInterfaces parameter and setting the interface type using
//   **.eth[*].typename in the INI file. see: ~ExtInterface derived modules and ~IExternalInterface
// - If wireless card is present, node mobility can be set using *.*.mobility.typename
//   see: inet.mobility and ~IMobility
//
module StandardHost2 extends ApplicationLayerNodeBase
{
    parameters:
        bool activateLLCsource = default(false);
        bool activateLLCsink = default(false);
        int numSourceApps = default(0);
        int numSinkApps = default(0);
        @display("i=device/pc2");
        @figure[submodules];
        forwarding = default(false);  // disable routing by default
        ipv4.arp.proxyArpInterfaces = default(""); // proxy arp is disabled on hosts by default
        *.routingTableModule = default("^.ipv4.routingTable");
        llc.typename = default("Ieee8022Llc");
        llc.registerProtocol = true;
        //bridging.typename = default(hasIncomingStreams || hasOutgoingStreams || hasStreamRedundancy || hasIngressTrafficFiltering || hasEgressTrafficFiltering ? "BridgingLayer" : ""); // switch to modular bridging
        //ethernet.typename = default("EthernetLayer"); // use Ethernet protocol layer outside of network interfaces
        //eth[*].typename = default("LayeredEthernetInterface"); // switch to modular Ethernet interface
        //eth[*].macLayer.typename = default("EthernetMacLayer");
        //eth[*].phyLayer.typename = default("EthernetPhyLayer");
    submodules:
        sourceApp[numSourceApps]: Ieee8022LlcSourceApp if activateLLCsource {
            parameters:
                @display("p=847.0841,374.10657,row,150");
        }
        sinkApp[numSinkApps]: Ieee8022LlcSinkApp if activateLLCsink {
            parameters:
                @display("p=1147.7054,374.10657,row,150");
        }

    connections allowunconnected:
        for i=0..numSourceApps-1 {
            sourceApp[i].socketOut --> nl.in++;
            sourceApp[i].socketIn <-- nl.out++;
        }
        //sourceApp[i].socketOut --> nl.in++ if exists(sourceApp);
        //nl.out++ --> sourceApp[i].socketIn if exists(sourceApp);

        for i=0..numSinkApps-1 {
            sinkApp[i].socketOut --> nl.in++;
            sinkApp[i].socketIn <-- nl.out++;
        }
        //sinkApp[i].socketOut --> nl.in++ if exists(sinkApp);
        //nl.out++ --> sinkApp[i].socketIn if exists(sinkApp);

}