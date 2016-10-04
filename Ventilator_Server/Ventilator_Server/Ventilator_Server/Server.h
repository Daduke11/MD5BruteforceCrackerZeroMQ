#pragma once
#ifndef Server_h
#define Server_h

#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define PASSLENGTH 5
#define MD5LENGTH 32
#define IPADDRESS 15
#define ASCIISTART 32
#define ASCIIFINISH 126
#define ASCIIRANGE 94

void readConfigFile(FILE *ptrFile);
void assignWorkPacketToSlave(int numOfThreads, char startPassword[PASSLENGTH + 1], int passwordCount, char targetHash[MD5LENGTH + 1], int slaveNumber);
void splitWork(int totNumSlaves);
char ASCIIChar(int ASCIIValue);

typedef struct {
	int numOfThreads;
	int passwordLength;
	char startPassword[PASSLENGTH + 1];
	int passwordCount; // ending ASCII range
	char targetHash[MD5LENGTH + 1];
	char serverIPaddress[IPADDRESS + 1];
} workPacket;

typedef struct {
	int totalNumberOfSlaves;
	char IPaddress[IPADDRESS + 1];
	int numberOfThreads;
	char targetHash[MD5LENGTH + 1];
} worker;


#endif