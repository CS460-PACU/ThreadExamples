#############################################################################
# File name:  Makefile
# Author:     chadd williams
# Date:       Sept 1, 2021
# Class:      CS360
# Assignment: 
# Purpose:    
#############################################################################

TARGETS=bin/OneThread bin/TwoThreads_ParamRace bin/TwoThreads_Sync bin/CondWaitThread\
 bin/TwoThreads_NoSharedData bin/TwoThreads_WithSharedData\
 bin/TwoThreads_WithSharedData_Char bin/TwoThreads_WithSharedData_Int\
 bin/TwoThreads_WithSharedData.asm bin/TwoThreads_Sync.asm \
 bin/TwoThreads_WithSharedData_Int.asm \
 bin/TwoThreads_WithSharedData_Char.asm 

ENSCRIPT_FLAGS=-C -T 2 -p - -M Letter -Ec --color -fCourier8

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


bin/TwoThreads_WithSharedData_Char: bin src/TwoThreads_WithSharedData_Char.c
	gcc -o bin/TwoThreads_WithSharedData_Char -pthread src/TwoThreads_WithSharedData_Char.c -g -Wall

bin/TwoThreads_WithSharedData_Int: bin src/TwoThreads_WithSharedData_Int.c
	gcc -o bin/TwoThreads_WithSharedData_Int -pthread src/TwoThreads_WithSharedData_Int.c -g -Wall


bin/CondWaitThread: src/CondWaitThread.c
	gcc -o bin/CondWaitThread -pthread src/CondWaitThread.c -g -Wall

bin/TwoThreads_WithSharedData.asm: bin src/TwoThreads_WithSharedData.c
	gcc -o bin/TwoThreads_WithSharedData.asm -fverbose-asm -S src/TwoThreads_WithSharedData.c

bin/TwoThreads_Sync.asm: bin src/TwoThreads_Sync.c
	gcc -o bin/TwoThreads_Sync.asm -fverbose-asm -S src/TwoThreads_Sync.c

bin/TwoThreads_WithSharedData_Char.asm: bin src/TwoThreads_WithSharedData_Char.c
	gcc -o bin/TwoThreads_WithSharedData_Char.asm -fverbose-asm -S src/TwoThreads_WithSharedData_Char.c

bin/TwoThreads_WithSharedData_Int.asm: bin src/TwoThreads_WithSharedData_Int.c
	gcc -o bin/TwoThreads_WithSharedData_Int.asm -fverbose-asm -S src/TwoThreads_WithSharedData_Int.c


clean:
	rm -f ${TARGETS} bin/*

printIntExample:
	enscript ${ENSCRIPT_FLAGS} src/TwoThreads_WithSharedData_Int.c  | ps2pdf - bin/TwoThreads_WithSharedData_Int.pdf
