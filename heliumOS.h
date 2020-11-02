/**
 * heliumOS.h
 *
 *  Created on: Nov 1, 2020
 *      Author: Michał Młodawski
 *
 *Copyright 2020 SimpleMethod
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy of
 *this software and associated documentation files (the "Software"), to deal in
 *the Software without restriction, including without limitation the rights to
 *use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *of the Software, and to permit persons to whom the Software is furnished to do
 *so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 ******************************************************************************
 */

#ifndef INC_HELIUMOS_H_
#define INC_HELIUMOS_H_

#include <main.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <usbd_cdc_if.h>

uint8_t receivedData[40];
uint8_t receivedDataFlag;
uint8_t hostComOpen;
uint8_t mutexHello;

struct Commmand {
	char command[20];
	char usage[50];
	char args[50];
	int taskNumber;
};

static const uint8_t taksNumbers = 5; /*Change to correct number of tasks*/

void vprint(const char *fmt, va_list argp);
void virtualPrintf(const char *fmt, ...);

void helium_init();
void helium_motd();
void helium_mainLoop();
void helium_parseCommand();

void helium_cliOutput();
void helium_cliOutputError();

uint8_t helium_searchWordComplex(char *command, char *buffor, char **args);

/* Section for new tasks */
void helium_task_1(char *args);
void helium_task_2(char *args);
void helium_task_3(char *args);
void helium_task_4(char *args);
void helium_task_5(char *args);

#endif /* INC_HELIUMOS_H_ */
