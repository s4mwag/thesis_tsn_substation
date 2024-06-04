import csv
import math
import os

other_traffic = "11110110"
previousTimeslot = 0
GCL = {}

# Calculate hyperperiods
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

# Read and sort Schedule.txt
schd=csv.reader(open("Schedule.txt"), delimiter=";")
sortSchd=sorted(schd, key=lambda x: int(x[3]))
sortSchd=sorted(sortSchd, key=lambda x: int(x[1]))

# Add open gates for gPTP and SV traffic
for i in sortSchd:
    if int(i[0]) > 5:
        i[5] = "00001000"
    else:
        i[5] = "00000001"

link = 7
j = 0

#file = open("results/sorted_schedule_by_link.txt", "w+")
#file.write(str(sortSchd))
#file.close()

# Calculate the GCL timeslots for each link and add corresponding gates
for i, openTimeslot in enumerate(sortSchd):
        if int(openTimeslot[1]) != link:  
            if (hyperperiod[int(link)] * 1000) > previousTimeslot:
                GCL[j] = {'link': str(link), 'timeslot': str((hyperperiod[int(link)] * 1000) - previousTimeslot) + 'ns', 'gate': other_traffic}
                j += 1
            previousTimeslot = 0
            link = int(openTimeslot[1])
        if int(openTimeslot[3]) != previousTimeslot: ## This is other traffic slot
            GCL[j] = {'link': openTimeslot[1], 'timeslot': str(int(openTimeslot[3]) - previousTimeslot) + 'ns', 'gate': other_traffic}
            j += 1
        
        GCL[j] = {'link': openTimeslot[1], 'timeslot': str(int(openTimeslot[4]) - int(openTimeslot[3])) + 'ns', 'gate': openTimeslot[5]}
        previousTimeslot = int(openTimeslot[4])
        j += 1

previous_link = '0'
timeslot = []
gate = []
f = open("results/GCL_for_omnet.txt", "w+")

link_to_interface = {8: [1, 0], 10: [1, 1], 12: [1, 2], 14: [1, 3], 16: [1, 4], 18: [1, 5], 20: [1, 6], 22: [1, 7],  ## Switch 1
                     24: [2, 0], 26: [2, 1], 28: [2, 2], 30: [2, 3], 32: [2, 4], 19: [2, 5], 33: [2, 6],             ## Switch 2
                     21: [3, 5], 34: [3, 6], 36: [3, 0], 38: [3, 1], 40: [3, 2], 42: [3, 3], 44: [3, 4]}             ## Switch 3

for i in GCL:
    if GCL[i]['link'] != previous_link and int(previous_link) in link_to_interface:
        
        data = "*.switch_" + str(link_to_interface[int(previous_link)][0]) + ".eth[" + str(link_to_interface[int(previous_link)][1]) + "].macLayer.queue.gateControlList.durations = " + repr(timeslot) + '\n'
        data = data.replace("'", "")
        data = data + "*.switch_" + str(link_to_interface[int(previous_link)][0]) + ".eth[" + str(link_to_interface[int(previous_link)][1]) + "].macLayer.queue.gateControlList.gateStates = " + repr(gate) + '\n'
        f.write(data)
        timeslot = []
        gate = []

    if int(GCL[i]['link']) in link_to_interface:
        timeslot.append(GCL[i]['timeslot'])
        gate.append(GCL[i]['gate'])
    
    previous_link = GCL[i]['link']

## For last link
timeslot = []
gate = []    
for key in GCL:
    if GCL[key]['link'] == str(list(link_to_interface)[-1]):
        timeslot.append(GCL[key]['timeslot'])
        gate.append(GCL[key]['gate'])
        last_link = int(GCL[key]['link'])
        
data = "*.switch_" + str(link_to_interface[last_link][0]) + ".eth[" + str(link_to_interface[last_link][1]) + "].macLayer.queue.gateControlList.durations = " + repr(timeslot) + '\n'
data = data.replace("'", "")
data = data + "*.switch_" +  str(link_to_interface[last_link][0]) + ".eth[" +  str(link_to_interface[last_link][1]) + "].macLayer.queue.gateControlList.gateStates = " + repr(gate) + '\n'
f.write(data)
f.close()
f = open("results/GCL_for_omnet.txt", "r")

f = open("results/GCL_for_omnet_sorted.txt", "w+")

with open('results/GCL_for_omnet.txt', 'r') as r:
    for line in sorted(r):
        f.write(line)
 

f.close()
os.remove("results/GCL_for_omnet.txt")

file_schedule = open('Schedule.txt', 'r')
schedule = file_schedule.readlines()

file_STHS = open('STHS_input.txt', 'r')
STHS_input = file_STHS.readlines()

file_AVB_input = open('AVB_Input.txt', 'w+')

message_ID = 0

for STHS_line in STHS_input:
    
    if STHS_line.startswith('length = '):
        length = int(STHS_line.split(' ')[2])
        
        for schedule_line in schedule:
            schedule_line_split = schedule_line.split(';')
            
            if int(schedule_line[0]) == message_ID:
                file_AVB_input.write('message\n')
                file_AVB_input.write('length = ' + str(length) + '\n')
                
                if int(schedule_line_split[1]) in hyperperiod:
                    file_AVB_input.write('period = ' + str(hyperperiod[int(schedule_line_split[1])]) + '\n')
                    file_AVB_input.write('deadline = ' + str(hyperperiod[int(schedule_line_split[1])]) + '\n')
                file_AVB_input.write('priority = ST\n')
                file_AVB_input.write('linkNbr = 1\n')
                file_AVB_input.write('link = ' + schedule_line_split[1] + '\n')
                file_AVB_input.write('jitterIn = 0\n')
                file_AVB_input.write('initOffset = 0\n')
                file_AVB_input.write('offset = ' + str((int(schedule_line_split[3]) / 1000)) + '\n')
                file_AVB_input.write('\n')


file_AVB_input.close()
print("Fin")