# NintendoDS_TouchScreen_XYpos
Nintendo DS game console has a built in Touch Screen and is equipped with ARM7+ARM9 processors. This project programs ARM7+ARM9 processors to detect Touch Screen press location and print X\Y location coordinates on console display.


Code Working:

The following functionalities have been added to the respective keys, as tabulated below:

 A: 		Touch screen reading feature is turned on
 
 B: 		Touch screen reading feature is turned off
 
 Up: 	 	Y coordinate is incremented by 1
 
 Down:	Y coordinate is decremented by 1
 
 Left: 	X coordinate is decremented by 1
 
 Right: X coordinate is incremented by 1

When the key 'A' is pressed, touch screen position is read from the touch pad serial adapter by ARM 7. The observed coordinates are stored in the following memory locations:

X coordinate - 0x3001000

Y coordinate - 0x3002000

ARM 9 has been programmed to read the coordinates and to print them on screen.

When any key press is detected especially of the 4 arrow keys or 'B', an event occurs. When 'B' is pressed, the input from the serial adapter of touch pad is no longer taken into account. When any of the arrow keys are pressed, the functionality indicated by the above tabulation is performed. This function is performed irrespective of activeness of the touch pad adapter. But upon disabling the touch pad adapter, the last touch values are stored and displayed to the user.

The modified values of the coordinates i.e after key press, have been stored in the following memory locations to be accessed by ARM 9 for displaying purposes:

X coordinate - 0x3001010


Y coordinate - 0x3002010

Motivation:
This project is an attempt to understand ARM7\ARM9 peripheral communication and memory mapping.

   i) MAIN memory used by ARM9 for cacheing. Identifying memory segment to allow inter-processor communciation is interesting. 
   ii) ARM 7 has a higher priority over ARM 9, had to sneak in extra parts to the code to handle efficient operation of program.  

Reference:
  Nintendo DS training kit was used to understand how to communicate with peripherals (touch screen & on_screen display).
  
Test:
 Code can be tested via Nintendo DS emulators.
