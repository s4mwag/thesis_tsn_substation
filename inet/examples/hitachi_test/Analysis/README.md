Here are the instructions:
1. Create a folder to execute the tool, called Experiment<ID> (you can create a new one so you always have Experiment74 as reference)
2. Define the topology numerating the links in each direction —> full-duplex = 2-unidirectional-links
3. Inside the folder, generate the ST traffic file, named STHS_input.txt . Offset is always 0 for TT traffic.
4. Execute TSN_HeuristicScheduler.exe (which is outside the folder) with three parameters: <ID> STHS_input.txt <num_queues> (the ID is the new folder number, and we can use one queue).
5. Create a parser and use it to translate the TT schedule (Schedule.txt) into the AVB analysis tool input (AVB_input.txt). Remember that in order to create the AVB_input.txt information, you will need to calculate the hyperperiod of each link, and then also read the frame size from the STHS_input.txt file. This will result in a partial AVB traffic file that will contain the TT traffic. 
6. Add the AVB traffic to the AVB_input.txt file from the previous step.
7. Add the utilization per link in the analysis.h file and modify the analysis.c to read that info (I can help with this). Compile and obtain the AVB_analysis.exe.
8. Execute AVB_analysis.exe with parameter <ID>.
9. Inside the folder: we will find the result in the file final_result.txt -> we need to compare columns 3 (deadline) and 4 (worst case RT). If 3  is smaller than 4, then it is not schedulable.
 
**FOR THE IMPLEMENTATION ONLY (NOT RELATED TO THE ANALYSIS):**
You can use the schd-parser.py file to parse the TT schedule. If it doesn’t work, you might need to provide the absolute path to the files instead of the relative one. You will also see that the file that it creates has more info than you need. You can choose to change it, or just copy paste what you need.