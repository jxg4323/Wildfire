#!/bin/bash
#Experiment 1: Display the help message to show command line options
#Experiment 2: Show the amount of generations for a forest fire without wind and compare it to experiment 3
#Experiment 3: add a wind value of 3 coming from the west to the preivous experiment to show that the wind will shorten generations
./wildfire help 2> experiment1.txt
./wildfire 9 5 60 89 50 > experiment2.txt
./wildfire 10 5 60 89 50 3 W > experiment3.txt
#windgens=$(tail -1 experiment3.txt | head -1)
#nowindgens=$(tail -1 experiment2.txt | head -1)
