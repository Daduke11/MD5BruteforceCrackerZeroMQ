/*

Request - Reply

Receive client work package in multi-parts.

client.c

*/


#include "Client.h"

static threadWorker threadWorkPacket[25];
static workPacket workPackage;
static int result = 0;
static const char min_char = ' ', max_char = '~';
int main(int argc, char const *argv[]) {
	
	char md5Check[33];
	int rc;
	/*char *MD5example = MD5Hash("asdf");
	printf("MD5 returned string %s", MD5example);
	
	printf("result = %d\n", MD5Compare("912ec803b2ce49e4a541068d495ab570", MD5example));
	 
	free(MD5example);
	*/
	void* context = zmq_ctx_new();

	printf("Client Starting....\n");

	void* request = zmq_socket(context, ZMQ_REP); // needs to be req, thou shall only have one REP, many REQ's, REQ's must initiate to REP
	zmq_connect(request, "tcp://127.0.0.1:5558");
	//char *ptrMD5 = MD5Hash("asdf ");
	//strcpy_s(workPackage.targetHash, sizeof(workPackage.targetHash), "912ec803b2ce49e4a541068d495ab570");
	//strcpy_s(workPackage.targetHash, sizeof(workPackage.targetHash), ptrMD5);
	//strcpy_s(workPackage.startPassword, sizeof(workPackage.startPassword), " sdf");
	//workPackage.passwordLength = 5;

	//recieve work package - multipart messages and store in the workPackage Struct
	for (int count = 0; count < 5; count++) {

		zmq_msg_t reply;
		rc = zmq_msg_init(&reply);
		assert(rc == 0);
		rc = zmq_msg_recv(&reply, request, 0);
		assert(rc >= 0);  // die if length is negative.
		printf("Received: %s, size %d\n", (char *)zmq_msg_data(&reply), (int)zmq_msg_size(&reply));
		if (count == 0) {
			workPackage.numOfThreads = strtoi((char *)zmq_msg_data(&reply));
			printf("numOfThreads %d\n", workPackage.numOfThreads);
		}
		if (count == 1) {
			workPackage.passwordLength = strtoi((char *)zmq_msg_data(&reply));
			printf("passLength %d\n", workPackage.passwordLength);
		}
		if (count == 2) {
			char *ptrStartPassword = (char *)zmq_msg_data(&reply);
			strncpy_s(workPackage.startPassword, sizeof(workPackage.startPassword), ptrStartPassword, zmq_msg_size(&reply));
			printf("startPassword %s\n", workPackage.startPassword);
		}
		if (count == 3) {
			workPackage.passwordCount = strtoi((char *)zmq_msg_data(&reply));
			printf("passwordCount %d\n", workPackage.passwordCount);
		}
		if (count == 4) {
			char *ptrTargethash = (char *)zmq_msg_data(&reply);
			strncpy_s(workPackage.targetHash, sizeof(workPackage.targetHash), ptrTargethash, zmq_msg_size(&reply));
			printf("targetHash %s\n", workPackage.targetHash);
		}
		zmq_msg_close(&reply);

		uint64_t more_part;
		size_t more_size = sizeof(more_part);
		zmq_getsockopt(request, ZMQ_RCVMORE, &more_part, &more_size);		

		if (!more_part)
			break;
	}
	//splitWork(workPackage.numOfThreads);
	//spawnThreads(workPackage.numOfThreads);
	int result = MD5Bruteforce();

	//recurse(0);
	printf("\n");
	printf("result %s\n", workPackage.passwordResult);
	system("pause");
	//system("pause");

	// do the work
	//if (MD5Bruteforce()) {
	//	printf("password result %s", workPackage.passwordResult);
	//}

	zmq_close(request);
	zmq_ctx_destroy(context);

	return 0;
}

// Over-simplified use http://stackoverflow.com/questions/3420629/convert-string-to-integer-sscanf-or-atoi
int strtoi(const char *nptr) {
	int i = (int)strtol(nptr, (char **)NULL, 10);
	return i;
}

BOOL MD5Bruteforce() {
	// take in min_char, max_char for index[0]
	//const char min_char = ' ', max_char = '~';
	int len = workPackage.passwordLength;
	char *ptrMD5temp = malloc(PASSLENGTH+1 * sizeof(char));
	strcpy_s(ptrMD5temp, sizeof(ptrMD5temp), workPackage.startPassword);
		
	/*
	//recurse(0);
	if (result) {
		return TRUE;
	}
	*/
	//int index;
	/*
	for (index = 0; index < len && workPackage.startPassword[index] <= max_char-1; index++) {
		printf("current string %s", workPackage.startPassword);
		if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
			strcpy_s(workPackage.passwordResult,
				sizeof(workPackage.passwordResult),
				workPackage.startPassword);
			return TRUE;
		}
		for (index--; index >= 0; index--) {
			workPackage.startPassword[index] = min_char;
			printf("current string %s", workPackage.startPassword);
			if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
				strcpy_s(workPackage.passwordResult,
					sizeof(workPackage.passwordResult),
					workPackage.startPassword);
				return TRUE;
			}
		}
	}
	*/

	int index;
	for (index = 0; index < workPackage.passwordLength; index++) {
		for (int charRange = 32; charRange <= 125; charRange++) {
			workPackage.startPassword[index]++;
			printf("current string %s", workPackage.startPassword);
			if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
				strcpy_s(workPackage.passwordResult,
					sizeof(workPackage.passwordResult),
					workPackage.startPassword);
				return TRUE;
			}
			if (index == 1) {
				workPackage.startPassword[0] = min_char;
				for (int charRange = 32; charRange <= 125; charRange++) {
					workPackage.startPassword[0]++;
					printf("current string %s", workPackage.startPassword);
					if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
						strcpy_s(workPackage.passwordResult,
							sizeof(workPackage.passwordResult),
							workPackage.startPassword);
						return TRUE;
					}
				}
			}
			if (index == 2) {
				workPackage.startPassword[1] = min_char;
				for (int charRange = 32; charRange <= 125; charRange++) {
					workPackage.startPassword[1]++;
					printf("current string %s", workPackage.startPassword);
					if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
						strcpy_s(workPackage.passwordResult,
							sizeof(workPackage.passwordResult),
							workPackage.startPassword);
						return TRUE;
					}
					workPackage.startPassword[0] = min_char;
					for (int charRange = 32; charRange <= 125; charRange++) {
						workPackage.startPassword[0]++;
						printf("current string %s", workPackage.startPassword);
						if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
							strcpy_s(workPackage.passwordResult,
								sizeof(workPackage.passwordResult),
								workPackage.startPassword);
							return TRUE;
						}
					}
				}
			}
			if (index == 3) {
				workPackage.startPassword[2] = min_char;
				for (int charRange = 32; charRange <= 125; charRange++) {
					workPackage.startPassword[2]++;
					//printf("current string %s", workPackage.startPassword);
					if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
						strcpy_s(workPackage.passwordResult,
							sizeof(workPackage.passwordResult),
							workPackage.startPassword);
						return TRUE;
					}
					workPackage.startPassword[1] = min_char;
					for (int charRange = 32; charRange <= 125; charRange++) {
						workPackage.startPassword[1]++;
						//printf("current string %s", workPackage.startPassword);
						if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
							strcpy_s(workPackage.passwordResult,
								sizeof(workPackage.passwordResult),
								workPackage.startPassword);
							return TRUE;
						}
						workPackage.startPassword[0] = min_char;
						for (int charRange = 32; charRange <= 125; charRange++) {
							workPackage.startPassword[0]++;
							//printf("current string %s", workPackage.startPassword);
							if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
								strcpy_s(workPackage.passwordResult,
									sizeof(workPackage.passwordResult),
									workPackage.startPassword);
								return TRUE;
							}
						}
					}
				}
			}
			if (index == 4) {
				workPackage.startPassword[3] = min_char;
				for (int charRange = 32; charRange <= 125; charRange++) {
					workPackage.startPassword[3]++;
					//printf("current string %s", workPackage.startPassword);
					if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
						strcpy_s(workPackage.passwordResult,
							sizeof(workPackage.passwordResult),
							workPackage.startPassword);
						return TRUE;
					}
					workPackage.startPassword[2] = min_char;
					for (int charRange = 32; charRange <= 125; charRange++) {
						workPackage.startPassword[2]++;
						//printf("current string %s", workPackage.startPassword);
						if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
							strcpy_s(workPackage.passwordResult,
								sizeof(workPackage.passwordResult),
								workPackage.startPassword);
							return TRUE;
						}
						workPackage.startPassword[1] = min_char;
						for (int charRange = 32; charRange <= 125; charRange++) {
							workPackage.startPassword[1]++;
							//printf("current string %s", workPackage.startPassword);
							if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
								strcpy_s(workPackage.passwordResult,
									sizeof(workPackage.passwordResult),
									workPackage.startPassword);
								return TRUE;
							}
							workPackage.startPassword[0] = min_char;
							for (int charRange = 32; charRange <= 125; charRange++) {
								workPackage.startPassword[0]++;
								//printf("current string %s", workPackage.startPassword);
								if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
									strcpy_s(workPackage.passwordResult,
										sizeof(workPackage.passwordResult),
										workPackage.startPassword);
									return TRUE;
								}
							}
						}
					}
				}
			}
		}

		//for (index--; index = 0; index--) {
			//workPackage.startPassword[index] = min_char;
			/*
			for (int charRange = 32; charRange <= 125; charRange++) {
				workPackage.startPassword[index]++;
				printf("current string %s", workPackage.startPassword);
				if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
					strcpy_s(workPackage.passwordResult,
						sizeof(workPackage.passwordResult),
						workPackage.startPassword);
					return TRUE;
				}
			}
			*/
		//}
	}

	free(ptrMD5temp);
	return FALSE;
}

void recurse(int index) {
	if (workPackage.startPassword[index] < max_char) {
		workPackage.startPassword[index]++;
		//printf("current string inside if %s", workPackage.startPassword);
		//printf("current string %s", workPackage.startPassword);
	}
	else {
		//printf("current string inside else %s", workPackage.startPassword);
		workPackage.startPassword[index] = min_char;
		recurse(index - 1);
		//recurse(index - 1);
	}
	if (MD5Compare(workPackage.targetHash, MD5Hash(workPackage.startPassword))) {
		strcpy_s(workPackage.passwordResult,
			sizeof(workPackage.passwordResult),
			workPackage.startPassword);
		result = 1;
	}
	recurse(0);

}

char ASCIIChar(int ASCIIValue) {
	char character = ASCIIValue;
	return character;
}

char *MD5Hash(char *msg) {

	int j, k;
	//const char *msg = "The quick brown fox jumps over the lazy dog.";
	char *ptrMD5BruteForce = malloc( 33 * sizeof(char));
	//char MD5BruteForce[33];
	//ptrMD5BruteForce = MD5BruteForce;
	unsigned *d = md5(msg, strlen(msg));
	WBunion u;
	int length = 0;
//	printf("= ");
	for (j = 0; j<4; j++) {
		u.w = d[j];
		for (k = 0; k < 4; k++) {
			//printf("	%02x\n", u.b[k]);
			length += snprintf((char*)ptrMD5BruteForce + length, 33 - length, "%02x", u.b[k]);
		}
	}
//	printf("\n");
	printf("	%s\n", ptrMD5BruteForce);

	return ptrMD5BruteForce;
	free(ptrMD5BruteForce);
}

/*Compare two MD5 strings*/
BOOL MD5Compare(char *MD5Orig, char *MD5BruteForce ) {
	int i = 0;
	if (strncmp(MD5Orig, MD5BruteForce, MD5LENGTH) == 0) {
		return TRUE;
	}
	return FALSE;
}

void spawnThreads(int numOfThreads) {
	// send min char, max char for index[0] to each thread

	/* this variable is our reference to the thread */
	pthread_t thread1;
/*
	char *thread[8] = "thread";
	for (int i = 1; i < numOfThreads; i++) {
		thread[7] = i;
		pthread_t thread;
	}
*/
	char range[3] = { ' ', '~' };
	/*
	// create a second thread which executes thread_worker(&range) 
	if (pthread_create(&thread1, NULL, thread_worker, &range)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;

	}

	// execute first thread work
	printf("first thread work finished\n");

	// wait for the second thread to finish 
	if (pthread_join(thread1, NULL)) {

		fprintf(stderr, "Error joining thread\n");
		return 2;

	}
	*/


}

void *thread_worker(void *thread_worker_void_ptr)
{

	/* increment x to 100 */
	/* Poo bear wants his honey ! DO WORK IN HERE 
	int *x_ptr = (int *)thread_worker_void_ptr;
	while (++(*x_ptr) < 100);

	printf("x increment finished\n");
	*/
	/* the function must return something - NULL will do */
	return NULL;

}

/*split work into multiple thread work packages*/
void splitWork(int totNumThreads) {

	int workerSplit = workPackage.passwordCount / totNumThreads;
	int passwordCount = ASCIISTART;
	int currentStartingChar = (int)workPackage.startPassword[0];
	int currentEndingChar = (int)workPackage.startPassword[0] + workPackage.passwordCount;
	char startPassword[6];

	if (totNumThreads == 1) {
		assignWorkPacketToThread(ASCIISTART, ASCIIFINISH, 0);
	}
	else {

		for (int i = 0; i < totNumThreads; i++) {
			// calc ASCII range for work packet
			passwordCount = currentStartingChar + workerSplit;

			// offset by 1 to counter the chopping off decimal
			if (passwordCount < currentEndingChar) {
				passwordCount += 1;
			}
			// catch if over currentEndingChar or less than currentEndingChar && == final slave
			else if (passwordCount > currentEndingChar || (passwordCount < currentEndingChar && i == totNumThreads)) {
				passwordCount = currentEndingChar;
			}

			assignWorkPacketToThread(currentStartingChar, passwordCount, i);

			printf("thread split %d\nthread number %d	starting ASCII int %d	ending ASCII int %d\n",
				workerSplit, i, currentStartingChar, passwordCount);
			currentStartingChar = passwordCount + 1;
		}

	}
}

void assignWorkPacketToThread(int start_char, int end_char, int threadNumber) {
	threadWorkPacket[threadNumber].startChar = start_char;
	threadWorkPacket[threadNumber].endChar = end_char;
}
