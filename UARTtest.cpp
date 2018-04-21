#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "UART.h"
#include "UART.cpp"
#include "GPIO.h"
#include "GPIO.cpp"
using namespace std;

int main(){
	UART u1(1, 9600);
	GPIO p21("0", "in", "21");
	GPIO p20("0", "in", "20");
	int i = 1;
	string d;
	string f;
	int length;
	for (int i = 0; i < 5; i++)
	{
		f = u1.recv();
		cout << f << endl;
	}
	while (1)
	{
		//cin >> d;
		//u1.send(d);
		// cout << i << endl;
		if (p21.getvalue() == "1")
			u1.send("11");
		if (p20.getvalue() == "1")
			u1.send("10");

		f = u1.recv();
		// length = f.length() / 2;
		// cout << "length: " << length << endl;
		// //cout << f << endl;
		// f = f.substr(1,length);
		if (f.at(0) == '1')
		{
			f.erase(f.begin());
			cout << "Sensor1: ";
			cout << f << endl;
		}
		else if (f.at(0) == '2')
		{
			f.erase(f.begin());
			cout << "Sensor2: ";
			cout << f << endl;
		}
		//cout << "Sensor2: ";
		//usleep(10);
	}

}