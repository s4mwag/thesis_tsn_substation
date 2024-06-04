from asyncore import write
from contextlib import AsyncExitStack
from fileinput import close
from itertools import cycle
from os import fdopen
from operator import itemgetter
from typing import final
import csv
import warnings
import numpy as np 
import json 
import math

class port:
	def __init__(self, id, ip, portNum):
		self.id = id
		self.ip = ip
		self.portNum = portNum

def createGCL(cycles):
	i = int(format(2**cycles[0][0], "8b"))
	GCL=[[int(cycles[0][2]-cycles[0][1]), i]]
	cycles=np.delete(cycles,0,0)
	for line in cycles:
		i = int(format(2**line[0], "8b"))
		GCL = np.vstack([GCL, [int(line[2]-line[1]), i]])
	return GCL

def writeJson(fp, gateContrList):
	json.dump([{'switch': portInfo.id, 'ip': portInfo.ip, 'port_list': [{'port_number': portInfo.portNum, 'values': gateContrList.tolist()}]}], fp)



def selectPort(linkNumber):
	switch={
		1:"64",
		11:"65",
		2:"66",
		21:"65",
		3:"65",
		31:"68",
		4:"67",
		41:"68",
		5:"69",
		51:"68"
	}
	switchId = switch.get(linkNumber)
	switchIp = "192.168.4."+switchId
	switch={
		1:"0",
		11:"0",
		2:"0",
		21:"1",
		3:"2",
		31:"0",
		4:"0",
		41:"1",
		5:"0",
		51:"2"
	}
	p = port(switchId, switchIp, switch.get(linkNumber))
	return p


it=0
prevQueue=0
prevLink=7
queue=[]
cycleStart=[]
cycleStop=[]
linkPeriods=[]
finalCycle=np.array([8,0,0])
endGCL=0.0


schd=csv.reader(open("Schedule.txt"), delimiter=";")
outputFile=open("configuration.json","a")

sortSchd=sorted(schd, key=lambda x: int(x[1]))

#sortSchd=sorted(schd, key=itemgetter(1))

sortSchd.pop()
maxLinks=(sortSchd[-1][0])

hyperperiod = {}

with open("STHS_input.txt", "r") as messages:
    for message_line in messages:
        if message_line.startswith('period'):
            current_period = int(message_line.split(' ')[2])
        
        if message_line.startswith('link ='):    
            link_number = int(message_line.split(' ')[2])

            if link_number in hyperperiod:
                hyperperiod[link_number] = math.lcm(current_period, hyperperiod[link_number])
            else:
                hyperperiod[link_number] = current_period
                
    hyperperiod = dict(sorted(hyperperiod.items()))


for line in sortSchd:

	if int(line[1])==prevLink:
		queue.append(int(line[2]))
		cycleStart.append(int(line[3]))
		cycleStop.append(int(line[4]))
		#linkPeriods.append(int(line[5]))

	if int(line[1])!=prevLink or line==sortSchd[-1]:
		#selPort=selectPort(prevLink)
		cycles=np.column_stack((queue, cycleStart, cycleStop))
		orderedCycles=cycles[cycles[:,1].argsort()]

		endGCL=hyperperiod[int(line[1])]
		
		if orderedCycles[0][1]!=0:
			finalCycle=np.array([[7, 0, orderedCycles[0][1]]])
		finalCycle=np.vstack([finalCycle,orderedCycles[0]])

		while it<=(len(orderedCycles)-1):
			while it<(len(orderedCycles)-1) and (orderedCycles[it][2]==orderedCycles[it+1][1] and orderedCycles[it][0]==orderedCycles[it+1][0]):
				finalCycle[-1][2]=orderedCycles[it+1][2]
				orderedCycles[it][2]=orderedCycles[it+1][2]
				orderedCycles=np.delete(orderedCycles,it+1,0)
			if it<(len(orderedCycles)-1) and (orderedCycles[it][2]==orderedCycles[it+1][1] and orderedCycles[it][0]!=orderedCycles[it+1][0]):
				finalCycle=np.vstack([finalCycle,orderedCycles[it+1]])
			if it<(len(orderedCycles)-1) and (orderedCycles[it][2]!=orderedCycles[it+1][1]):
				finalCycle=np.vstack([finalCycle,[7, finalCycle[-1][2],orderedCycles[it+1][1]]])
				finalCycle=np.vstack([finalCycle,orderedCycles[it+1]])
			if it==(len(orderedCycles)-1) and finalCycle[-1][2]!=endGCL:
				finalCycle=np.vstack([finalCycle,[7, finalCycle[-1][2], endGCL]])
			if it==(len(orderedCycles)-1):	
				gates = createGCL(finalCycle)
				writeJson(outputFile, gates)
			it+=1
		
		queue=[]
		cycleStart=[]
		cycleStop=[]
		linkPeriods=[]
		finalCycle=[]
		it=0
		queue.append(int(line[2]))
		cycleStart.append(int(line[3]))
		cycleStop.append(int(line[4]))
		linkPeriods.append(int(line[5]))
		prevLink=int(line[1])

outputFile.close()

with open("configuration.json", 'r') as infile, \
     open("config.json", 'w') as outfile:
	data = infile.read()
	data = data.replace("][", ",")
	outfile.write(data)



