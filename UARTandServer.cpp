#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UART.h"
#include "UART.cpp"
#include "GPIO.h"
#include "GPIO.cpp"
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

using namespace std;

UART u1(1, 9600);
GPIO p21("0", "in", "21");
GPIO p20("0", "in", "20");
string sensor1 = "";
string sensor2 = "";
bool programOn = true;

void error( char *msg ) {
	perror(  msg );
	exit(-1);
}

void sendData( int sockfd, string x) {
	char buffer[1024];
	int n;
	sprintf(buffer, "%s\n", x.c_str());
	if ( (n = send( sockfd, buffer, strlen(buffer), 0 ) ) < 0 )
		error( const_cast<char *>( "ERROR writing to socket") );
    //buffer[n] = '\0';
}

string getData( int sockfd ) {
	char buffer[1024];
	const int result = recv(sockfd, buffer, sizeof(buffer), MSG_DONTWAIT);
	if (result < 0)
	{
		//perror("recv error: ");
		return "";
	}
	else if (result == 0)
	{
		return "0";
	}
	buffer[result] = '\0';
    string s ="";
    for (int i = 0; i < result; i++)
    {
    	s += (char)buffer[i];
    }
    cout << "getData cout: " << s <<" size: " << s.size() << endl;
    return s;
}

void *Server(void *x)
{
	int sockfd, newsockfd, portno = 8133, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	string data1 = "";

	printf( "using port #%d\n", portno );

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error( const_cast<char *>("ERROR opening socket") );
	bzero((char *) &serv_addr, sizeof(serv_addr));
	bool enable = true;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("10.1.93.45");
	serv_addr.sin_port = htons( portno );
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0) 
		error( const_cast<char *>( "ERROR on binding" ) );
	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags < 0) 
	{
		perror("flags");
		close(sockfd);
		exit(-1);

	}
	if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		perror("fcntl");
		close(sockfd);
		exit(-1);
	}
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	//int modes = 1;
	bool running = false;
//--- infinite wait on a connection ---
	while(1)
	{
		printf( "waiting for new client...\n" );
		if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
		{
			sleep(1);
			continue;
		}
		running = true;
		while ( running == true ) 
		{
//---- wait for a number from client ---
			data1 = getData( newsockfd );
			//data1 = "nothing";
			cout  << "got: " << data1 << endl;
			if ( data1 == "0" ) 
			{
				running = false;
				break;
			}
 //--- send new data back --- 
			else if (data1 == "11")
			{
				for(int i = 0; i < 3; i++)
				u1.send("11");
			}
			else if (data1 == "10")
			{
				for(int i = 0; i < 3; i++)
				u1.send("10");
			}
			else if (data1 == "80")
			{
				for(int i = 0; i < 3; i++)
				u1.send("80");
			}
			else if (data1 == "90")
			{
				for(int i = 0; i < 3; i++)
				u1.send("90");
			}

			sendData(newsockfd, sensor1);
			cout << "sensor1 from thread: " << sensor1 << endl;
			//sendData(newsockfd, sensor2);
			//cout << "sensor2: " << sensor2 << endl;
			usleep(500000);
			if ( data1 == "-2" )
				break;
		}
		close(newsockfd);
	}
	return 0; 
}

int main(){
	
	int x = 1;
	string d;
	string f;
	int length;

	GPIO p16("0", "in", "16");
	pthread_t serverThread;
	for (int i = 0; i < 5; i++)
	{
		f = u1.recv();
		cout << f << endl;
		if (f.at(0) == '1')
		{
			f.erase(f.begin());
			cout << "Sensor1: ";
			cout << f << endl;
			sensor1 = f;
		}
		else if (f.at(0) == '2')
		{
			f.erase(f.begin());
			cout << "Sensor2: ";
			cout << f << endl;
			sensor2 = f;
		}
	}
	//create server thread to accept client and send data
	pthread_create(&serverThread, NULL, Server, (void *) x);

	while (1)
	{
		if (p21.getvalue() == "1")
			u1.send("11");
		if (p20.getvalue() == "1")
			u1.send("10");

		f = u1.recv();
		sensor1 = f;
		if (f.at(0) == '!')
			f.erase(f.begin());
		// else if (f.at(0) == '*')
		// 	f.erase(f.begin());
		// else if (f.at(0) == '?')
		// 	f.erase(f.begin());
		
		if (f.at(0) == '1')
		{
			f.erase(f.begin());
			cout << "Sensor1: ";
			cout << f << endl;
			//sensor1 = f;
		}
		else if (f.at(0) == '2')
		{
			f.erase(f.begin());
			cout << "Sensor2: ";
			cout << f << endl;
			//sensor2 = f;
		}
	}

}
