#include <Wire.h>
#include <PID_v1.h>
#include <LMotorController.h>
#include <MPU6050_wrap.h>

// INIT VARIABLES
double kp = 70, ki = 240, kd = 1.9;
double setpoint = 175;
float ypr[3];           // [yaw, pitch, roll]   
double input, output;
float motorconstA = 0.8;
float motorconstB = 0.8;
int sampleTime = 1;
float comp_filtercoeff = 0.2;

// SETUP PINS
int MIN_ABS_SPEED = 100;
int PWM_MAX = 255;
int ENA = 3;
int IN1 = 4;
int IN2 = 8;
int IN3 = 5;
int IN4 = 7;
int ENB = 6;

//MPU with 
MPU6050 imu(Wire, comp_filtercoeff, 1-comp_filtercoeff);

//PID
PID pid(&input, &output, &setpoint, kp, ki, kd, DIRECT);

//MOTOR CONTROLLER
LMotorController motorController(ENA, IN1, IN2, ENB, IN3, IN4, motorconstA, motorconstB);

// INITIALIZE SETUP
void setup()
{
    Serial.begin(115200);

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    Wire.begin();
    imu.begin();
    imu.calcGyroOffsets(true);

    //setup PID
    pid.SetMode(AUTOMATIC);
    pid.SetSampleTime(sampleTime);
    pid.SetOutputLimits(-PWM_MAX, PWM_MAX);  
}

// MAIN LOOP
void loop()
{
        imu.update();
        ypr[0] = imu.getAngleX();
        ypr[1] = imu.getAngleY();
        ypr[2] = imu.getAngleZ();
        input = ypr[1];

        pid.Compute();
        motorController.move(output, MIN_ABS_SPEED);        
}
