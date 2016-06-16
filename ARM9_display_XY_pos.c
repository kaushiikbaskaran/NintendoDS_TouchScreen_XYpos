#include <nds.h>
#include <stdio.h>

/* Memory segments - used for storing current\previous X\Y coordinates */
int *current_X_pos_p  = 0x3001000;
int *current_Y_pos_p  = 0x3002000;
int *prev_X_pos_p = 0x3001010;
int *prev_Y_pos_p = 0x3002010;


int main(void)
{
    consoleDemoInit();
    volatile int* local_current_X_pos_p  = (int *) current_X_pos_p;   //should be 32 bit aligned
    volatile int* local_current_Y_pos_p  = (int *) current_Y_pos_p;   //should be 32 bit aligned
    volatile int* local_prev_X_pos_p = (int *) prev_X_pos_p;      //should be 32 bit aligned
    volatile int* local_prev_Y_pos_p = (int *) prev_Y_pos_p;     //should be 32 bit aligned

    while(1)
    {
        iprintf("\x1b[5;0H                         ");
        iprintf ("\x1b[5;0H Touch screen reader before keypress x=%4X y=%4X",
                 *local_prev_X_pos_p, *local_prev_Y_pos_p);
        iprintf("\x1b[10;0H                         ");
        iprintf ("\x1b[10;0H Touch screen reader with keypress x=%4X y=%4X",
                 *local_current_X_pos_p, *local_current_Y_pos_p);
        
        swiWaitForVBlank();
    }
    return 0;
}

