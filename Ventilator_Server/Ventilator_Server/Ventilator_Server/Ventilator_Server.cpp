#include "Ventilator_Server.h"
/*
int main(void)
{
	printf("Lab 4 Ventilator Server\n");

	void *context = zmq_ctx_new();
	// check context for non-NULL
	// we will push to this 
	// Socket to send messages on
	void *sendReceive = zmq_socket(context, ZMQ_REQ);
	// check sendReceive for non-NULL
	zmq_connect(sendReceive, "tcp://127.0.0.1:5557");
	//if (sendReceive != NULL && zmq_bind(sendReceive, "tcp://*:5557") == 0) {
	//	printf("bind successful");
	//}
	//else {
	//	printf("bind unsuccessful");
	//}


	// check that it is 0 //0 means successful -1 is bad

	//  Socket to send start of batch message on
	// we will also push here
	//void *sink = zmq_socket(context, ZMQ_PUSH);
	//zmq_connect(sink, "tcp://127.0.0.1:5558");
	
	if (sink != NULL && zmq_connect(sink, "tcp://localhost:5558") == 0) {
	printf("sink successful");
	}
	else {
	printf("sink unsuccessful");
	}
	


	//zmq_connect(sink, "tcp://localhost:5558");
	// check that it returns 0, 0 means successful

	//wait here until the workers are ready and listening

	//send first work packet zmq_send(<socket>, "string/data", <number of char's / data length>, <default Send flags> )
	printf("Press Enter when the workers are ready: ");
	getchar();
	printf("Sending tasks to workers...\n");
	char string[12];
	//sprintf_s(string, "%s", "Hello_World");
	struct workPacket *ptrMsgPacket;
	//	new struct workPacket;
	//msgPacket->numOfThreads = 1;
	//msgPacket->passwordLength = 5;
	//msgPacket->startPassword = "     ";
	struct workPacket workPackage{
		1,
		5,
		"     ",
		50,
		"asdf"
	};

	ptrMsgPacket = &workPackage;
	zmq_msg_t
	char *buffer = (char*)ptrMsgPacket;
	zmq_send(sendReceive, buffer, sizeof(workPackage),0);
	
	
	int *ptrThreadCount;
	int threadCount = 1;
	ptrThreadCount = &threadCount;
	
	zmq_msg_t numOfThreads;
	zmq_msg_init_size(&numOfThreads, 1);
	memcpy(zmq_msg_data(&numOfThreads), "1", 1);




	//{ 1, 5, "     ", 50, "a5987" };

	//zmq_send(sendReceive, "hello_world", 12, 0);
	//s_send(sendReceive, string);
	//printf("%s", string);
	//zmq_send(sendReceive, "hello_world", 11, 0);
	/*if (zmq_send(sink, "hello_world", 11, 0) == 0) {
	printf("sink successful");
	}
	else {
	printf("sink unsuccessful");
	}
	//check for 0;
	
	//... send more works ....

	// Sink part: wait for and retrieve work results

	//void *reciever = zmq_socket(context, ZMQ_PULL);
	//zmq_bind(reciever, "tcp://*:5559");
	
	if (reciever != NULL && zmq_bind(reciever, "tcp://localhost:5559") == 0) {
	printf("reciever bind successful");
	}
	else {
	printf("reciever bind unsuccessful");
	}
	
	// check for null
	//zmq_bind(reciever, "tcp://localhost:5559");
	// assert 0

	// buffer to get work signals
	char *string2 = s_recv(sendReceive);
	
	//zmq_recv(reciever, buffer, 11, 0);
	//zmq_recv( <socket>, <dest for data>, <buffer size>, <flags: 0 use default flags> )
	/*if (zmq_recv(reciever, buffer, 11, 0) == 0) {
	printf("zmq reciever successful");
	}
	else {
	printf("zmq reciever unsuccessful");
	}//buffer gets passed as pointer
	
	// this will lock
	// assert 0
	// result wil be in buffer

	// close everything off and quit
	// close all sockets
	printf("Buffer reciever: %s", string2);
	//zmq_close(reciever);

	printf("Press Enter when the workers are ready: ");
	getchar();
	zmq_close(sendReceive);

	// close zmq
	zmq_ctx_destroy(context);

	return 0;
}

*/