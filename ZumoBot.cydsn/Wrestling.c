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
//Project sumo wrestling *********************************************
//A function that move the robot to the edge of the ring
struct sensors_ dig;
void startingline(void){
    BatteryLed_Write(1);
    while(SW1_Read()==1);
    BatteryLed_Write(0);
    vTaskDelay(1000);
    while(dig.L3 == 0 && dig.R3 == 0){
        //Move along the line turn left
        if(dig.L2 == 1 && dig.R2 == 0){
            motor_turn(0, 70, 10);
        }    
        motor_forward(255, 0);
        reflectance_digital(&dig);
    }
    motor_forward(0, 0);
    print_mqtt("Zumo07/ READY", "zumo");
}

//A function about the wrestling
void wrestling(void){
       
    TickType_t startT = 0, endT = 0, obstacleT = 0;
    int interval = 0;
    startT = xTaskGetTickCount();
    print_mqtt("Zumo07/ Start", "%d", startT);
    
    while (SW1_Read()==1){
        int x = Ultra_GetDistance();
        vTaskDelay(100);
        
        //Using ultra sensor to detect obsticle
        if(x < 5){
            motor_forward(0, 0);
            motor_backward(145, 200);
           
           //turn left 
            motor_turn(0, 150, 200);
            obstacleT = xTaskGetTickCount();
            print_mqtt("zumo07/ obstacle ", "%d", obstacleT);
        }
        
        //Stay inside the ring
        if(dig.L3 == 0 && dig.R3 == 1){
             motor_forward(0, 0);
             motor_backward(50, 500);
             //tank_turn_left(200, angle);
             motor_turn(0, 90, 1000);
             obstacleT = xTaskGetTickCount();
             //print_mqtt("zumo07/ obstacle", "%d", obstacleT);
         }
         if(dig.L3 == 1 && dig.R3 == 0){
              motor_forward(0, 0);
              motor_backward(50, 500);
              //tank_turn_right(200, angle);
              motor_turn(90, 0, 1000);
              obstacleT = xTaskGetTickCount();
              //print_mqtt("zumo07/ obstacle", "%d", obstacleT);
         }
        motor_forward(70, 0);
        reflectance_digital(&dig);
    }
    if(SW1_Read()== 0){
        motor_stop();
        endT = xTaskGetTickCount();
        interval = endT - startT;
        print_mqtt("zumo07/stop", "%d", endT);
        print_mqtt("zumo07/time", "%d", interval);
    }    
    motor_stop();
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
    
    startingline();
    
    IR_Start();
    IR_flush();
    
    bool led = false;
    while(true){
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        wrestling();   
    }
    progEnd(100);               
}

#endif


/* [] END OF FILE */
