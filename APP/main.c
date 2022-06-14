/**
 ***************************************************************************
 * @file    main.c
 * @author  abdelhai Tahoon
 * @brief   Main file of "Smart home project for (MAY) NTI By (FREE RTOS)"
 *
 *  This file includes the flow code of smart home application.
 *  Functionality of the project :
 *
 *		-->Door locking system using password input by keypad.
 *  	-->Displays real time temperature.
 *  	-->Controls a fan according to room temperature.
 *  	-->Controls a Window according to gas sensor level.
 *  	-->Buzzer alert after predefined wrong input passwords trials.
 *
 ***************************************************************************
 */

#include "main.h"


/* Password global variables*/
u32 UserEnteredPassword	= 0;
u32 CorrectPassword		= 1234;
u8 AvilableTrialsNumber			= MAX_TRIALS;


/* adc global variables*/
f32 TempReading;
u32 GasReading;
u16 AdcValueRead ;
u8 SwitcChannelFlag ;



/* create global object of system flags*/
SystemFags_t System; 					/*!--> Global object of Struct contains flags for Whole system	*/



/* Handel for each task to susspend or resume  tasks with each handel*/
xTaskHandle AdcReadHandle	= NULL;
xTaskHandle LoginHandle		= NULL;
xTaskHandle UserHandle		= NULL;
xTaskHandle EmergencyHandle = NULL;
xTaskHandle DoorHandel		= NULL;
xTaskHandle FanHandel		= NULL;
xTaskHandle WindowHandel	= NULL;


/*semaphor for Adc to prevent reading value untill isr of adc is Entered*/
xSemaphoreHandle AdcReadSem;


/****************************************************************************/
/********************* 		Main Function		  ***************************/
int main()
{
	/* Setup initializations */
	vSystemInitialization();

	/* create semaphore  for adc  */
	vSemaphoreCreateBinary(AdcReadSem);

	/*Create tasks arguments:-
	* ***********************************************
	* 1-Function name
	* 2-pointer for constant name for debugging
	* 3-stack depth of the task
	* 4-task input parameter as pointer to void
	* 5-task priority
	* 6-task ID(handle) as pointer to void
	* ***********************************************
	*/
	xTaskCreate(vTaskUserPanel	,NULL ,180,NULL ,1 ,&UserHandle);
	xTaskCreate(taskLoginPannel	,NULL ,180,NULL ,2 ,&LoginHandle);
	xTaskCreate(vTaskKeypad		,NULL ,120,NULL ,4 ,NULL);
	xTaskCreate(vtaskAdcRead		,NULL ,70 ,NULL ,3 ,&AdcReadHandle);
	xTaskCreate(vTaskEmergency	,NULL ,50 ,NULL ,5 ,&EmergencyHandle);
	xTaskCreate(vtaskDoor		,NULL ,50 ,NULL ,1 ,&DoorHandel);
	xTaskCreate(vTaskFan			,NULL ,50 ,NULL ,1 ,&FanHandel);
	xTaskCreate(vTaskWindow		,NULL ,50 ,NULL ,2 ,&WindowHandel);


	/*start the scheduler to handle excution of the tasks above*/
	vTaskStartScheduler();
	while(1){

	}

}
/*****************************End of main***********************************/
/***************************************************************************/

/****************************************************************************/
/******************		Definition of  Function		  ***********************/




/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vTaskUserPanel.									*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: for displaying login process					 	*/
/****************************************************************************/

void taskLoginPannel(void*pv){
	static u8 PassIndex =1;								/*index for displaying star for each entered key*/
	/*display constant messages in lcd one time */
	LCD_enuGoToPosition(2,4);
	LCD_enuWriteString("EnterPass");
	LCD_enuGoToPosition(4,3);
	LCD_enuWriteString("Trials Left");

	while(1){
		if(System.LoginFlag == FALSE)			/* if user didn't entered the correct password */
		{

			/* suspend all tasks are used for displaying system */
			vTaskSuspend(UserHandle);
			vTaskSuspend(AdcReadHandle);
			vTaskSuspend(DoorHandel);
			vTaskSuspend(FanHandel);
			vTaskSuspend(WindowHandel);


			if(System.EmergencyFlag == FALSE)	/* user still didn't entered maximum free trials */
			{
				LCD_enuGoToPosition(4,1);
				LCD_enuWriteIntegerNum(AvilableTrialsNumber);	/* display avilable trials */
			}

			if(System.keypressedFlag == TRUE)	/* if key pressed from keypad */
			{
				LCD_enuGoToPosition(1,PassIndex);	/* set the position to print star*/
				LCD_enuWriteData('*');
				PassIndex++;						/* increase the index for the next star*/
				System.keypressedFlag=0;			/* reset key pressed flag for another press*/
			}

			if(System.WrongPassFlag == TRUE)		/* if wrong password input */
			{
				LCD_enuGoToPosition(1,1);
				LCD_enuWriteString("            ");	/* clear stars printed for the new one */
				PassIndex=1;						/* reset the position of star index */
				System.WrongPassFlag = FALSE;		/* reset wrong password for the next time  */
			}

			if(System.EmergencyFlag == TRUE)		/* if emeregcy flag is set (user inter wrong password more than avilable trials) */
			{
				LCD_enuClear();						/* clear screen to display alert message */
				LCD_enuGoToPosition(2,1);
				LCD_enuWriteString("ALERT!! EMERGECY");
			}else if (System.EnteredEmergencyFlag == TRUE &&  System.EmergencyFlag == FALSE) /* check if duration of emergency is ended*/
			{
				LCD_enuClear();						/*clear screen to display password pannel again */
				LCD_enuGoToPosition(2,4);
				LCD_enuWriteString("EnterPass");
				LCD_enuGoToPosition(4,3);
				LCD_enuWriteString("Trials Left");
				System.EnteredEmergencyFlag= FALSE;	/*reset the value of the flag  */
			}


		}

		vTaskDelay(100);
	}

}

/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vTaskUserPanel.									*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: after login for displaying interfacing process 	*/
/****************************************************************************/
void vTaskUserPanel(void*pv){
	/* display fixed messages in lcd */
	LCD_enuGoToPosition(1,1);
	LCD_enuWriteString("Door:");

	LCD_enuGoToPosition(3,1);
	LCD_enuWriteString("Temp:");

	LCD_enuGoToPosition(4,1);
	LCD_enuWriteString("Window:");

	LCD_enuGoToPosition(2,1);
	LCD_enuWriteString("Fan :");



	while(1){
		if(System.LoginFlag == TRUE)			/* if user Entered right password */
		{

			/* suspend login and emergency task */
			vTaskSuspend(LoginHandle);
			vTaskSuspend(EmergencyHandle);



			if(System.DoorFlag == DOOR_OPENING){		/* for printing indicaton of the door  status */
				LCD_enuGoToPosition(1,6);
				LCD_enuWriteString("Opened");
			}else if ((System.DoorFlag == DOOR_CLOSED))
			{
				LCD_enuGoToPosition(1,6);
				LCD_enuWriteString("Closed");
			} else{/*Misra c */}


			if(System.FanFlag == TRUE){					/* for printing indicaton of the fan  status */
				LCD_enuGoToPosition(2,6);
				LCD_enuWriteString("ON ");
			}else if(System.FanFlag == FALSE){
				LCD_enuGoToPosition(2,6);
				LCD_enuWriteString("OFF ");
			}


			LCD_enuGoToPosition(3,6);
			LCD_enuWriteIntegerNum(TempReading);	/* for printing current tempreature value */

			if(System.WinodwFlag == TRUE)
			{
				LCD_enuGoToPosition(4,9);
				LCD_enuWriteString("ON ");
			}else
			{
				LCD_enuGoToPosition(4,9);
				LCD_enuWriteString("OFF");
			}


		}

		vTaskDelay(100);

	}

}



/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vTaskKeypad.										*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: take input key from the keypad  					*/
/****************************************************************************/
void vTaskKeypad(void*pv){

	u8 KeyPressedVal;
	while(1){

		Keypad_GetPressedKey(&KeyPressedVal);		/*store key pressed from keypad in KeyPressedVal */

		if (KeyPressedVal != KEYPAD_NOT_PRESSED)	/*check if user pressed a key */
		{
			if(KeyPressedVal>='0' &&KeyPressedVal <='9')		/*!check whether the key pressed is number of not*/
			{
				System.keypressedFlag =1;						/*indication for key pressed to display star*/
				UserEnteredPassword *=10;						/*update the next digit to store in a password*/
				UserEnteredPassword += (KeyPressedVal - 48);	/*Convent digit to number from ascii code by subtract '0'
				 	 	 	 	 	 	 	 	 	 	 	 	 then add it to the previous value*/

			}else if (KeyPressedVal == '=')						/*!check if the user entered a password*/
			{
				if (UserEnteredPassword == CorrectPassword)		/*!check if the password is correct */
				{
					System.LoginFlag = TRUE;					/*!--> Set Login flag is true */
					LCD_enuClear();								/*!--> clear screen to display new panel*/
					/*Resume all tasks for displaying the system*/
					vTaskResume(UserHandle);
					vTaskResume(AdcReadHandle);
					vTaskResume(DoorHandel);
					vTaskResume(FanHandel);
					vTaskResume(WindowHandel);
				}else											/*!check if the password is False */
				{
					System.WrongPassFlag = TRUE;				/*!-->Set Wrong  entered password flag to tue */
					UserEnteredPassword =0;						/*!-->reset the entered password variable for new value*/
					AvilableTrialsNumber--;					    /*!-->decrease the avilable trial numbers */
				}

			}else {/* Misra C Rules */}


			if( AvilableTrialsNumber == 0)				 		/*!check if the entered wrong password exceeded the limit*/
			{
				System.EmergencyFlag = TRUE;					/*!-->Set emergency flag */
				AvilableTrialsNumber=MAX_TRIALS;				/*!-->return number of avilable trial for next time */
			}else {/* Misra C Rules */}
		}

		vTaskDelay(50);
	}

}

/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vTaskEmergency.									*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: task to set alert by buzzer for a while then
 * 						  return to system login again 						*/
/****************************************************************************/

void vTaskEmergency(void*pv)
{
	static u8 CountTimerForEmergency =0; 	/*!--> counter for buzzer time to be on */
	while(1)
	{
		if(	System.EmergencyFlag == TRUE)	/*!--> check for state of Emergency */
		{
			DIO_enuTogglePinValue(DIO_u8GROUP_A, DIO_u8PIN5); /* Toggle buzzer */
			CountTimerForEmergency++;	/* increase the timer counter */

			if(CountTimerForEmergency == 10){	/* if it's reached 8 times of 2oo ms so it's 2 seconds */
				CountTimerForEmergency =0;		/* Reset the counter */
				System.EnteredEmergencyFlag =1;	/*flag to display enter password  pannel again */
				System.EmergencyFlag = FALSE;	/*to go out of emergency Mode */

			}
		}
		vTaskDelay(200);
	}


}

/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vtaskDoor.										*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: task to control the door when login
 * 						  it opens door for a while then wait then close it */
/****************************************************************************/
void vtaskDoor(void*pv)
{
	u8 DoorTimeCounter =1;			/* counter for the door time */
	while(1){
		if(System.LoginFlag == TRUE)	/* if user entered right password  */
		{
			if(System.DoorFlag == DOOR_OPENING)	/* check if the door is opening */
			{


				if(DoorTimeCounter <=5) 	/* till the counter reaches 200 ms 5 times  let the door opening*/
				{

					DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN6,DIO_u8HIGH);
					DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN7,DIO_u8LOW);
					DoorTimeCounter++;

				}
				else if(DoorTimeCounter <=9)	/* till the counter reaches 200 ms  4 time (9-5) times  wait and do nothing */
				{
					DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN6,DIO_u8LOW);
					DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN7,DIO_u8LOW);
					DoorTimeCounter++;

				}
				else if(DoorTimeCounter <=13)	/* till the counter reaches 200 ms 4  (13-9)times  let the door Closing*/
				{
					DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN6,DIO_u8LOW);
					DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN7,DIO_u8HIGH);
					DoorTimeCounter++;

				}
				else							/*after that time do nothing */
				{
					DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN6,DIO_u8LOW);
					DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN7,DIO_u8LOW);
					System.DoorFlag = DOOR_CLOSED ;
					DoorTimeCounter = 0;
				}




			}

		}


		vTaskDelay(200);
	}



}

/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vTaskFan.											*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: task to control fan according to temprature sensor*/
/****************************************************************************/
void vTaskFan(void*pv){
	while(1){

		if (TempReading >= MAX_TEMPRATURE)		/* Tempreature exceeded maximum Tempreature*/
		{
			DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN3,DIO_u8HIGH);	/* open the fan */
			System.FanFlag = TRUE;	/* set flag of a fan to display */
		}
		else
		{
			DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN3,DIO_u8LOW);	/*close the fan */
			System.FanFlag = FALSE; /* clear flag of a fan to display */
		}

		vTaskDelay(170);

	}
}

/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vTaskWindow.										*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: task to control window according to gas sensor	*/
/****************************************************************************/
void vTaskWindow(void*pv){
	while (1)
	{
		if (GasReading > ULTIMATE_GAS_VALUE )
		{
			DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN4,DIO_u8HIGH);	/*open the window */
			System.WinodwFlag = TRUE; /* set flag of a window to display */
		}else
		{
			DIO_enuSetPinValue(DIO_u8GROUP_A, DIO_u8PIN4,DIO_u8LOW); /*close the window */
			System.WinodwFlag = FALSE; /* clear flag of a fan to display */
		}
		vTaskDelay(170);
	}
}

/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vAdcIsrNotification.								*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: indication that adc has finished  conversion	 	*/
/****************************************************************************/
void vtaskAdcRead(void*pv){

	u8 semaphorStatus ;			/*  for semaphore status */
	while(1){
		semaphorStatus= xSemaphoreTake(AdcReadSem,5);			/*  store status of taking semaphore that AdcIsrNotification give */

		if(semaphorStatus == pdPASS)							/*!if succeeded in taking the semaphore */
		{
			if(SwitcChannelFlag ==0){							/*!-->switch the adc channel to read from gas sensor */
				ADC_enuSelectChannel(1);
				ADC_enuRead( &AdcValueRead );					/*!-->store the value of adc read  */

				GasReading = (u32) (AdcValueRead * 5000UL / 1024);  /*!-->convert it to millivolt  */

				ADC_enuStartConversion();						 /*!-->start adc conversion let the AdcIsrNotification give a semaphore */
				SwitcChannelFlag =1;							 /*!-->switch the flag for next time to read from temperature sensor */
			}
			else if (SwitcChannelFlag ==1)
			{
			ADC_enuSelectChannel(0);							/*!-->switch the adc channel to read from Temperature sensor */
			ADC_enuRead( &AdcValueRead );						/*!-->store the value of adc read  */

			TempReading = (5 * AdcValueRead * 100.f /1024);		/*!-->convert it temperature values */

				if (TempReading - ((u32) TempReading ) >= .5)	/*approximating for the next integer value */
				{
					TempReading = (u32) TempReading + 1 ;
				}

			ADC_enuStartConversion();							/*start adc conversion let the AdcIsrNotification give a semaphore */
			SwitcChannelFlag =0;								/*switch the flag for next time to read from temperature sensor */
			}


		}

		vTaskDelay(10);

	}
}

/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vAdcIsrNotification.								*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: indication that adc has finished  conversion	 	*/
/****************************************************************************/
void vAdcIsrNotification(void*p)
{
	xSemaphoreGive(AdcReadSem);			 /*Give a semaphore for adc read to take */
}

/****************************************************************************/
/****************************************************************************/
/*  Function Name		: vSystemInitialization.							*/
/*  Function Return		: void												*/
/*  Function Arguments	: void 												*/
/*  About function		: It contains initialization of everything in system*/
/****************************************************************************/
void vSystemInitialization(void)
{

	/* LCD initioalization */
	LCD_enuInit();
	LCD_enuWriteCommand(0x0c);	// remove cursor
	/* ADC initioalization */
	ADC_enuInit();
	ADC_enuEnableADC_INT();
	ADC_enuCallBack(vAdcIsrNotification, NULL);
	ADC_enuEnableADC();
	GIE_enuEnable();
	ADC_enuSelectChannel(1);
	ADC_enuStartConversion();
	/* Keypad initioalization */
	Keypad_enuInit();
	/* buzzerInitialization	*/
	DIO_enuSetPinDirection(DIO_u8GROUP_A, DIO_u8PIN5,DIO_u8OUTPUT);
	/* Door Initialization	*/
	DIO_enuSetPinDirection(DIO_u8GROUP_A, DIO_u8PIN6,DIO_u8OUTPUT);
	DIO_enuSetPinDirection(DIO_u8GROUP_A, DIO_u8PIN7,DIO_u8OUTPUT);
	/* Fan Initialization	*/
	DIO_enuSetPinDirection(DIO_u8GROUP_A, DIO_u8PIN3,DIO_u8OUTPUT);
	/* Window Initialization	*/
	DIO_enuSetPinDirection(DIO_u8GROUP_A, DIO_u8PIN4,DIO_u8OUTPUT);
}

/****************************************************************************/
