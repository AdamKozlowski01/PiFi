/*********************************************
* ConnectionFactory.cpp
* Created By: Adam Kozlowski
* Last Updated: 8/6/2016
* Factory for creating different kinds of connections
*
*
*********************************************/

#include "ConnectionFactory.h"


void EchoConnection::run(){
	while(threadIndex < 0){
		threadIndex = deadPool->allocThread(connectionStart, this);
	}
	running = true;
}

EchoConnection::EchoConnection(int& fileDescrptr, ThreadPool* tp){ 
	this->connection_fd = fileDescrptr; 
	this->deadPool = tp;
	this->threadIndex = -1;

	run();
}

EchoConnection::~EchoConnection(){
	
}

void* EchoConnection::connectionProtocol(){
	char str[100];
	char quitStr[] = "end";

	while(true){
		bzero(str, 100);

		read(connection_fd, str, 100);

		if(!(strcmp(str, quitStr)))
			break;

		printf("Echoing back - %s", str);
		write(connection_fd, str, strlen(str)+1);
	}
	running = false;
	pthread_exit(NULL);
}

void EchoConnection::endConnection(){
	deadPool->freeThread(threadIndex);
	close(connection_fd);
}

Connection* EchoFactory::createConnection(int& fileDescrptr){
	Connection* connection = new EchoConnection(fileDescrptr, deadPool);
	return connection;
}