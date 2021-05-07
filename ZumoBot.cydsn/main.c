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
        moveW5E3();   
    }
    
    progEnd(100);               
}
   
#endif

//Project sumo wrestling *********************************************
//A function that move the robot to the edge of the ring
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

//Project 2 line follower *********************************************

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
    
#if 0
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
//Project 3 Maze ******************************************************
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
            //count the line
            /*if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3){
                countX--; //Position purpose X-coordinate
                print_mqtt("Zumo7/position", " %d %d", countX, countY); //printing position
                vTaskDelay(200); //Avoiding double counting
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
                countX++; //Position purpose
                while(dig.L3 == 0 && dig.R3 == 0){ //then move forward
                    motor_forward(100, 0);
                    if(dig.L3 == 1 && dig.R3 == 1){ //until reached the line of cross road
                        break;
                    }
                    reflectance_digital(&dig);
                }
                motor_forward(100, 60); //This 1
                motor_turn(175, 0, 300); //This 2 together make a 90 degree right turn
                motor_backward(100, 450); //This 3
                motor_forward(100, 60); //This 4
                motor_turn(175, 0, 300); //This 5 together make a 180 degree turn
                obs = 0; //set obstacle to none
                mode = 2; //set direction to right
                motor_forward(0, 0); //stop the car
                reflectance_digital(&dig);
            }*/
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
#if 0
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

#if 0
// Name and age
void zmain(void)
{
    char name[32];
    int age;
    
    
    printf("\n\n");
    
    printf("Enter your name: ");
    //fflush(stdout);
    scanf("%s", name);
    printf("Enter your age: ");
    //fflush(stdout);
    scanf("%d", &age);
    
    printf("You are [%s], age = %d\n", name, age);

    while(true)
    {
        BatteryLed_Write(!SW1_Read());
        vTaskDelay(100);
    }
 }   
#endif

#if 0
//battery level//
void zmain(void)
{
    ADC_Battery_Start();        

    int16 adcresult =0;
    float volts = 0.0;

    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed

    while(true)
    {
        char msg[80];
        ADC_Battery_StartConvert(); // start sampling
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            
            // Print both ADC results and converted value
            printf("%d %f\r\n",adcresult, volts);
        }
        vTaskDelay(500);
    }
 }   
#endif

#if 0 

//Tick Timer Example
void zmain(void) 
{
	TickType_t Ttime = xTaskGetTickCount();
	TickType_t PreviousTtime = 0;

	while(true) 
	{
		while(SW1_Read()) vTaskDelay(1); // loop until user presses button
		Ttime = xTaskGetTickCount(); // take button press time
		/*Print out the time between button presses in seconds. int cast used to suppress warning messages*/
		printf("The amount of time between button presses is: %d.%d seconds\n", (int)(Ttime-PreviousTtime)/1000%60, (int)(Ttime-PreviousTtime)%1000);
		while(!SW1_Read())vTaskDelay(1); // loop while user is pressing the button
		PreviousTtime = Ttime; // remember previous press time
	}
	
}

#endif

#if 0
// button
void zmain(void)
{
    while(true) {
        printf("Press button within 5 seconds!\n");
	    TickType_t Ttime = xTaskGetTickCount(); // take start time
        bool timeout = false;
        while(SW1_Read() == 1) {
            if(xTaskGetTickCount() - Ttime > 5000U) { // too long time since start
                timeout = true;
                break;
            }
            vTaskDelay(10);
        }
        if(timeout) {
            printf("You didn't press the button\n");
        }
        else {
            printf("Good work\n");
            while(SW1_Read() == 0) vTaskDelay(10); // wait until button is released
        }
    }
}
#endif

#if 0
// button
void zmain(void)
{
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    while(true)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) vTaskDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif


#if 0
//ultrasonic sensor//
void zmain(void)
{
    Ultra_Start();                          // Ultra Sonic Start function
    
    while(true) {
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
        printf("distance = %d\r\n", d);
        vTaskDelay(200);
    }
}   
#endif

#if 0
//IR receiverm - how to wait for IR remote commands
void zmain(void)
{
    IR_Start();
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    bool led = false;
    // Toggle led when IR signal is received
    while(true)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        if(led) printf("Led is ON\n");
        else printf("Led is OFF\n");
    }    
 }   
#endif



#if 0
//IR receiver - read raw data
// RAW data is used when you know how your remote modulates data and you want to be able detect 
// which button was actually pressed. Typical remote control protocols requires a protocol specific
// state machine to decode button presses. Writing such a state machine is not trivial and requires
// that you have the specification of your remotes modulation and communication protocol    
void zmain(void)
{
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    // print received IR pulses and their lengths
    while(true)
    {
        if(IR_get(&IR_val, portMAX_DELAY)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
        }
    }    
 }   
#endif


#if 0
//reflectance
void zmain(void)
{
    struct sensors_ ref;
    struct sensors_ dig;

    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    

    while(true)
    {
        // read raw sensor values
        reflectance_read(&ref);
        // print out each period of reflectance sensors
        printf("%5d %5d %5d %5d %5d %5d\r\n", ref.L3, ref.L2, ref.L1, ref.R1, ref.R2, ref.R3);       
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig); 
        //print out 0 or 1 according to results of reflectance period
        printf("%5d %5d %5d %5d %5d %5d \r\n", dig.L3, dig.L2, dig.L1, dig.R1, dig.R2, dig.R3);        
        
        vTaskDelay(200);
    }
}   
#endif


#if 0
//motor
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors

    vTaskDelay(3000);
    
    motor_forward(100,2000);     // moving forward
    motor_turn(200,50,2000);     // turn
    motor_turn(50,200,2000);     // turn
    motor_backward(100,2000);    // moving backward
     
    motor_forward(0,0);         // stop motors

    motor_stop();               // disable motor controller
    
    while(true)
    {
        vTaskDelay(100);
    }
}
#endif

#if 0
/* Example of how to use te Accelerometer!!!*/
void zmain(void)
{
    struct accData_ data;
    
    printf("Accelerometer test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    
    while(true)
    {
        LSM303D_Read_Acc(&data);
        printf("%8d %8d %8d\n",data.accX, data.accY, data.accZ);
        vTaskDelay(50);
    }
 }   
#endif    

#if 0
// MQTT test
void zmain(void)
{
    int ctr = 0;

    printf("\nBoot\n");
    send_mqtt("Zumo01/debug", "Boot");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 

    while(true)
    {
        printf("Ctr: %d, Button: %d\n", ctr, SW1_Read());
        print_mqtt("Zumo01/debug", "Ctr: %d, Button: %d", ctr, SW1_Read());

        vTaskDelay(1000);
        ctr++;
    }
 }   
#endif


#if 0
void zmain(void)
{    
    struct accData_ data;
    struct sensors_ ref;
    struct sensors_ dig;
    
    printf("MQTT and sensor test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Accelerometer Ok...\n");
    }
    
    int ctr = 0;
    reflectance_start();
    while(true)
    {
        LSM303D_Read_Acc(&data);
        // send data when we detect a hit and at 10 second intervals
        if(data.accX > 1500 || ++ctr > 1000) {
            printf("Acc: %8d %8d %8d\n",data.accX, data.accY, data.accZ);
            print_mqtt("Zumo01/acc", "%d,%d,%d", data.accX, data.accY, data.accZ);
            reflectance_read(&ref);
            printf("Ref: %8d %8d %8d %8d %8d %8d\n", ref.L3, ref.L2, ref.L1, ref.R1, ref.R2, ref.R3);       
            print_mqtt("Zumo01/ref", "%d,%d,%d,%d,%d,%d", ref.L3, ref.L2, ref.L1, ref.R1, ref.R2, ref.R3);
            reflectance_digital(&dig);
            printf("Dig: %8d %8d %8d %8d %8d %8d\n", dig.L3, dig.L2, dig.L1, dig.R1, dig.R2, dig.R3);
            print_mqtt("Zumo01/dig", "%d,%d,%d,%d,%d,%d", dig.L3, dig.L2, dig.L1, dig.R1, dig.R2, dig.R3);
            ctr = 0;
        }
        vTaskDelay(10);
    }
 }   

#endif

#if 0
void zmain(void)
{    
    RTC_Start(); // start real time clock
    
    RTC_TIME_DATE now;

    // set current time
    now.Hour = 12;
    now.Min = 34;
    now.Sec = 56;
    now.DayOfMonth = 25;
    now.Month = 9;
    now.Year = 2018;
    RTC_WriteTime(&now); // write the time to real time clock

    while(true)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            printf("%2d:%02d.%02d\n", now.Hour, now.Min, now.Sec);
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }   
#endif


//custom funcation
void progEnd(uint32_t delay){
    bool led = false;
    while(true){
        BatteryLed_Write(led^=1);
        vTaskDelay(delay);
    }    
}






