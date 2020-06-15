#include <stdio.h>
#include <stdarg.h>

#include "spider.h"
#include "serial.h"



void print(char* msg,...){
	char buf[4*1024];
	char *p=&buf[0];
	va_list va;

	va_start(va,msg);
	vsprintf(buf,msg,va);
	va_end(va);

	while(*p != 0){
		SER_PutChar(*p);
		p++;
	}
}

void start(){
	delay();


	SER_Init();
	print("Hello World!\r\n");
}


void xadd(char* s){
	print(s);
}
