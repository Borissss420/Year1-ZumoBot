/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//Project 3 Maze ******************************************************
struct sensors_ dig;
//move to the first line and wait for IR signal
    void P3startingline(void){
    BatteryLed_Write(1);
    while(SW1_Read()==1);
    BatteryLed_Write(0);
    vTaskDelay(1000);
    while(dig.L3 == 0 && dig.L2 == 0 && dig.R2 == 0 && dig.R3 == 0){
        motor_forward(255, 0);
        reflectance_digital(&dig);
    }    
    motor_forward(0, 0);
    print_mqtt("Zumo07/ready", " maze");
    }
    
void maze(void){
    int obs = 0; //obstacle
    int edge = 0;
    int countX = 0, countY = -3; //coordinate
    int mode = 0; //mode 0 = move up(turn left), 1 = move left, 2 = move right, 3 = move up(turn right), 4 = stop the car
    TickType_t startT = 0, endT = 0; //timing purpose
    int interval = 0; //interval between starting and ending time
    startT = xTaskGetTickCount(); //get starting time
    print_mqtt("Zumo07/start", " %d", startT);
    
    //Big Big Loop !!!!!
    while(true){
        //what to do when facing up !!! (Turn left)
        while(mode == 0){
            //count the line
            if(dig.L3 == 1 || dig.R3 == 1){
                countY++; //Position purpose Y-coordinate
                print_mqtt("Zumo7/position", " %d %d", countX, countY); //printing position
                vTaskDelay(400); //Avoiding double counting
            }
            //Using ultra sensor to detect obstacle
            int x = Ultra_GetDistance();
            //obstacle ahead
            if(x < 5){ 
                obs = 1;
            }
            //what to do when detected obstacle
            if(obs == 1){
                if(countX == -3){
                    mode = 3;
                }
                else {
                    motor_backward(100, 450); //move backward first
                //countY--; //Position purpose
                    while(dig.L3 == 0 && dig.R3 == 0){ //then move forward
                        motor_forward(100, 0);
                        if(dig.L3 == 1 && dig.R3 == 1){ //until reached the line of cross road
                        break;
                        }
                        reflectance_digital(&dig);
                    }
                    motor_forward(20, 300); //This 1
                    motor_turn(0, 175, 300); //This 2 together make a 90 degree left turn
                    obs = 0; //set obstacle to none
                    mode = 1; //set direction to left
                    motor_forward(0, 0); //stop the car
                    reflectance_digital(&dig);
                }
            }
            //Turn left if there is no road ahead
            if(countY >= 10 && countX >= 0 && dig.L1 == 0 && dig.R1 == 0){
                edge = 1;
            }
            if(countY >= 10 && countX <= 0 && dig.L1 == 0 && dig.R1 == 0){
                edge = 1;
                mode = 3;
            }
            if(edge == 1 && countX >=0){
                motor_backward(100, 450); //move backward first
                reflectance_digital(&dig);
                while(dig.L3 == 0 && dig.R3 == 0){ //then move forward
                    motor_forward(100, 0);
                    if(dig.L3 == 1){ //until reached the line of cross road
                    break;
                    }
                    reflectance_digital(&dig);
                }
                motor_forward(20, 300); //This 1
                motor_turn(0, 175, 300); //This 2 together make a 90 degree left turn
                edge = 0; //set edge to negative
                mode = 1; //set direction to left
                //countY++; //Position purpose Y-coordinate
                print_mqtt("Zumo7/position", " %d %d", countX, countY); //printing position
                motor_forward(0, 0); //stop the car
                reflectance_digital(&dig);   
            }
            //Stop the car
            if(countX == 0 && countY == 13){
                motor_forward(10, 1000);
                mode = 4;
            }
            //continue to move forward
            motor_forward(100, 1);
            reflectance_digital(&dig);
        } 
       ///what to do when facing left !!!
        while(mode == 1){           
            //Right turn to face back to up when no obstacle
            if(dig.L3 == 1 && dig.R3 == 1){
                motor_forward(0, 0);
                motor_forward(100, 60); //This 1
                motor_turn(175, 0, 300); //This 2 together make a 90 degree right turn
                countX--; //Position purpose X - coordinate
                print_mqtt("Zumo7/position", " %d %d", countX, countY); //Print position
                mode = 0; //set direction to up
            }
            //continue to move forward
            motor_forward(100, 1);
            reflectance_digital(&dig);
        }
        //what to do when facing right !!!
        while(mode == 2){
            //count the line
            /*if(dig.L3 == 1 && dig.L2 ==1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
                countY++;
                vTaskDelay(200);
            }*/
            /*//Using ultra sensor to detect obstacle
            int x = Ultra_GetDistance();
            //obstacle ahead
            if(x < 5){ 
                obs = 1;
            }
            //what to do when detected obstacle
            if(obs == 1){
                motor_backward(100, 450); //move backward first
                while(dig.L3 == 0 && dig.R3 == 0){ //then move forward
                    motor_forward(100, 0);
                    if(dig.L3 == 1 && dig.R3 == 1){ //until reached the line of cross road
                        break;
                    }
                    reflectance_digital(&dig);
                }
                motor_forward(100, 60); //This 1
                motor_turn(0, 175, 300); //This 2 together make a 90 degree left turn
                motor_backward(100, 450); //This 3
                motor_forward(100, 60); //This 4
                motor_turn(0, 175, 300); //This 5 together make a 180 degree turn
                obs = 0; //set obstacle to none
                mode = 2; //set direction to right
                motor_forward(0, 0); //stop the car
                reflectance_digital(&dig);
            }*/
            //Turn left to face up when there is no obstacle
            if(dig.L3 == 1 && dig.R3 == 1){
                motor_forward(0, 0);
                motor_forward(100, 60); //This 1
                motor_turn(0, 175, 300); //This 2 together make a 90 degree left turn
                countX++; //Position purpose X-coordinate
                print_mqtt("Zumo7/position", " %d %d", countX, countY); //Print position
                mode = 3; //set direction to up
            }
            //continue to move forward
            motor_forward(100, 1);
            reflectance_digital(&dig);
        }
        //what to do when facing up !!! (Turn right)
        while(mode == 3){
            //count the line
            if(dig.L3 == 1 || dig.R3 == 1){
                countY++; //Position purpose Y-coordinate
                print_mqtt("Zumo7/position", " %d %d", countX, countY); //printing position
                vTaskDelay(400); //Avoiding double counting
            }
            //Using ultra sensor to detect obstacle
            int x = Ultra_GetDistance();
            //obstacle ahead
            if(x < 5){ 
                obs = 1;
            }
            //what to do when detected obstacle
            if(obs == 1){
                if(countX == 3){
                    mode = 1;
                }
                else {
                    motor_backward(100, 450); //move backward first
                    //countY--; //Position purpose
                    while(dig.L3 == 0 && dig.R3 == 0){ //then move forward
                        motor_forward(100, 0);
                        if(dig.L3 == 1 && dig.R3 == 1){ //until reached the line of cross road
                        break;
                        }
                        reflectance_digital(&dig);
                    }
                    motor_forward(20, 300); //This 1
                    motor_turn(175, 0, 300); //This 2 together make a 90 degree right turn
                    obs = 0; //set obstacle to none
                    mode = 2; //set direction to right
                    motor_forward(0, 0); //stop the car
                    reflectance_digital(&dig);
                }
            }
            //Turn right if there is no road ahead
            if(countY >= 10 && countX <=0 && dig.L1 == 0 && dig.R1 == 0){
                edge = 1;
            }
            if(countY >= 10 && countX >=0 && dig.L1 == 0 && dig.R1 == 0){
                edge = 1;
                mode = 0;
            }
            if(edge == 1 && countX <=0){
                motor_backward(100, 450); //move backward first
                reflectance_digital(&dig);
                while(dig.L3 == 0 && dig.R3 == 0){ //then move forward
                    motor_forward(100, 0);
                    if(dig.R3 == 1){ //until reached the line of cross road
                    break;
                    }
                    reflectance_digital(&dig);
                }
                motor_forward(20, 300); //This 1
                motor_turn(175, 0, 300); //This 2 together make a 90 degree right turn
                edge = 0; //set edge to negative
                mode = 2; //set direction to right
                //countY++; //Position purpose Y-coordinate
                print_mqtt("Zumo7/position", " %d %d", countX, countY); //printing position
                motor_forward(0, 0); //Pause the car
                reflectance_digital(&dig); 
            }
            //Stop the car
            if(countX == 0 && countY == 13){
                motor_forward(10, 1000);
                mode = 4;
            }
            //continue to move forward
            motor_forward(100, 1);
            reflectance_digital(&dig);
        }
        //Break the loop to stop the car
        if(mode == 4){
            break;
        }
    }
    motor_forward(0, 0);
    endT = xTaskGetTickCount();
    interval = endT - startT;
    print_mqtt("Zumo07/stop", " %d", endT);
    print_mqtt("Zumo07/time", " %d", interval);
}
//Custom funcation
void progEnd(uint32_t delay){
    bool led = false;
    while(true){
        BatteryLed_Write(led^=1);
        vTaskDelay(delay);
    }    
}
#if 1
void zmain(void){
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 18000, 18000, 9000, 9000);
    motor_start();
    motor_forward(0, 0);
    Ultra_Start();
    P3startingline();  
    
    IR_Start();
    IR_flush();
    
    bool led = false;
    while(true) {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        maze();   
    }
    progEnd(100);               
}
#endif  

/* [] END OF FILE */
