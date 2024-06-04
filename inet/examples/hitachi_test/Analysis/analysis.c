#include "analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int compareStrings(char* str1, char* str2) {
	int equal = 1;
	for (int i = 0; i < strlen(str2); i++) {
		if (str1[i] != str2[i]) {
			equal = 0;
		}
	}
	return equal;
}

time_type getParameter(char* str) {
	char parameter[100];
	int pos_parameter = 0;
	int is_parameter = 0;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '\n') {
			break;
		}
		if (is_parameter == 1) {
			parameter[pos_parameter] = str[i];
			pos_parameter++;
		}
		if (str[i] == ' ' && str[i-1] == '=') {
			is_parameter = 1;
		}
	}
	parameter[pos_parameter] = '\0';
	return atoll(parameter);
}

MsgClass getClass(char* str) {
	char msg_class[10];
	int pos_class = 0;
	int is_class = 0;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '\n') {
			break;
		}
		if (is_class == 1) {
			msg_class[pos_class] = str[i];
			pos_class++;
		}
		if (str[i] == ' ' && str[i - 1] == '=') {
			is_class = 1;
		}
	}
	msg_class[pos_class] = '\0';
	if (compareStrings(msg_class, "ST")) {
		return ST;
	}
	else if (compareStrings(msg_class, "ClassA")) {
		return ClassA;
	}
	else if (compareStrings(msg_class, "ClassB")) {
		return ClassB;
	}
	else if (compareStrings(msg_class, "NRT")) {
		return NRT;
	}
}

// add messages for industrial case study
int addMessage_industry(char* exp_ID) {

	FILE* fp;
	char str[100];
	char filename[100];
	
	//strcpy(filename, "C:\\Users\\dmu01\\Desktop\\test\\");
	strcpy(filename, ".\\Experiment");
	strcat(filename, exp_ID);
	strcat(filename, "\\AVB_Input.txt");

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Could not open file %s", filename);
		return 1;
	}

	int message_ID = -1;
	int link_ID = 0;
	int offset_ID = 0;
	printf("1: Starting loop\n");
	while (fgets(str, 500, fp) != NULL)
		if (compareStrings(str, "message")) {
			message_ID++;
			EthMsgList[message_ID].ID = message_ID;
			//printf("\nEthMsgList[%i].ID = %lld;\n", message_ID, EthMsgList[message_ID].ID);
			link_ID = 0;
			offset_ID = 0;
		}
		else if (compareStrings(str, "length")) {
			EthMsgList[message_ID].lengthInBytes = getParameter(str);
			//printf("EthMsgList[%i].lengthInBytes = %lld;\n", message_ID, EthMsgList[message_ID].lengthInBytes);
		}
		else if (compareStrings(str, "period")) {
			EthMsgList[message_ID].period = getParameter(str) * 1000000;
			//printf("EthMsgList[%i].period = %lld;\n", message_ID, EthMsgList[message_ID].period);
		}
		else if (compareStrings(str, "deadline")) {
			EthMsgList[message_ID].deadline = getParameter(str) * 1000000;
			//printf("EthMsgList[%i].deadline = %lld;\n", message_ID, EthMsgList[message_ID].deadline);
		}
		else if (compareStrings(str, "priority")) {
			EthMsgList[message_ID].priority = getClass(str);
			//printf("EthMsgList[%i].priority = %d;\n", message_ID, (int)EthMsgList[message_ID].priority);
		}
		else if (compareStrings(str, "linkNbr")) {
			EthMsgList[message_ID].linkNbr = getParameter(str);
			//printf("EthMsgList[%i].linkNbr = %lld;\n", message_ID, EthMsgList[message_ID].linkNbr);
		}
		else if (compareStrings(str, "link")) {
			EthMsgList[message_ID].link[link_ID] = getParameter(str);
			//printf("EthMsgList[%i].link[%i] = %lld;\n", message_ID, link_ID, EthMsgList[message_ID].link[link_ID]);
			link_ID++;
		}
		else if (compareStrings(str, "jitterIn")) {
			EthMsgList[message_ID].jitterIn = getParameter(str) * 1000000;
			//printf("EthMsgList[%i].jitterIn = %lld;\n", message_ID, EthMsgList[message_ID].jitterIn);
		}
		else if (compareStrings(str, "initOffset")) {
			EthMsgList[message_ID].initOffset = getParameter(str) * 1000000;
			//printf("EthMsgList[%i].initOffset = %lld;\n", message_ID, EthMsgList[message_ID].initOffset);
		}
		else if (compareStrings(str, "offset") && offset_ID < EthMsgList[message_ID].linkNbr) {
			EthMsgList[message_ID].offset[offset_ID] = getParameter(str) * 1000000;
			//printf("EthMsgList[%i].offset[%i] = %lld;\n", message_ID, offset_ID, EthMsgList[message_ID].offset[offset_ID]);
			offset_ID++;
		}
	printf("1: Loop finished\n");
	fclose(fp);
	printf("1: Close file\n");
	message_ID++;
	printf("MessageID: %d\n", message_ID);

	return message_ID;
}


// add idleSlopes to test manually
void addIdleSlope_industry(int num_messages) {

	for (int i = 0; i < maxLink; i++) {
		linkIdleSlope[i].alphaP_A = 0;
		linkIdleSlope[i].alphaP_B = 0;
	}

	linkIdleSlope[16].alphaP_A = 0.496;
	linkIdleSlope[16].alphaP_B = 0.496;

	linkIdleSlope[18].alphaP_A = 0.496;
	linkIdleSlope[18].alphaP_B = 0.496;

	linkIdleSlope[30].alphaP_A = 0.496;
	linkIdleSlope[30].alphaP_B = 0.496;

	linkIdleSlope[32].alphaP_A = 0.496;
	linkIdleSlope[32].alphaP_B = 0.496;
	
	linkIdleSlope[42].alphaP_A = 0.496;
	linkIdleSlope[42].alphaP_B = 0.496;

	linkIdleSlope[44].alphaP_A = 0.496;
	linkIdleSlope[44].alphaP_B = 0.496;


	/*
	for (int i = 0; i < maxLink; i++) {
		linkIdleSlope[i].alphaP_A = 0;
		linkIdleSlope[i].alphaP_B = 0;
	}

	double ST_utilization[maxLink];
	for (int i = 0; i < maxLink; i++) {
		ST_utilization[i] = 0.0;
	}

	for (int i = 0; i < num_messages; i++) {
		if (EthMsgList[i].priority == ST) {
			for (int j = 0; j < EthMsgList[i].linkNbr; j++) {
				ST_utilization[EthMsgList[i].link[j]] += (((double)EthMsgList[i].lengthInBytes / (double)EthMsgList[i].period) / ((double)networkSpeed / 8000000000.0));
			}
		}
	}
	for (int i = 0; i < num_messages; i++) {
		if (EthMsgList[i].priority == ClassA) {
			for (int j = 0; j < EthMsgList[i].linkNbr; j++) {
				linkIdleSlope[EthMsgList[i].link[j]].alphaP_A += (1.1/(1.0 - ST_utilization[EthMsgList[i].link[j]])) * (((double)EthMsgList[i].lengthInBytes / (double)EthMsgList[i].period) / ((double)networkSpeed/8000000000.0));
			}
		}
		if (EthMsgList[i].priority == ClassB) {
			for (int j = 0; j < EthMsgList[i].linkNbr; j++) {
				linkIdleSlope[EthMsgList[i].link[j]].alphaP_B += (1.1 / (1.0 - ST_utilization[EthMsgList[i].link[j]])) * (((double)EthMsgList[i].lengthInBytes / (double)EthMsgList[i].period) / ((double)networkSpeed / 8000000000.0));
			}
		}
	}
	double maxA = 0;
	double maxB = 0;
	for (int i = 0; i < maxLink; i++) {
		if (linkIdleSlope[i].alphaP_A > maxA) {
			maxA = linkIdleSlope[i].alphaP_A;
		}
		if (linkIdleSlope[i].alphaP_B > maxB) {
			maxB = linkIdleSlope[i].alphaP_B;
		}
	}
	for (int i = 0; i < maxLink; i++) {
		linkIdleSlope[i].alphaP_A = maxA;
		linkIdleSlope[i].alphaP_B = maxB;
	}
	*/
}


// find offsets of ST messages on other links than the first link
void findSToffsets() {
	int m, l;

	for (m = 0; m < NumberOfMSGs_Eth; m++) {
		if (EthMsgList[m].priority == ST) {
			for (l = 0; l < EthMsgList[m].linkNbr; l++) {
				EthMsgList[m].offset[l] = EthMsgList[m].initOffset * l;
			}
		}
	}
}


time_type LCMcalc(time_type v1, time_type v2) {
	time_type lcm;
	time_type n1, n2;

	n1 = v1;
	n2 = v2;
	
	while (n1 != n2) {
		if (n1 > n2) {
			n1 = n1 - n2;
		}
		else {
			n2 = n2 - n1;
		}
	}

	lcm = (v1 / n1) * v2;
	return lcm;
}

// check if message m share link l with message i
int checkSharedLink(long int l, long int i)
{
	int x;

	for (x = 0; x < EthMsgList[i].linkNbr; x++) {
		if (EthMsgList[i].link[x] == l) {
			return 1;
		}
	}
	return -1;
}

// define cycle of the ST class per link, the member is the link ID
// cycle is the cycle of gates operation (when it repeats again)
void cycleLink() {
	int i, l, m;
	int count;
	time_type lcmValue = 0;

	for (i = 0; i < maxLink; i++) {
		cycle[i] = 0;
	}

	for (l = 0; l < maxLink; l++) {
		count = 0;
		lcmValue = 0;
		for (m = 0; m < NumberOfMSGs_Eth; m++) {
			if (EthMsgList[m].priority == ST) {
				if (checkSharedLink(l, m) >= 0) {
					count++;
					if (count == 1) {
						lcmValue = EthMsgList[m].period;
					}
					else {
						lcmValue = LCMcalc(lcmValue, EthMsgList[m].period);
					}
				}
			}
		}
		cycle[l] = lcmValue;
	}
}

// if we receive payload larger than 1500B, we need to defragment it into several messages
// We use Maximum Transmission Unit (MTU) to break the data payload, MTU is given by the user, but the default is 1500B
void defragmentation() {
	int i, j;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].lengthInBytes > 1500) {
			EthMsgList[i].fragmented = Fragmented;
			EthMsgList[i].fragmentedNumber = ceil((double)EthMsgList[i].lengthInBytes / MTU);
			EthMsgList[i].lastChunkInBytes = EthMsgList[i].lengthInBytes - (EthMsgList[i].fragmentedNumber - 1) * MTU;
			for (j = 0; j < EthMsgList[i].fragmentedNumber; j++) {
				EthMsgList[i].localID[j] = j;
			}
		}
	}
}


// in this case we only have 168B for each message because of class measure interval of 125us for A and 250us for B
void defragmentation_industrycase() {
	int i, j;

	//for NRT traffic we use the MTU size to divide
	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == NRT) {
			if (EthMsgList[i].lengthInBytes > 1500) {
				EthMsgList[i].fragmented = Fragmented;
				EthMsgList[i].fragmentedNumber = ceil((double)EthMsgList[i].lengthInBytes / MTU);
				EthMsgList[i].lastChunkInBytes = EthMsgList[i].lengthInBytes - (EthMsgList[i].fragmentedNumber - 1) * MTU;
				for (j = 0; j < EthMsgList[i].fragmentedNumber; j++) {
					EthMsgList[i].localID[j] = j;
				}
			}
		}
	}

	//for A and B we use 168B size
	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].lengthInBytes > 1500) {
			EthMsgList[i].fragmented = Fragmented;
			if (EthMsgList[i].priority == ClassA) {
				EthMsgList[i].fragmentedNumber = ceil((double)EthMsgList[i].lengthInBytes / 168);
				EthMsgList[i].lastChunkInBytes = EthMsgList[i].lengthInBytes - (EthMsgList[i].fragmentedNumber - 1) * 168;
				for (j = 0; j < EthMsgList[i].fragmentedNumber; j++) {
					EthMsgList[i].localID[j] = j;
				}
			}
			else if (EthMsgList[i].priority == ClassB) {
				EthMsgList[i].fragmentedNumber = ceil((double)EthMsgList[i].lengthInBytes / 50);
				EthMsgList[i].lastChunkInBytes = EthMsgList[i].lengthInBytes - (EthMsgList[i].fragmentedNumber - 1) * 50;
				for (j = 0; j < EthMsgList[i].fragmentedNumber; j++) {
					EthMsgList[i].localID[j] = j;
				}
			}
		}
	}

}


// caclulate the length in bits and transmission time in picoseconds, note the length input is in bytes
void extractTransTime(){
	int i;
	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ST) {
			EthMsgList[i].lengthInBits = (EthMsgList[i].lengthInBytes + frameMAC) * 8;
			EthMsgList[i].transTime = EthMsgList[i].lengthInBits * m_Tau;
		}
		else {
			if (EthMsgList[i].fragmented == Fragmented) {
				EthMsgList[i].lastChunk = (EthMsgList[i].lastChunkInBytes + frameMAC) * 8 * m_Tau;
				EthMsgList[i].transTime = ((MTU + frameMAC) * 8 * (EthMsgList[i].fragmentedNumber - 1) * m_Tau) + EthMsgList[i].lastChunk;
			}
			else if (EthMsgList[i].fragmented == NonFragmented) {
				EthMsgList[i].lengthInBits = (EthMsgList[i].lengthInBytes + frameMAC) * 8;
				EthMsgList[i].transTime = EthMsgList[i].lengthInBits * m_Tau;
			}
		}
	}
}

// caclulate the length in bits and transmission time in picoseconds, note the length input is in bytes
void extractTransTime_industrycase() {
	int i;
	int localMTU;
	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ST) {
			EthMsgList[i].lengthInBits = (EthMsgList[i].lengthInBytes + frameMAC) * 8;
			EthMsgList[i].transTime = EthMsgList[i].lengthInBits * m_Tau;
		}
		else {
			if (EthMsgList[i].fragmented == Fragmented) {
				EthMsgList[i].lastChunk = (EthMsgList[i].lastChunkInBytes + frameMAC) * 8 * m_Tau;
				if (EthMsgList[i].priority == ClassA) {
					localMTU = 168;
				}
				else if (EthMsgList[i].priority == ClassB) {
					localMTU = 50;
				}
				else if (EthMsgList[i].priority == NRT) {
					localMTU = 1500;
				}
				EthMsgList[i].transTime = ((localMTU + frameMAC) * 8 * (EthMsgList[i].fragmentedNumber - 1) * m_Tau) + EthMsgList[i].lastChunk;
			}
			else if (EthMsgList[i].fragmented == NonFragmented) {
				EthMsgList[i].lengthInBits = (EthMsgList[i].lengthInBytes + frameMAC) * 8;
				EthMsgList[i].transTime = EthMsgList[i].lengthInBits * m_Tau;
			}
		}
	}
}

// find the blocking term for Class B
time_type findBlocking(time_type msg, time_type link){
	time_type blck = 0;
	time_type i;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority > EthMsgList[msg].priority) {
			if (checkSharedLink(link, i) >= 0) {
				if (EthMsgList[i].fragmented == Fragmented) {
					if (MTUInPS > blck) {
						blck = MTUInPS;
					}
				}
				if (EthMsgList[i].fragmented == NonFragmented) {
					if (EthMsgList[i].transTime > blck) {
						blck = EthMsgList[i].transTime;
					}
				}
			}
		}
	}
	return blck;
}

// find the high priority interference
time_type findHighIntForQ(time_type msg, time_type prev_t, time_type link) {
	time_type hpTerm = 0;
	time_type i, j;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ClassA) {
			if (checkSharedLink(link, i) >= 0) {
				if (EthMsgList[i].fragmented == NonFragmented) {
					hpTerm = hpTerm + ceil((double)(prev_t + EthMsgList[i].jitterLink[link]) / EthMsgList[i].period) * EthMsgList[i].transTime;
				}
				else if (EthMsgList[i].fragmented == Fragmented) {
					for (j = 0; j < EthMsgList[i].fragmentedNumber; j++) {
						if (j == EthMsgList[i].fragmentedNumber - 1) {
							hpTerm = hpTerm + ceil((double)(prev_t + EthMsgList[i].jitterLink[link]) / EthMsgList[i].period) * EthMsgList[i].lastChunk;
						}
						else {
							hpTerm = hpTerm + ceil((double)(prev_t + EthMsgList[i].jitterLink[link]) / EthMsgList[i].period) * MTUInPS;
						}
					}
				}
			}
		}
	}

	return hpTerm;
}

// find the same priority interference
time_type findSameIntForQ(time_type msg, time_type link, time_type q) {
	time_type i;
	time_type floorValue;
	time_type spTerm = 0;

	//int idleSlopeIndex = EthMsgList[msg].link[link];

	//it does not matter if the interfering messages are fragmented or not
	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ClassB) {
			if (msg != i) {
				if (checkSharedLink(link, i) >= 0) {
					floorValue = floor((double)(((q - 1) * EthMsgList[msg].period) / EthMsgList[i].period) + 1);
					spTerm = spTerm + floorValue * ((double)(EthMsgList[i].transTime) / linkIdleSlope[link].alphaP_B);
				}
			}
		}
	}
	return spTerm;
}

time_type findItselfInt(time_type msg, time_type link, time_type spTerm) {
	time_type itself = 0;

	//int idleSlopeIndex = EthMsgList[msg].link[link];

	if (EthMsgList[msg].fragmented == Fragmented) {
		//itself = ((double)EthMsgList[msg].transTime / linkIdleSlope[link].alphaP_B);
		itself = EthMsgList[msg].transTime;
	}
	else if (EthMsgList[msg].fragmented == NonFragmented) {
		if (spTerm > 0) {
			//itself = ((double)EthMsgList[msg].transTime / linkIdleSlope[link].alphaP_B);
			itself = EthMsgList[msg].transTime;
		}
		else if (spTerm == 0) {
			itself = EthMsgList[msg].transTime;
		}
	}
	return itself;
}

time_type findSTint(time_type msg, time_type link, time_type prev_t, time_type c, time_type it) {
	time_type stInt = 0;
	time_type phase;
	int i;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ST) {
			if (checkSharedLink(link, i) >= 0) {
				phase = (EthMsgList[i].offset[link] - EthMsgList[c].instances[link].inst[it]) % EthMsgList[i].period;
				stInt = stInt + (floor((double)phase / EthMsgList[i].period) + ceil((double)(prev_t - phase) / EthMsgList[i].period)) * EthMsgList[i].transTime;
			}
		}
	}

	return stInt;
}

time_type findSTpreempt(time_type msg, time_type link, time_type prev_t, time_type c, time_type it) {
	time_type preempt = 0;
	time_type phase;
	int i;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ST) {
			if (checkSharedLink(link, i) >= 0) {
				phase = (EthMsgList[i].offset[link] - EthMsgList[c].instances[link].inst[it]) % EthMsgList[i].period;
				preempt = preempt + (floor((double)phase / EthMsgList[i].period) + ceil((double)(prev_t - phase) / EthMsgList[i].period)) * frameMAC;
			}
		}
	}

	return preempt;
}

// find the maximum q (instance) to search for the worst-case
time_type findQmax(time_type msg, time_type link) {
	time_type blocking;
	time_type delay = 1;
	time_type q = 0;
	time_type hpTerm = 0;
	time_type spTerm = 0;
	time_type itselfTerm = 0;
	time_type stTerm = 0;
	time_type stPreempt = 0;
	time_type qMax = 0;
	time_type t, prev_t;
	time_type phase;
	time_type prevQ = 0;
	int c, it;

	//find the low priority blocking
	blocking = findBlocking(msg, link);


	//in this loop we find the maximum q (qMax)
	while (delay > q * EthMsgList[msg].period)
	{
		//if the message m is fragmented, then we compute the response time for the last chunk
		//in this case, the other chunks are the same priority interference
		if (EthMsgList[msg].fragmented == NonFragmented) {
			t = EthMsgList[msg].transTime;
		}
		else if (EthMsgList[msg].fragmented == Fragmented) {
			t = EthMsgList[msg].lastChunk;
		}
		q++;

		//calculate the queuing delay
		for (c = 0; c < NumberOfMSGs_Eth; c++) {
			if (EthMsgList[c].priority == ST) {
				if (checkSharedLink(link, c) >= 0) {
					for (it = 0; it < EthMsgList[c].instNumber; it++) {
						prev_t = -1;
						t = 0;
						while (t != prev_t) {
							prev_t = t;
							hpTerm = findHighIntForQ(msg, prev_t, link);
							spTerm = findSameIntForQ(msg, link, q);
							itselfTerm = findItselfInt(msg, link, spTerm);
							stTerm = findSTint(msg, link, prev_t, c, it);
							stPreempt = findSTpreempt(msg, link, prev_t, c, it);
							t = blocking + spTerm + (itselfTerm * q) + hpTerm + stTerm + stPreempt;
						}
						if (t > delay) {
							delay = t;
						}
					}
				}
			}
			else if (EthMsgList[c].priority == ClassA) {
				if (checkSharedLink(link, c) >= 0) {
					prev_t = -1;
					t = 0;
					while (t != prev_t) {
						prev_t = t;
						hpTerm = findHighIntForQ(msg, prev_t, link);
						spTerm = findSameIntForQ(msg, link, q);
						itselfTerm = findItselfInt(msg, link, spTerm);
						t = blocking + spTerm + (itselfTerm * q) + hpTerm;
					}
					if (t > delay) {
						delay = t;
					}
				}
			}
		}
	
		//update for the next iteration
		prevQ = q;
		if (prevQ > qMax) {
			qMax = prevQ;
		}
	}

	return qMax;
}

time_type findHighInt(time_type msg, time_type prev_w, time_type link) {
	time_type hpTerm = 0;
	time_type i, j;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ClassA) {
			if (checkSharedLink(link, i) >= 0) {
				if (EthMsgList[i].fragmented == NonFragmented) {
					hpTerm = hpTerm + floor((double)(((prev_w + EthMsgList[i].jitterLink[link]) / EthMsgList[i].period) + 1)) * EthMsgList[i].transTime;
				}
				else if (EthMsgList[i].fragmented == Fragmented) {
					for (j = 0; j < EthMsgList[i].fragmentedNumber; j++) {
						if (j == EthMsgList[i].fragmentedNumber - 1) {
							hpTerm = hpTerm + floor((double)(((prev_w + EthMsgList[i].jitterLink[link]) / EthMsgList[i].period) + 1)) * EthMsgList[i].lastChunk;
						}
						else {
							hpTerm = hpTerm + floor((double)(((prev_w + EthMsgList[i].jitterLink[link]) / EthMsgList[i].period) + 1)) * MTUInPS;
						}
					}
				}
			}
		}
	}

	return hpTerm;
}

// find the utilization for a particular message
// in case of large messages (which need fragmentation), taking the complete size is enough
int utilizationCheckClassB(time_type link) {
	double utilClassA = 0;
	double utilClassB = 0;
	time_type i;

	//int idleSlopeIndex = EthMsgList[msg].link[link];

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ClassA) {
			if (checkSharedLink(link, i) >= 0) {
				utilClassA = utilClassA + ((double)EthMsgList[i].transTime / (double)EthMsgList[i].period);
			}
		}
		else if (EthMsgList[i].priority == ClassB) {
			if (checkSharedLink(link, i) >= 0) {
				utilClassB = utilClassB + ((double)EthMsgList[i].transTime / (linkIdleSlope[link].alphaP_B *(double)EthMsgList[i].period));
			}
		}
	}

	//printf("The total utilization on link %d is %f \n", link, utilClassA + utilClassB);

	if (utilClassA + utilClassB < 1) {
		return 1;
	}
	else {
		//return 0;
		return 1;
	}
}

time_type responseTimeClac(time_type msg, time_type q, time_type link) {
	time_type blocking;
	time_type RT = 0;
	time_type w = 0;
	time_type w1 = 0;
	time_type prev_w = -1;
	time_type hpInt = 0;
	time_type spInt = 0;
	time_type itselfInt = 0;
	time_type stInt = 0;
	time_type stPreempt = 0;
	time_type phase;
	time_type t;
	time_type prev_t = -1;
	time_type wTime;
	int c, it;

	//int idleSlopeIndex = EthMsgList[msg].link[link];

	//find the blocking by lower priority messages
	blocking = findBlocking(msg, link);

	//calculation of first phase (the busy period)
	wTime = 0;
	for (c = 0; c < NumberOfMSGs_Eth; c++) {
		if (EthMsgList[c].priority == ST) {
			if (checkSharedLink(link, c) >= 0) {
				for (it = 0; it < EthMsgList[c].instNumber; it++) {
					prev_w = -1;
					w1 = 0;
					while (w1 != prev_w) {
						prev_w = w1;
						hpInt = findHighInt(msg, prev_w, link);
						spInt = findSameIntForQ(msg, link, q);
						itselfInt = findItselfInt(msg, link, spInt);
						stInt = findSTint(msg, link, prev_w, c, it);
						stPreempt = findSTpreempt(msg, link, prev_w, c, it);
						w1 = blocking + hpInt + spInt + ((q - 1) * itselfInt) + stInt + stPreempt;
					}
					if (w1 > wTime) {
						wTime = w1;
					}
				}
			}
		}
		else if (EthMsgList[c].priority == ClassA) {
			if (checkSharedLink(link, c) >= 0) {
				prev_w = -1;
				w1 = 0;
				while (w1 != prev_w) {
					prev_w = w1;
					hpInt = findHighInt(msg, prev_w, link);
					spInt = findSameIntForQ(msg, link, q);
					itselfInt = findItselfInt(msg, link, spInt);
					w1 = blocking + hpInt + spInt + ((q - 1) * itselfInt);
				}
				if (w1 > wTime) {
					wTime = w1;
				}
			}
		}
	}

	//calculate the second phase (only preemption by ST, but starting after the busy period)
	for (c = 0; c < NumberOfMSGs_Eth; c++) {
		if (EthMsgList[c].priority == ST) {
			if (checkSharedLink(link, c) >= 0) {
				for (it = 0; it < EthMsgList[c].instNumber; it++) {
					prev_t = -1;
					t = wTime;
					while (t != prev_t) {
						prev_t = t;
						spInt = findSameIntForQ(msg, link, q);
						itselfInt = findItselfInt(msg, link, spInt);
						stInt = findSTint(msg, link, t, c, it);
						stPreempt = findSTpreempt(msg, link, t, c, it);
						t = wTime + stInt + stPreempt + itselfInt + ((q - 1) * EthMsgList[msg].period);
					}
					if (t > RT) {
						RT = t;
					}
				}
			}
		}
		else if (EthMsgList[c].priority == ClassA) {
			if (checkSharedLink(link, c) >= 0) {
				prev_t = -1;
				t = wTime;
				while (t != prev_t) {
					prev_t = t;
					spInt = findSameIntForQ(msg, link, q);
					itselfInt = findItselfInt(msg, link, spInt);
					t = wTime + itselfInt + ((q - 1) * EthMsgList[msg].period);
				}
				if (t > RT) {
					RT = t;
				}
			}
		}
	}

	return RT;
}


void findJitterOfClassA() {
	time_type i, l, k;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ClassA) {
			for (l = 0; l < EthMsgList[i].linkNbr; l++) {
				for (k = 0; k <= l; k++) {
					EthMsgList[i].jitterLink[EthMsgList[i].link[l]] = EthMsgList[i].jitterLink[EthMsgList[i].link[l]] + EthMsgList[i].resTime[EthMsgList[i].link[k]];
				}
				EthMsgList[i].jitterLink[EthMsgList[i].link[l]] = EthMsgList[i].jitterLink[EthMsgList[i].link[l]] - (l + 1) * EthMsgList[i].transTime;
			}
		}
	}
}

// find instances of ST messages according to the offset
void findInstances() {
	time_type i, l, k, link;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ST) {
			for (l = 0; l < EthMsgList[i].linkNbr; l++) {
				link = EthMsgList[i].link[l];
				EthMsgList[i].instNumber = cycle[link] / EthMsgList[i].period;
				for (k = 0; k < EthMsgList[i].instNumber; k++) {
					EthMsgList[i].instances[l].inst[k] = k * EthMsgList[i].period + EthMsgList[i].offset[l];
				}
			}
		}
	}
}

// find the ST interference
time_type findSTInterference(time_type linkID, time_type t, time_type c, time_type it) {
	time_type interfST = 0;
	time_type phase;
	int i, l, lin;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ST) {
			if (checkSharedLink(linkID, i) >= 0) {
				for (l = 0; l < EthMsgList[i].linkNbr; l++) {
					if (linkID == EthMsgList[i].link[l]) {
						lin = l;
						break;
					}
				}
				phase = (EthMsgList[i].offset[lin] - EthMsgList[c].instances[lin].inst[it]) % EthMsgList[i].period;
				interfST = interfST + (floor((double)phase / EthMsgList[i].period) + ceil((double)(t - phase) / EthMsgList[i].period)) * EthMsgList[i].transTime;
			}
		}
	}
	return interfST;
}

// find the preemption contribution
time_type findPreemption(time_type linkID, time_type t, time_type c, time_type it) {
	time_type preemptST = 0;
	time_type phase;
	int i, l, lin;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		if (EthMsgList[i].priority == ST) {
			if (checkSharedLink(linkID, i) >= 0) {
				for (l = 0; l < EthMsgList[i].linkNbr; l++) {
					if (linkID == EthMsgList[i].link[l]) {
						lin = l;
					}
				}
				phase = (EthMsgList[i].offset[lin] - EthMsgList[c].instances[lin].inst[it]) % EthMsgList[i].period;
				preemptST = preemptST + (floor((double)phase / EthMsgList[i].period) + ceil((double)(t - phase) / EthMsgList[i].period)) * frameMAC * 8 * m_Tau;
			}
		}
	}
	return preemptST;
}


// calculate response time of AVB Ethernet messages
// all response times are calculated in picoseconds
void Compute_RT_AVB_Msg()
{
	int m, l, i, ll, x;
	int c, it;
	int link, linkID;
	time_type resTime;
	time_type Blocking;
	time_type samePriorityInt;
	time_type itselfInt;
	time_type STInterf;
	time_type preempt;
	time_type qMax, q;
	time_type RT, prevRT;
	int utilCheck;
	time_type t = 0;
	time_type prev_t = 0;
	int off = 0;

	// check the utilization for each link
	for (ll = 0; ll < maxLink; ll++) {
		utilCheck = utilizationCheckClassB(ll);
		if (utilCheck == 0) {
			return;
		}
	}

	// service all class A messages in this loop
	// we start from the first link of the messages
	// the response time, jitter output and jitter input are in picoseconds
	for (m = 0; m < NumberOfMSGs_Eth; m++) {
		if (EthMsgList[m].priority == ClassA) {
			t = EthMsgList[m].transTime;
			for (l = 0; l < EthMsgList[m].linkNbr; l++) {
				Blocking = 0;
				itselfInt = 0;
				samePriorityInt = 0;
				linkID = EthMsgList[m].link[l];

				// find the same priority interference
				// it does not matter if the same priority messages are fragmented or not, we take the whole length (not-fragmented one)
				for (i = 0; i < NumberOfMSGs_Eth; i++) {
					if (EthMsgList[i].priority == ClassA) {
						if (m != i) {
							if (checkSharedLink(EthMsgList[m].link[l], i) >= 0) {
								samePriorityInt = samePriorityInt + ((double)(EthMsgList[i].transTime) / linkIdleSlope[linkID].alphaP_A);
							}
						}
					}
				}
				// find the interference of the message itself
				// if message m is fragmented, then for sure it has the same priority interference (at least from the other chunks of message m)
				// in that case we just take the length of message without fragmentation with inflation factor
				if (EthMsgList[m].fragmented == Fragmented) {
					//itselfInt = ((double)(EthMsgList[m].transTime) / linkIdleSlope[linkID].alphaP_A);
					itselfInt = EthMsgList[m].transTime;
				}
				else if (EthMsgList[m].fragmented == NonFragmented) {
					if (samePriorityInt > 0) {
						//itselfInt = (double)EthMsgList[m].transTime / linkIdleSlope[linkID].alphaP_A;
						itselfInt = EthMsgList[m].transTime;
					}
					else if (samePriorityInt == 0) {
						itselfInt = EthMsgList[m].transTime;
					}
				}

				// find the blocking
				// if the blocking is by a fragmented message, we take only one chunk (which is equal to MTU)
				for (i = 0; i < NumberOfMSGs_Eth; i++) {
					if (EthMsgList[i].priority > EthMsgList[m].priority) {
						if (checkSharedLink(linkID, i) >= 0) {
							if (EthMsgList[i].fragmented == Fragmented) {
								if (MTUInPS > Blocking) {
									Blocking = MTUInPS;
								}
							}
							else if (EthMsgList[i].fragmented == NonFragmented) {
								if (EthMsgList[i].transTime > Blocking) {
									Blocking = EthMsgList[i].transTime;
								}
							}
						}
					}
				}
				// calculate the response time for message m crossing link l only
				// here we will have several critical instant candidates, the biggest should be considered
				resTime = 0;
				for (c = 0; c < NumberOfMSGs_Eth; c++) {
					if (EthMsgList[c].priority == ST) {
						if (checkSharedLink(linkID, c) >= 0) {
							for (it = 0; it < EthMsgList[c].instNumber; it++) {
								t = EthMsgList[m].transTime;
								prev_t = 0;
								while (t != prev_t) {
									prev_t = t;
									STInterf = findSTInterference(linkID, t, c, it);
									preempt = findPreemption(linkID, t, c, it);
									t = Blocking + samePriorityInt + itselfInt + STInterf + preempt;
								}
								if (t > resTime) {
									resTime = t;
								}
							}
						}
						else {
							t = Blocking + samePriorityInt + itselfInt;
							if (t > resTime) {
								resTime = t;
							}
						}
					}
				}
				if (resTime == 0) { //DANI
					EthMsgList[m].resTime[linkID] = Blocking + samePriorityInt + itselfInt;
				}
				else {
					EthMsgList[m].resTime[linkID] = resTime;
				}
				if (EthMsgList[m].resTime[linkID] > EthMsgList[m].period)
					printf("message %d in Class A is not schedulable on link %d!! \n", m, l);
			}
			// calculate the response time for message m crossing all links
			for (l = 0; l < EthMsgList[m].linkNbr; l++){
				link = EthMsgList[m].link[l];
				EthMsgList[m].responseTime = EthMsgList[m].responseTime + EthMsgList[m].resTime[link];
			}
			// add the switch fabric latency
			EthMsgList[m].responseTime = EthMsgList[m].responseTime + (EthMsgList[m].linkNbr - 1) * switchFabric;
			// add the input jitter from the sender task
			EthMsgList[m].responseTime = EthMsgList[m].responseTime + EthMsgList[m].jitterIn;
			EthMsgList[m].jitterOut = EthMsgList[m].responseTime - ((EthMsgList[m].transTime * EthMsgList[m].linkNbr) + (EthMsgList[m].linkNbr - 1) * switchFabric);

		}
	}

	// compute the jitter of each Class A message on each link that it crosses
	// we need these jitters when computing the response time for Class B messages
	findJitterOfClassA();

	// service all class B messages in this loop
	// we start from their first link

	for (m = 0; m < NumberOfMSGs_Eth; m++) {
		if (EthMsgList[m].priority == ClassB) {
			for (l = 0; l < EthMsgList[m].linkNbr; l++) {
				RT = 0;
				prevRT = 0;
				qMax = findQmax(m, EthMsgList[m].link[l]);
				for (q = 1; q <= qMax; q++) {
					RT = responseTimeClac(m, q, EthMsgList[m].link[l]);
					if (RT > prevRT) {
						EthMsgList[m].resTime[EthMsgList[m].link[l]] = RT;
						prevRT = RT;
					}
				}
				if (EthMsgList[m].resTime[EthMsgList[m].link[l]] > EthMsgList[m].period)
					printf("message %d in Class B is not schedulable on link %d!! \n", m, l);
			}
			// calculate the response time for message m crossing all links
			for (l = 0; l < EthMsgList[m].linkNbr; l++){
				link = EthMsgList[m].link[l];
				EthMsgList[m].responseTime = EthMsgList[m].responseTime + EthMsgList[m].resTime[link];
			}
			// add the switch fabric latency
			EthMsgList[m].responseTime = EthMsgList[m].responseTime + (EthMsgList[m].linkNbr - 1) * switchFabric;
			// add the input jitter from the sender task
			EthMsgList[m].responseTime = EthMsgList[m].responseTime + EthMsgList[m].jitterIn;
			EthMsgList[m].jitterOut = EthMsgList[m].responseTime - ((EthMsgList[m].transTime * EthMsgList[m].linkNbr) + (EthMsgList[m].linkNbr - 1) * switchFabric);
		}
	}


	//calculate for ST
	for (m = 0; m < NumberOfMSGs_Eth; m++) {
		if (EthMsgList[m].priority == ST) {
			EthMsgList[m].responseTime = EthMsgList[m].initOffset * (EthMsgList[m].linkNbr - 1) + EthMsgList[m].transTime;
		}
	}

}


//reset the response times in all links
//also check if we need fragmentation of messages
void initialize()
{
	time_type i, l, k;

	schedulableFlag = schedulable;

	m_Tau = 1000000000 / networkSpeed;
	MTUInPS = (MTU + frameMAC) * 8 * m_Tau;

	guard = (frameMAC + 81) * 8 * m_Tau;

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		EthMsgList[i].responseTime = 0;
		EthMsgList[i].fragmented = NonFragmented;
		for (l = 0; l < maxLink; l++) {
			EthMsgList[i].resTime[l] = 0;
			EthMsgList[i].jitterLink[l] = 0;
		}
	}

	for (i = 0; i < NumberOfMSGs_Eth; i++) {
		for (l = 0; l < maxLink; l++) {
			for (k = 0; k < Maxinst; k++) {
				EthMsgList[i].instances[l].inst[k] = 0;
			}
		}
	}

}

void Print_resoult(char* exp_ID, int num_messages) {

	FILE* fp;
	char filename[100];
	printf("Printing to file...\n");
	//strcpy(filename, "C:\\Users\\dmu01\\Desktop\\test\\");
	strcpy(filename, ".\\Experiment");
	strcat(filename, exp_ID);
	strcat(filename, "\\");
	strcat(filename, "final_results.txt");

	fp = fopen(filename, "w");

	for (int i = 0; i < num_messages; i++) {
		
		if (EthMsgList[i].priority == ClassA || EthMsgList[i].priority == ClassB) {

			fprintf(fp, "%i;%i;%lli;%lli;\n", (int)EthMsgList[i].ID, (int)EthMsgList[i].priority, EthMsgList[i].deadline, EthMsgList[i].responseTime);
			printf("%i;%i;%lli;%lli;\n", (int)EthMsgList[i].ID, (int)EthMsgList[i].priority, EthMsgList[i].deadline, EthMsgList[i].responseTime);

		}
		
	}
	fclose(fp);
}

void Jitter_Output_Calculation(int num_messages) {
	for (int i = 0; i < num_messages; i++) {
		if (EthMsgList[i].priority != ST) {
			EthMsgList[i].jitterOut = EthMsgList[i].responseTime - (((EthMsgList[i].linkNbr - 1) * (EthMsgList[i].transTime + switchFabric)) + EthMsgList[i].transTime);
		}
	}
}


void main(int argc, char** argv)
{
	EthMsgList = (EthrMsgType*)calloc(MaxEthernetMessage, sizeof(EthrMsgType));
	if (EthMsgList == NULL) {
        printf("Memory allocation failed...");
        exit(EXIT_FAILURE);
    }
	//EthrMsgType EthMsgList[131251];
	int num_messages;
	int exp_ID;
	//addMessage();
	//addIdleSlope();
	exp_ID = atoi(argv[1]);
	printf("1\n");
	num_messages = addMessage_industry(argv[1]);
	NumberOfMSGs_Eth = num_messages;
	printf("2\n");
	addIdleSlope_industry(num_messages);
	printf("3\n");
	initialize();
	printf("4\n");
	//defragmentation();
	//defragmentation_industrycase();
	extractTransTime();
	printf("5\n");
	//extractTransTime_industrycase();
	cycleLink();
	printf("6\n");
	//findSToffsets();//offset calculations
	findInstances();
	printf("7\n");
	Compute_RT_AVB_Msg();
	printf("8\n");
	//Jitter_Output_Calculation(num_messages);

	printf("num_messages: %d\n", num_messages);
	Print_resoult(argv[1], num_messages);
	printf("9");
}
