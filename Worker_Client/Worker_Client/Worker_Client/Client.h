#pragma once
#ifndef Client_h
#define Client_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <zmq.h>
#include <assert.h>
#include <string.h>
#include "md5.h"
#include <pthread.h>


#define PASSLENGTH 5
#define MD5LENGTH 32
#define ASCIISTART 32
#define ASCIIFINISH 126
#define ASCIIRANGE 94

int strtoi(const char *nptr);
BOOL MD5Compare(char *MD5Orig, char *MD5BruteForce);
char *MD5Hash(char *msg);
char ASCIIChar(int ASCIIValue);
BOOL MD5Bruteforce();
void recurse(int index);
void spawnThreads(int numOfThreads);
void splitWork(int totNumThreads);
void assignWorkPacketToThread(int start_char, int end_char, int threadNumber);
char ASCIIChar(int ASCIIValue);
void *thread_worker(void *thread_worker_void_ptr);

typedef struct workPacket {
	int numOfThreads;
	int passwordLength;
	char startPassword[PASSLENGTH + 1];
	int passwordCount; // ending ASCII range
	char targetHash[MD5LENGTH + 1];
	char passwordResult[PASSLENGTH + 1];
} workPacket;

typedef struct threadWorker {
	char startChar;
	char endChar;
} threadWorker;


#endif