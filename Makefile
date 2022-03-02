#############################################################################
# File name:  Makefile
# Author:     chadd williams
# Date:       Sept 1, 2021
# Class:      CS360
# Assignment: 
# Purpose:    
#############################################################################

TARGETS=bin/OneThread bin/TwoThreads_ParamRace bin/TwoThreads_Sync bin/CondWaitThread bin/TwoThreads_NoSharedData bin/TwoThreads_WithSharedData

all: ${TARGETS}

bin:
	mkdir -p bin

bin/OneThread: bin bin/OneThread.o
	gcc -o bin/OneThread -pthread bin/OneThread.o -g -Wall

bin/OneThread.o: src/OneThread.c
	gcc -o bin/OneThread.o -pthread src/OneThread.c -c -g -Wall


bin/TwoThreads_Sync: bin src/TwoThreads_Sync.c
	gcc -o bin/TwoThreads_Sync -pthread src/TwoThreads_Sync.c -g -Wall
	
bin/TwoThreads_ParamRace: bin src/TwoThreads_ParamRace.c
	gcc -o bin/TwoThreads_ParamRace -pthread src/TwoThreads_ParamRace.c -g -Wall -std=c99 


bin/TwoThreads_NoSharedData: bin src/TwoThreads_NoSharedData.c
	gcc -o bin/TwoThreads_NoSharedData -pthread src/TwoThreads_NoSharedData.c -g -Wall

bin/TwoThreads_WithSharedData: bin src/TwoThreads_WithSharedData.c
	gcc -o bin/TwoThreads_WithSharedData -pthread src/TwoThreads_WithSharedData.c -g -Wall


bin/CondWaitThread: src/CondWaitThread.c
	gcc -o bin/CondWaitThread -pthread src/CondWaitThread.c -g -Wall

clean:
	rm -f ${TARGETS} bin/*
