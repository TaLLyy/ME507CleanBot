/** @file motor_driver.cpp
 *    This file contains the implementation of two motor drivers that can control
 *    the PWM and direction of two seperate motors
 *  @author Weston Montgomery 
 *  @author Alex Haduong 
 *  @author Aris Recidoro
 *  @date 05 Nov 2020
 */


#include <Arduino.h>
#include <PrintStream.h>
#include <cmath>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "taskshare.h"
#include "motor_driver.h"



/** @brief   Function which runs a motor.
 *  @details This function runs at precise interfals using @c vTaskDelayUntil() and
 *           runs a motor whose duty cycle is controlled by a
 *           power level sent from the UI task. The simulation is just a very
 *           simple implementation of a first-order filter. 
 *  @param   PHASE_PIN is PHASE PIN for DRV8838 Motor Carrier
 *  @param   ENABLE_PIN is ENABLE PIN for DRV8838 Motor Carrier
 */
void Motor_Driver::SetPins(uint8_t PHASE_PIN, uint8_t ENABLE_PIN)
{
    uint8_t PIN_MD1_IN1 = PHASE_PIN;
    uint8_t PIN_MD1_IN2 = ENABLE_PIN;
    pinMode(PIN_MD1_IN2, OUTPUT);   // initializing pin 3 as a pin for output
    pinMode(PIN_MD1_IN1, OUTPUT);   // initializing pin 5 as a pin for output
}

/** @brief   Function which runs a motor.
 *  @details This function runs at precise interfals using @c vTaskDelayUntil() and
 *           runs a motor whose duty cycle is controlled by a
 *           power level sent from the UI task. The simulation is just a very
 *           simple implementation of a first-order filter. 
 *  @param   duty_cycle_var a variable user inputs to change speed of motor
 */
void Motor_Driver::ChangeSpeed(uint8_t duty_cycle_var)
{
    float sim_speed = 1; // Variable holding the simulated motor speed
    float sim_A = 0.99; // constant which controls the simulated time constant of the motor
    float sim_B = 1.0 - sim_A; // constant which controls the simulated time constant of the motor
    const TickType_t sim_period = 50;         // RTOS ticks (ms) between runs

    // Initialise the xLastWakeTime variable with the current time.
    // It will be used to run the task at precise intervals
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {   
       
        if (duty_cycle_var < 0)
        {
            sim_speed = sim_speed * sim_A + abs(duty_cycle_var) * sim_B; // Calculate the next motor speed
            //digitalWrite(PIN_MD1_IN1, 0);                    // setting IN1 to 0 drives motor forward
            digitalWrite(PIN_MD1_IN1, 0);
            // digitalWrite(PIN_MD1_IN2, 1);
            analogWrite(PIN_MD1_IN2, (int32_t)sim_speed); // setting IN2 to duty cycle controls pwm% speed  
        }
        else if (duty_cycle_var == 0)
        {
            digitalWrite(PIN_MD1_IN1, 1);
            analogWrite(PIN_MD1_IN2, 1);
        }
        else
        {
            sim_speed = sim_speed * sim_A + abs(duty_cycle_var) * sim_B; // Calculate the next motor speed
            //digitalWrite(PIN_MD1_IN1, 0);                    // setting IN1 to 0 drives motor forward
            digitalWrite(PIN_MD1_IN1, 1);
            // digitalWrite(PIN_MD1_IN2, 1);
            analogWrite(PIN_MD1_IN2, 255-(int32_t)sim_speed); // setting IN2 to duty cycle controls pwm% speed  
        }

        
        // This type of delay waits until it has been the given number of RTOS
        // ticks since the task previously began running. This prevents timing
        // inaccuracy due to not accounting for how long the task took to run
        vTaskDelayUntil (&xLastWakeTime, sim_period);
    }
}
