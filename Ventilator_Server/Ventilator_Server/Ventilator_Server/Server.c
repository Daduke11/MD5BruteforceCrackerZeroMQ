/*

Request - Reply

Send work packages in multiple-parts.

server.c

*/

#include "Server.h"

static worker slaveInformation[25];
static workPacket slaveWorkPacket[25];

int main(int argc, char const *argv[]) {

	void* context = zmq_ctx_new();
	void* respond = zmq_socket(context, ZMQ_REQ); //needs to be rep need to send dummy data from req first
	char src[6], dest[27];
	//zmq_bind(respond, "tcp://127.000.000.001:5558");
	errno_t err;
	// read in file 
	FILE *ptrDataIn;

	// Open for read (will fail if file "crt_fopen_s.c" does not exist)
	err = fopen_s(&ptrDataIn, "config.txt", "r");
	if (err == 0)
	{
		printf("The file 'config.txt' was opened\n");
	}
	else
	{
		printf("The file 'config.txt' was not opened\n");
	}

	// read config.txt process into slaveInformation
	readConfigFile(ptrDataIn);
	int totNumSlaves = slaveInformation[0].totalNumberOfSlaves;
	
	// split work among total number of physical slave machines
	splitWork(totNumSlaves);


	printf("Press Enter when the workers are ready: ");
	getchar();
	printf("Starting...\n");
	int rc;
	
	strcpy_s(src, 6,":5558");
	
	// send work packets to each slave
	for (int i = 0; i < totNumSlaves; i++) {
		strcpy_s(dest, 7,"tcp://");
		strncat_s(dest, sizeof(dest), slaveInformation[i].IPaddress, 
			15);// sizeof(slaveInformation[i].IPaddress)
		strncat_s(dest, sizeof(dest), src, sizeof(src));
		printf("this is dest address %s, physical slave number %d\n", dest, i);
		zmq_bind(respond, dest);
		zmq_msg_t msg_1_numOfThreads, msg_2_passwordLength, msg_3_startPassword, msg_4_passwordCount, msg_5_targetHash;
		
		// numberOfThreads into a char
		int size = 1;
		size = sizeof(slaveInformation[i].numberOfThreads);
		char *bufferNoOfThreads = malloc(sizeof(char)*size);
		_itoa_s(slaveInformation[i].numberOfThreads, bufferNoOfThreads, sizeof(bufferNoOfThreads), 10);
		
		// passwordCount into a char
		int passCountSize = 1;
		passCountSize = sizeof(slaveWorkPacket[i].passwordCount);
		char *bufferPasswordCount = malloc(sizeof(char)*passCountSize);
		_itoa_s(slaveWorkPacket[i].passwordCount, bufferPasswordCount, sizeof(bufferPasswordCount), 10);


		// initialize message size
		rc = zmq_msg_init_size(&msg_1_numOfThreads, size+1);
		assert(rc == 0);
		rc = zmq_msg_init_size(&msg_2_passwordLength, 2);
		assert(rc == 0);
		rc = zmq_msg_init_size(&msg_3_startPassword, PASSLENGTH + 1);
		assert(rc == 0);
		rc = zmq_msg_init_size(&msg_4_passwordCount, 3);
		assert(rc == 0);
		rc = zmq_msg_init_size(&msg_5_targetHash, MD5LENGTH + 1);
		assert(rc == 0);
		
		
		// memcopy info to send and send the message
		memcpy(zmq_msg_data(&msg_1_numOfThreads), bufferNoOfThreads, size+1);
		rc = zmq_msg_send(&msg_1_numOfThreads, respond, ZMQ_SNDMORE);
		assert(rc >= 0);  // die if length is negative.

		memcpy(zmq_msg_data(&msg_2_passwordLength), "5", 2);
		rc = zmq_msg_send(&msg_2_passwordLength, respond, ZMQ_SNDMORE);
		assert(rc >= 0);  // die if length is negative.

		memcpy(zmq_msg_data(&msg_3_startPassword), slaveWorkPacket[i].startPassword, PASSLENGTH + 1);
		rc = zmq_msg_send(&msg_3_startPassword, respond, ZMQ_SNDMORE);
		assert(rc >= 0);  // die if length is negative.
		
		// need to calc password count
		memcpy(zmq_msg_data(&msg_4_passwordCount), bufferPasswordCount, strlen(bufferPasswordCount)+1);
		rc = zmq_msg_send(&msg_4_passwordCount, respond, ZMQ_SNDMORE);
		assert(rc >= 0);  // die if length is negative.

		memcpy(zmq_msg_data(&msg_5_targetHash), slaveInformation[i].targetHash, MD5LENGTH + 1);
		zmq_msg_send(&msg_5_targetHash, respond, 0);
		system("pause");
		/*

		struct workPacket workPackage{
		1,
		5,
		"     ",
		50,
		"asdf"
		};

		zmq_msg_t request;
		zmq_msg_init(&request);
		zmq_msg_recv(&request, respond, 0);
		printf("Received: hello\n");
		
		zmq_msg_close(&request);
		
		sleep(1);
		*/
		// Close stream if it is not NULL 
		if (ptrDataIn)
		{
			err = fclose(ptrDataIn);
			if (err == 0)
			{
				printf("The file 'config.txt' was closed\n");
			}
			else
			{
				printf("The file 'config.txt' was not closed\n");
			}
		}
		// close MSG's
		zmq_msg_close(&msg_1_numOfThreads);
		zmq_msg_close(&msg_2_passwordLength);
		zmq_msg_close(&msg_3_startPassword);
		zmq_msg_close(&msg_4_passwordCount);
		zmq_msg_close(&msg_5_targetHash);
	}

	zmq_close(respond);
	zmq_ctx_destroy(context);

	return 0;
}

/*split work among physical slaves*/
void splitWork(int totNumSlaves) {
	
	int workerSplit = ASCIIRANGE / totNumSlaves;
	int passwordCount= ASCIIRANGE;
	int currentStartingChar = ASCIISTART;
	char startPassword[6];
	
		if (totNumSlaves == 1) {
			assignWorkPacketToSlave(
				slaveInformation[0].numberOfThreads,
				" sdf ",
				passwordCount,
				slaveInformation[0].targetHash,
				0);
		}
		else {
			
			for (int i = 0; i < totNumSlaves; i++) {
				// calc ASCII range for work packet
				
				char startPassFirstChar[2] = { ASCIIChar(currentStartingChar) };
				strcpy_s(startPassword, 2, startPassFirstChar);
				strncat_s(startPassword, sizeof(startPassword), "sdf ", 4);

				passwordCount = currentStartingChar + workerSplit;

				// offset by 1 to counter the chopping off decimal
				if (passwordCount < ASCIIFINISH) {
					passwordCount += 1;
				}
				// catch if over ASCIIFINISH or less than ASCIIFINISH && == final slave
				else if (passwordCount > ASCIIFINISH || (passwordCount < ASCIIFINISH && i == totNumSlaves)) {
					passwordCount = ASCIIFINISH;
				}

				assignWorkPacketToSlave(
					slaveInformation[i].numberOfThreads,
					startPassword,
					passwordCount, // ending ASCII range
					slaveInformation[i].targetHash,
					i);

				printf("worker split %d\nslave number %d	starting ASCII int %d	ending ASCII int %d\n",
					workerSplit, i, currentStartingChar, passwordCount);
				currentStartingChar = passwordCount + 1;

				
			}
			
	}
}

/*assign workpackets to slaves*/
void assignWorkPacketToSlave( int numOfThreads, char startPassword[PASSLENGTH + 1], int passwordCount, char targetHash[MD5LENGTH + 1],  int slaveNumber  ) {
	slaveWorkPacket[slaveNumber].numOfThreads = numOfThreads;
	slaveWorkPacket[slaveNumber].passwordLength = 5;
	strcpy_s(slaveWorkPacket[slaveNumber].startPassword, PASSLENGTH + 1, startPassword);
	slaveWorkPacket[slaveNumber].passwordCount = passwordCount;
	strcpy_s(slaveWorkPacket[slaveNumber].targetHash, MD5LENGTH + 1, targetHash);
}

char ASCIIChar(int ASCIIValue) {
	char character = ASCIIValue;
	return character;
}

/* read config file */
void readConfigFile(FILE *ptrFile) {
	// exit if cannot be opened
	int n = 0; // counter
	if (ptrFile == NULL) {
		printf("File could not be opened\n");
	}
	else {
		while (!feof(ptrFile)) {
			fscanf_s(ptrFile, "%d %s %d %s",
				&slaveInformation[n].totalNumberOfSlaves, 
				slaveInformation[n].IPaddress,
				(int)sizeof(slaveInformation[n].IPaddress),
				&slaveInformation[n].numberOfThreads,
				slaveInformation[n].targetHash,
				(int)sizeof(slaveInformation[n].targetHash)
				);
			n++;
		}
	}
}