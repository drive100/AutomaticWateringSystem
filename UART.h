#pragma once
#include <string>
#include <unistd.h>
#include <iostream>
using namespace std;
class UART
{
private:
	int port;
	int baud;
public:
	UART(){}
	UART(int p, int b)
	{
		port = p;
		baud = b;
	}
	void send(string m);
	const char* recv();
};