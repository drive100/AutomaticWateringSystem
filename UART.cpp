
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "UART.h"
using namespace std;

void UART::send(string message)
{
	ofstream art;
	art.open("/dev/ttyAMA0");
	art << message << endl;;
	art.close();

}

const char* UART::recv()
{

	ifstream art2;
	string m;
	art2.open("/dev/ttyAMA0");
	art2 >> m;
	//m.c_str();
	const char * mess = m.c_str();
	cout << mess << endl;
	art2.close();
	return mess;
	
}