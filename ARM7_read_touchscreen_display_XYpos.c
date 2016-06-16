#include <nds.h>

/* Memory segments - used for storing current\previous X\Y coordinates */
int *current_X_pos_p  = 0x3001000;
int *current_Y_pos_p  = 0x3002000;
int *prev_X_pos_p = 0x3001010;
int *prev_Y_pos_p = 0x3002010;

/* Flag to detect if its time to power down */
volatile bool exitflag = FALSE;

/* Callback API when power button is pressed */
void powerButtonCB()
{
	exitflag = true;
}

/* API determines the touch coordinates          *
 *                                               *
 * @Input:                                       *
 *  uint8 command                                *
 *     command dictates either to read X or Y    *
 *  position info from touch peripheral          *
 *                                               */
uint8 touchdontRead(uint8 command)
{
	uint16 result, result2;
	uint32 oldIME = REG_IME;

	REG_IME = 0;
	
	SerialWaitBusy();

	// Write the command and wait for it to complete
	REG_SPICNT = SPI_ENABLE | SPI_BAUD_2MHz | SPI_DEVICE_TOUCH | SPI_CONTINUOUS; //0x8A01;
	REG_SPIDATA = command ;// Based on the command , the X or Y coordinates will be read
    SerialWaitBusy(); //Wait for serial communication to occur

	// Write the second command and clock in part of the data
	REG_SPIDATA = 0;
	SerialWaitBusy();
	result = REG_SPIDATA;

	// Clock in the rest of the data (last transfer)
	REG_SPICNT = SPI_ENABLE | 0x201;
	REG_SPIDATA = 0;
	SerialWaitBusy();

	result2 = REG_SPIDATA ;
	REG_IME = oldIME;

	// Return the result
	return result ;
}

int main()
{

	irqInit();
	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);
	setPowerButtonCB(powerButtonCB);
	 
	/* Local variable declaration */
	volatile static int keypress;
	volatile static uint8  enable_touch_press_sense = 0;
    uint8 x_pos_read=0, y_pos_read=0;
    uint8 curr_X_pos=0, curr_Y_pos=0;
	uint8 next_X_pos=0, next_Y_pos=0;
    uint8 horizontal_pos_change, vertical_pos_change;
    
    //While loop starts
    while (!exitflag)
    {
		if ( 0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R)))
        {
			exitflag = true;
		}
		
        //To detect the key pressed. Variable "Keypress" is used to store the value.
		asm ("MOV R0, #0x4000000"); 
		asm ("ADD R0, #0x130");  
		asm ("LDR R4, [R0]");
		asm ("MOV %[result], R4":[result]"=r"(keypress));
		
		//Detecting Touch positions
		x_pos_read = touchdontRead(0XD0); //X coordinates is stored in x_pos_read.
		y_pos_read = touchdontRead(0X90); //Y coordinates is stored in y_pos_read.
	
	//Depending on the key pressed, suitable action is decided using switch statement.
	switch (keypress)
		
		{
		case 1022:  //A
			enable_touch_press_sense = 1;
			break;
		case 959:  //UP
			vertical_pos_change = 1;
			break;
		case 895:  //DOWN
			vertical_pos_change = -1;
			break;
		case 991:  //LEFT
		    horizontal_pos_change = -1;
			break;
		case 1007:  //RIGHT
			horizontal_pos_change = 1;
			break;
		case 1021:  //B
			enable_touch_press_sense = 0;
			break;
	}

	//When A is pressed and till B press doesn't occur, the following block executes.
	if (enable_touch_press_sense==1)
	{
        // The keypress effect is added to the coordinates in this block.
        if ((x_pos_read==curr_X_pos ) && (y_pos_read==curr_Y_pos ))
        {
            next_X_pos=next_X_pos+(1*horizontal_pos_change);
            next_Y_pos=next_Y_pos+(1*vertical_pos_change);
            horizontal_pos_change=0;vertical_pos_change=0;
        }
        else
        {
            horizontal_pos_change=0;vertical_pos_change=0;
            curr_X_pos=x_pos_read;
            curr_Y_pos=y_pos_read;
            next_X_pos=x_pos_read;
            next_Y_pos=y_pos_read;
        }
	}
	//When B is pressed
	else
	{
		x_pos_read=next_X_pos+(1*horizontal_pos_change);
		y_pos_read=next_Y_pos+(1*vertical_pos_change);
		next_X_pos=x_pos_read;
		next_Y_pos=y_pos_read;
		horizontal_pos_change=0;vertical_pos_change=0;
	}
    *prev_X_pos_p=curr_X_pos;// The coordinates are pasted to the memory location to be transfered to ARM 9.
    *prev_Y_pos_p=curr_Y_pos;
    *current_X_pos_p=next_X_pos;
    *current_Y_pos_p=next_Y_pos;
        
	swiWaitForVBlank();
    }

    return 0;
}
