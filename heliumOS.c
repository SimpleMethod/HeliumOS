/**
 * heliumOS.c
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

#include "heliumOS.h"

struct Commmand *comm;

/*!
 * Replacement for the printf function.
 * @param fmt
 * @param argp
 */
void vprint(const char *fmt, va_list argp) {
	HAL_Delay(1);
	char string[300];
	if (0 < vsprintf(string, fmt, argp)) {
		CDC_Transmit_FS((uint8_t*) string, strlen(string));
	}
	HAL_Delay(1);
}

/*!
 * Replacement for the printf function.
 * @param fmt
 */
void virtualPrintf(const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vprint(fmt, argp);
	va_end(argp);
}

/*!
 *  System shell initialization.
 */
void helium_init() {
	comm = (struct Commmand*) malloc(taksNumbers * sizeof(struct Commmand));
	strcpy(comm[0].command, "help");
	strcpy(comm[0].usage, "Displays all commands");
	strcpy(comm[0].args, "");
	comm[0].taskNumber = 1;
	strcpy(comm[1].command, "version");
	strcpy(comm[1].usage, "Return CLI version and number of active task");
	strcpy(comm[1].args, "");
	comm[1].taskNumber = 2;
	strcpy(comm[2].command, "amplifier");
	strcpy(comm[2].usage, "Activates signal amplifier");
	strcpy(comm[2].args, "0=off 1=on");
	comm[2].taskNumber = 3;
	strcpy(comm[3].command, "attenuator");
	strcpy(comm[3].usage, "Set signal attenuation");
	strcpy(comm[3].args, "See truth Table for more information");
	comm[3].taskNumber = 4;
	strcpy(comm[4].command, "thrucal");
	strcpy(comm[4].usage, "Activates THRU-CAL");
	strcpy(comm[4].args, "0=off 1=on");
	comm[4].taskNumber = 5;
}

/*!
 * Display motd message.
 */
void helium_motd() {
	if (mutexHello == 0) {
		virtualPrintf("\r\nHeliumOS CLI Shell\r\n");
		mutexHello = 1;
	}
}

/*!
 * Main loop of shell.
 */
void helium_mainLoop() {
	if (hostComOpen == 1) {
		helium_motd();
	}
	if (receivedDataFlag == 1) {
		receivedDataFlag = 0;
		helium_parseCommand();
	}
}

/*!
 * Display command prompt with input buffer.
 */
void helium_cliOutput() {
	virtualPrintf("he> %s \r\n", receivedData);
}

/*!
 * Display error when specified command is missing.
 */
void helium_cliOutputError() {
	virtualPrintf("%s ?\r\n", receivedData);
}

/*!
 * Search commands and arguments from the data buffer.
 */
void helium_parseCommand() {
	helium_cliOutput();
	char *args;
	int result;

	for (uint8_t var = 0; var < taksNumbers; ++var) {
		if ((helium_searchWordComplex(comm[var].command, (char*) receivedData,
				&args)) == 1) {
			result = var;
			break;
		} else {
			result = -1;
		}
	}

	switch (result) {
	case 0:
		helium_task_1(args);
		break;
	case 1:
		helium_task_2(args);
		break;
	case 2:
		helium_task_3(args);
		break;
	case 3:
		helium_task_4(args);
		break;
	case 4:
		helium_task_5(args);
		break;
	case -1:
		helium_cliOutputError(0);
		break;
	default:
		helium_cliOutputError();
		break;
	}

}

/*!
 * Search commands and return status with arguments as array.
 * @param command Commands to find.
 * @param buffer Data buffer.
 * @param args Returns array with arguments.
 * @return Status of search commands.
 */
uint8_t helium_searchWordComplex(char *command, char *buffer, char **args) {

	uint8_t result = 0;
	uint8_t size = strlen(command);
	char *argsBuffer = malloc(10);
	for (uint8_t var = 0; var < 20; ++var) {
		if (receivedData[var] == 0x20 && var == size) {
			strncpy(argsBuffer, (char*) receivedData + var + 1, 20 - var + 1);
			*args = argsBuffer;
			return 1;

		} else if (var + 1 == size) {
			memset(argsBuffer, 0, sizeof(*argsBuffer));
			*args = argsBuffer;
			if (receivedData[var + 1] != 0x20) {
				return 1;
			}
		}
		if (command[var] == receivedData[var]) {
			result = +1;
		} else {
			result = 0;
			break;
		}
	}
	if (result > 0) {
		return 1;
	}
	return 0;
}

/*!
 * Display all commands
 * @param args Command argument.
 */
void helium_task_1(char *args) {
	virtualPrintf("Commands:");
	for (uint8_t var = 0; var < taksNumbers; ++var) {
		virtualPrintf("%s ", comm[var].command);

	}
	virtualPrintf("\r\n");
	for (uint8_t var = 0; var < taksNumbers; ++var) {
		if (strcmp(args, comm[var].command) == 0) {
			virtualPrintf("command: %s \r\nusage:%s\r\narguments:%s\r\n",
					comm[var].command, comm[var].usage, comm[var].args);
		}
	}

}

/*!
 * Display version of HeliumOS.
 * @param args Command argument.
 */
void helium_task_2(char *args) {
	virtualPrintf("HeliumOS version %s active task: %d\r\n", "0.01",
			taksNumbers);
}

/*!
 * RF switch for activation of amplifier or attenuator.
 * @param args Command argument.
 */
void helium_task_3(char *args) {
	uint8_t flag = 0;
	switch (args[0]) {
	case '?':
		virtualPrintf("%d\r\n", flag);
		break;
	case '0':
		flag = 0;
		HAL_GPIO_WritePin(RF_MOD_GPIO_Port, RF_MOD_Pin, 1);
		break;
	case '1':
		flag = 1;
		HAL_GPIO_WritePin(RF_MOD_GPIO_Port, RF_MOD_Pin, 0);
		break;
	default:
		break;
	}
}

/*!
 * Selection of attenuation level.
 * @param args Command argument.
 */
void helium_task_4(char *args) {

	uint8_t flags[4] = { 1, 1, 1, 1 };
	uint16_t ports[4] = { 0x0010, 0x0020, 0x0040, 0x0080 };

	if (args[0] == '?') {
		virtualPrintf("%d%d%d%d\r\n", flags[0], flags[1], flags[2], flags[3]);
	}

	for (int var = 0; var < 4; ++var) {
		if (args[var] == '0') {
			flags[var] = 0;
			HAL_GPIO_WritePin(GPIOA, ports[var], 0);
		} else if (args[var] == '1') {
			flags[var] = 1;
			HAL_GPIO_WritePin(GPIOA, ports[var], 1);
		}
	}

}

/*!
 * RF switch for activation of THRU-CAL or signal isolation.
 * @param args  Command argument.
 */
void helium_task_5(char *args) {
	uint8_t flag = 0;
	switch (args[0]) {
	case '?':
		virtualPrintf("%d\r\n", flag);
		break;
	case '0':
		flag = 0;
		HAL_GPIO_WritePin(RF_PASS_GPIO_Port, RF_PASS_Pin, 1);
		break;
	case '1':
		flag = 1;
		HAL_GPIO_WritePin(RF_PASS_GPIO_Port, RF_PASS_Pin, 0);
		break;
	default:
		break;
	}
}
