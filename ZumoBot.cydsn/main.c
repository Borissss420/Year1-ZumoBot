/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
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
/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

void progEnd(uint32_t delay);

//week3 ex1
#if 0
//motor
void zmain(void)
{
    // please place the bot to the starting line
    // six red spots touch the black starting line
    
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors

    motor_forward(165,2000);     // moving forward
    motor_turn(50,0,1050);     // turn
    motor_forward(0,0);
    vTaskDelay(1000);
    
    motor_forward(135, 1930);
    motor_turn(50,0,1050); 
    motor_forward(0,0);
    vTaskDelay(1000);
    
    motor_forward(150, 1810);
    motor_forward(0, 0);
    
    vTaskDelay(1000);
    motor_turn(50, 0, 1260);
    motor_forward(145, 500);
    motor_forward(0, 0);
    
    motor_turn(50, 0, 300);
    motor_forward(145, 300);
    motor_turn(50, 0, 100);
    motor_forward(145, 300);
    motor_turn(50, 0, 200);
    motor_forward(145, 180);
    motor_forward(0, 0);
    vTaskDelay(1000);
    
    motor_turn(50, 0, 220);
    motor_forward(145, 700);
    
     
    motor_forward(0,0);         // stop motors

    motor_stop();               // disable motor controller
    
    printf("\nTouch down!\n");
     progEnd(100);

}
#endif

//week3 ex2
#if 0
void zmain(void)
{
    Ultra_Start();
    motor_start();
    motor_forward(0, 0);
    
    BatteryLed_Write(1);
    while (SW1_Read()==1);
    BatteryLed_Write(0);
    vTaskDelay(1000);
    
    while (SW1_Read()==1){
        int x = Ultra_GetDistance();
        motor_forward(120, 50);
        if(x < 10){
            motor_forward(0, 0);
            vTaskDelay(1000);
            motor_backward(200, 1000);
            motor_turn(0, 250, 350);
        }   
    }
    
    printf("Mission Completed");
    motor_stop();
 }   
#endif

//week3 ex3
//Tank turn function
void tank_turn_left(uint8 speed,uint32 delay){
    SetMotors(1, 0, speed, 0, delay);
}

void tank_turn_right(uint8 speed,uint32 delay){
    SetMotors(0, 1, 0, speed, delay);
}

#if 0
void zmain(void)
{
    Ultra_Start();
    motor_start();
    motor_forward(0, 0);
    
    BatteryLed_Write(1);
    while(SW1_Read()==1);
    BatteryLed_Write(0);
    vTaskDelay(1000);
    
    while (SW1_Read()==1){
        int x = Ultra_GetDistance();
        
        if(x < 10){
            motor_forward(0, 0);
            vTaskDelay(1000);
            motor_backward(145, 1000);
            
           // found that 200 speed and 262 delay = angle 90 
           int angle = (rand()%(786-262+1))+ 262; // random in range
           if(rand()%2 == 1){ // random turn left or right
                tank_turn_left(200, angle);
           }else{
                tank_turn_right(200, angle);
           }
        }
        motor_forward(120, 50);
    }
    if(SW1_Read()== 0){
        motor_stop();
        printf("Mission Completed");
    }    
    motor_stop();
 }
#endif


//week4 ex1******************************************************
struct sensors_ dig;

void moveW4E1(void){
    int count = 0;
    
    while(count < 6){
        if(dig.L3 == 1 && dig.L2 ==1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
            //count delay
            if(count <= 5){
               vTaskDelay(350); 
            }else {
               vTaskDelay(0); 
            }      
        }
        
        motor_forward(100, 0);
        reflectance_digital(&dig);
    }    
    motor_forward(0, 0);
}    

#if 0
void zmain(void)
{
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 18000, 18000, 9000, 9000);
    motor_start();
    motor_forward(0, 0);
    Ultra_Start();
    
    //move to the first line
    BatteryLed_Write(1);
    while(SW1_Read()==1);
    BatteryLed_Write(0);
    vTaskDelay(1000);
    
    while(dig.L3 == 0 && dig.L2 == 0 && dig.R2 == 0 && dig.R3 == 0){
        motor_forward(255, 0);
        reflectance_digital(&dig);
    }    
    motor_forward(0, 0);
    
    IR_Start();
    IR_flush();
    
    bool led = false;
    while(true)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        moveW4E1();
    }
    progEnd(100);
 }

#endif

//week4 ex2 **********************************************************

void moveW4E2(void){
    int count = 0;
    
    while(count < 2){
        motor_forward(100, 0);
        
        if(dig.L3 == 1 && dig.L2 ==1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
            //count delay
            if(count < 2){
               vTaskDelay(350); 
            }else {
               vTaskDelay(0); 
            }      
        }
        
        if(dig.L2 == 0 && dig.R2 == 1){
            motor_turn(50, 0, 10);
        }else if(dig.L2 == 1 && dig.R2 == 0){
            motor_turn(0, 50, 10);
        }   
        reflectance_digital(&dig);
       
    }    
    motor_forward(0, 0);
}    

#if 0
 void zmain(void){
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 18000, 18000, 9000, 9000);
    motor_start();
    motor_forward(0, 0);
    Ultra_Start();
    
    //move to the first line
    BatteryLed_Write(1);
    while(SW1_Read()==1);
    BatteryLed_Write(0);
    vTaskDelay(1000);
    
    while(dig.L3 == 0 && dig.L2 == 0 && dig.R2 == 0 && dig.R3 == 0){
        motor_forward(255, 0);
        reflectance_digital(&dig);
    }    
    
    motor_forward(0, 0);
    
    IR_Start();
    IR_flush();
    
    bool led = false;
    while(true)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        moveW4E2();   
    }
    
    
    
    
 }   
#endif   

//week 4 ex3*******************************************************

void moveW4E3(void){
    int count = 0;
    
    while(count < 6){
        if(dig.L3 == 1 && dig.L2 ==1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
            //count delay
            if(count <= 4){
                vTaskDelay(300);
            }    
            if(count == 2){
                vTaskDelay(500);
                tank_turn_left(200, 262);
                motor_forward(100, 0);
                vTaskDelay(500);
            }
            
            if(count == 3 || count == 4){
                vTaskDelay(500);
                tank_turn_right(200, 262);
                motor_forward(100, 0);
                vTaskDelay(500);
            }    
        }
        
        motor_forward(100, 0);
        reflectance_digital(&dig);
    }   
    motor_forward(0, 0);
}    

#if 0
void zmain(void){
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 18000, 18000, 9000, 9000);
    motor_start();
    motor_forward(0, 0);
    Ultra_Start();
    
    //move to the first line
    BatteryLed_Write(1);
    while(SW1_Read()==1);
    BatteryLed_Write(0);
    vTaskDelay(1000);
    
    while(dig.L3 == 0 && dig.L2 == 0 && dig.R2 == 0 && dig.R3 == 0){
        motor_forward(255, 0);
        reflectance_digital(&dig);
    }    
    motor_forward(0, 0);
    
    IR_Start();
    IR_flush();
    
    bool led = false;
    while(true)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        moveW4E3();   
    }
    
    progEnd(100);
} 
#endif    

//week 5 ex1****************************************************************

#if 0
void zmain(void){
    TickType_t Tick = xTaskGetTickCount();
    TickType_t lastTick = 0;
    
    while(true){
        while(SW1_Read());
        vTaskDelay(10);
        Tick = xTaskGetTickCount();
        
        int interval = Tick - lastTick;//Time difference
        print_mqtt("Zumo07/button", "Milliseconds between two button presses: %d", interval);
        
        while(!SW1_Read());
        vTaskDelay(10);
        lastTick = Tick;
    }    
}    
    
#endif    

//week 5 ex2****************************************************************

#if 0
void zmain(void){
 while(true)
            {    motor_start();
                 Ultra_Start();
                motor_forward(200,100);
                if(Ultra_GetDistance() < 11){  //sensor detecting the obstacle
                   motor_forward(0,0);
                vTaskDelay(500);
                motor_backward(200,150);
                if(rand()%2 == 1){   //determining whether to turn left or right
                  tank_turn_left(200,262);  // found that 200 speed and 262 delay = angle 90
                print_mqtt("Zumo07/turn: ", "%s", "left");
                }else{
                 tank_turn_right(200,262);  
                print_mqtt("Zumo07/turn: ", "%s", "right");
                }
                }
                motor_forward(200,100); //keep moving
                    
    }      
}
   
#endif

//week 5 ex3****************************************************************
void moveW5E3(void) {
    TickType_t StartTime = 0, EndTime = 0;
    int count = 0, interval = 0;
    
    StartTime = xTaskGetTickCount();
    print_mqtt("Zumo07/Start time", "%d", StartTime);

    while(count < 2){
        if(dig.L3 == 1 && dig.L2 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
            vTaskDelay(110);
        }
        
        motor_forward(100, 0);
        reflectance_digital(&dig);
    }
    while(count < 3){
        if(dig.L3 == 1 && dig.L2 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
        }
        motor_forward(100, 0);
        reflectance_digital(&dig);
    }
    motor_forward(0, 0);
    EndTime = xTaskGetTickCount();
    print_mqtt("Zumo07/End Time", "%d", EndTime);
    interval = EndTime - StartTime;
    print_mqtt("Zumo07/button", "Milliseconds between two ticks: %d", interval);
} 




//custom funcation
void progEnd(uint32_t delay){
    bool led = false;
    while(true){
        BatteryLed_Write(led^=1);
        vTaskDelay(delay);
    }    
}






