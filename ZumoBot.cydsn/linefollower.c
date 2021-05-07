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
//Project 2 line follower *********************************************
struct sensors_ dig;
void linefollower(void){
    int count = 0;
    TickType_t startT = 0, endT = 0, missT =0, lineT =0;
    int interval = 0;
    startT = xTaskGetTickCount();
    print_mqtt("Zumo07/Start", " %d", startT);
    
    while(count < 3){
        //continue to move forward
        motor_forward(255, 1);
        //count the line
        if(dig.L3 == 1 && dig.L2 ==1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
            //count delay
            if(count < 3){
               vTaskDelay(400); 
            } else{
               vTaskDelay(0);              
               endT = xTaskGetTickCount();
               interval = endT - startT;
               print_mqtt("Zumo07/stop", " %d", endT);
               print_mqtt("Zumo07/time", " %d", interval);
            }  
        }
        //Left turn > 90 degrees
        if(dig.R2 == 1 && dig.R3 == 1 && dig.L2 == 1 && dig.L3 == 0){
            motor_turn(0, 255, 1);
            //SetMotors(0, 1, -128, 255, 1);
            //print_mqtt("zumo7", "hard left");
        }
        //Right turn > 90 degrees
        if(dig.R2 == 1 && dig.R3 == 0 && dig.L2 == 1 && dig.L3 == 1){
            motor_turn(255, 0, 1);
            //SetMotors(1, 0, 255, -128, 1);
            //print_mqtt("zumo7", "hard right");
        }
        //Simple left turn
        if(dig.L2 == 1 && dig.R2 == 0){ 
            SetMotors(1, 0, 255, 0, 1);
            //print_mqtt("zumo07","normal left");
        }  
        //Simple right turn
        if(dig.L2 == 0 && dig.R2 == 1){
            SetMotors(0, 1, 0, 255, 1);
            //print_mqtt("zumo7", "normal right");
        }
        //Bonus
        //off track
        if(dig.L1 == 0 && dig.R1 == 0){
            missT = xTaskGetTickCount();
            print_mqtt("Zumo07/miss", " %d", missT);
            //go back to the track
            if(dig.L1 == 1 && dig.R1 == 0){
            lineT = xTaskGetTickCount();
            print_mqtt("Zumo07/line", " %d", lineT);
            }
        }
       reflectance_digital(&dig);
    }    
    motor_forward(0, 0); 
}
//move to the first line and wait for IR signal
    void firstline(void){
    BatteryLed_Write(1);
    while(SW1_Read()==1);
    BatteryLed_Write(0);
    vTaskDelay(1000);
    while(dig.L3 == 0 && dig.L2 == 0 && dig.R2 == 0 && dig.R3 == 0){
        motor_forward(255, 0);
        reflectance_digital(&dig);
    }    
    motor_forward(0, 0);
    print_mqtt("Zumo07/ready", " line");
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
    firstline();  
    
    IR_Start();
    IR_flush();
    
    bool led = false;
    while(true) {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        linefollower();   
    }
    progEnd(100);               
}
#endif  
/* [] END OF FILE */
