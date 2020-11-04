
# HeliumOS


## Simple command line interface for create and manage tasks based on STM32

![HeliumOS](https://raw.githubusercontent.com/SimpleMethod/HeliumOS/master/Images/heliumOS.png)

# 🚀  Features

- Automatic baud rate support
- Automatic generation of help functions 
- Support for errors and function arguments
- Protection against incorrect commands

# 🖥️ Configuration
1. Adds USB support in the MCU and sets it as a virtual port in middleware section.
2. Changing buffer size **APP_RX_DATA_SIZE** to 40 characters in **usbd_cdc.if.h** file.
3. Add code in **CDC_Control_FS** function in **CDC_SET_CONTROL_LINE_STATE** switch case in **usbd_cdc.if.c** file.

>     /*SYSTEM INTERRUPT TO CHECK IF THE COM PORT HAS BEEN OPENED*/
>         	USBD_SetupReqTypedef * req = (USBD_SetupReqTypedef *)pbuf;
>         	if(req->wValue &0x0001 != 0)
>         		hostComOpen = 1;
>         		else
>         		hostComOpen = 0;

4. Add code in **CDC_Receive_FS** function after **USBD_CDC_ReceivePacket** function  and before **return (USBD_OK)** in  **usbd_cdc.if.c** file.

>       /*SYSTEM INTERRUPT TO RECEIVED DATA*/
>       extern uint8_t receivedData[40];
>       extern uint8_t receivedDataFlag;
>       memset(receivedData, 0, sizeof(receivedData));
>       strlcpy((char*) receivedData, (char*) Buf, (*Len) + 1);
>       receivedDataFlag = 1;
5. In **USER CODE BEGIN Includes** in file **main.c**  add **#include "heliumOS.h"**
6. In **USER CODE BEGIN 2** in file **main.c**  add **helium_init();"**
7. In main loop in file **main.c**  add **helium_mainLoop();"**
8. At this point you can start creating a new tasks!
# 🗺️  Guide for adding a new task

 1. Change number of tasks by editing the **taskNumbers** variable in **heliumOS.h**
 2. In the section for new tasks in the **heliumOS.h** file we add a new function in the format:
**void helium_task_X(char** * **args);**  where X is the task number.
 3. In **helium_init** function in the **heliumOS.c** file just write data in structure just as in example task. 
 4.  In **helium_parseCommand** function in the **heliumOS.c** file add a new task to switch block.
