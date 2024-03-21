#ifndef ANALYSIS_H
#define ANALYSIS_H

#define _CRT_SECURE_NO_WARNINGS

#define maxLink				100		// maximum links in the network
#define frameMAC			42		// number of bytes for the Ethernet frame overhead
#define MaxEthernetMessage	30000	// maximum number of messages in Ethernet
#define switchFabric		10000000//switch fabric delay is set to 10 microseconds, showing in ps
#define maxFrag				50		//maximum fragmentation value
#define Maxinst				50		//maximum number of instances for ST message

typedef long long int	time_type;

int NumberOfMSGs_Eth;
time_type networkSpeed = 100000;		//in Kbps
time_type MTU = 1500;					//MTU in Bytes
time_type MTUInPS;
time_type m_Tau;
time_type cycle[maxLink];
time_type guard;

// schedulability Error Code
typedef enum Sched {
	schedulable = 1,
	unschedulable = 2,
}schedFlag;
schedFlag schedulableFlag;

// enumeration of fragmentation flag
typedef enum fragmented {
	Fragmented = 1,
	NonFragmented = 2,
}fragFlag;

// class of AVB traffic, can be A, B or Non-Real-Time (NRT)
// priority 1 is reserved for future expansion
typedef enum msgClass {
	ST = 1,
	ClassA = 2,
	ClassB = 3,
	NRT = 4,
} MsgClass;

typedef struct inst
{
	time_type	inst[Maxinst];
}inst;


// the struct to save the message properties
typedef struct EthrMsg
{
	time_type	ID;					//ID of the message, should be unique, input
	time_type	localID[maxFrag];	//this ID is to keep track of messages which are fragmented
	time_type	fragmentedNumber;	//number of smaller messages after fragmentation
	fragFlag	fragmented;			//a flag to show if the message is fragmented
	time_type	lastChunkInBytes;	//the length in Bytes for the last chunk after defragmentation
	time_type	lastChunk;			//the length in ps for the last chunk after defragmentation
	time_type	lengthInBytes;		//length of the message in bytes, input
	time_type	lengthInBits;		//length of the message in bits, will be calculated
	time_type	transTime;			//length in picoseconds, will be calculated
	time_type	period;				//period of the message in picoseconds
	time_type	deadline;			//deadline of the message in picoseconds
	MsgClass	priority;			//priority of the message
	time_type	link[maxLink];		//links that the message crosses
	time_type	linkNbr;			//number of links a message crosses
	time_type	jitterLink[maxLink];//the jitter of only class A messages in each link
	time_type	resTime[maxLink];	//computed response time of the message for links
	time_type	responseTime;		//response time of the message crossing all links
	time_type	jitterIn;			//the jitter that is inherited from the sender node, in picoseconds
	time_type	jitterOut;			//the jitter that is given to the receiver node, in picoseconds
	time_type	offset[maxLink];	//the offset for ST messages on each link, in picoseconds
	time_type	initOffset;
	inst		instances[maxLink]; //the instances for the message according to the offset on a link
	time_type	instNumber;			//number of instances
} EthrMsgType;
EthrMsgType *EthMsgList;


// the struct of storing the alpha (idleSlope) for the links
// the idleSlope is in Mbps
typedef struct idleSlope
{
	double	alphaP_B;
	double	alphaP_A;
} idleSlopeValue;
idleSlopeValue linkIdleSlope[maxLink];


#endif