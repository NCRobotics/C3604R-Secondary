#ifndef ROBOT_h
#define ROBOT_h

#include <YETI_YUKON.h>
#include <STATE.h>

#include <XBOXRECV.h>

#include <YUKON_CytronMD10.h>

#include "Subsystems\Drive.h"
#include "Subsystems\Lift.h"
#include "Subsystems\Claw.h"
#include "Subsystems\BuddyBot.h"

#include "Autonomous\AUTONOMOUS.h"

class ROBOT
{
  public:
    ROBOT(YETI_YUKON &rYukon);

    void Setup();
    void Loop();

    YETI_YUKON &Yukon;

    STATE State;

    //Motors
    YUKON_CytronMD10 DriveRight;
    YUKON_CytronMD10 DriveLeft;
    YUKON_CytronMD10 LiftMotor;
    YUKON_CytronMD10 ClawMotor;
    YUKON_CytronMD10 BuddyBotLift;

    //Subsystems
    DRIVE Drive;
    LIFT Lift;
    CLAW Claw;
    BUDDYBOT BuddyBot;
    AUTONOMOUS Auton;

    //USB Items
    USB Usb;
    
    XBOXRECV Xbox;
    
    //Vars that I decided go here
    bool PrecisionMode = false;
    bool IsArcadeMode = false;
    bool IsNoLimits = false;
    bool IsDebugMode = false;
    float _NextSpeedUpdateMillis = 0;
    int LeftHasBeenLimited = -10;
    int RightHasBeenLimited = -10;
    int DebugModeOutput = 0;
    int RightArrayPos = 0;
    int LeftArrayPos = 0;
    int LiftArrayPos = 0;
    int ClawArrayPos = 0;
    int BuddyBotArrayPos = 0;
    int RightArrayProg = 0;
    int LeftArrayProg = 0;
    int LiftArrayProg = 0;
    int ClawArrayProg = 0;
    int BuddyBotArrayProg = 0;
    bool Recording = false;
    bool RecordMode = false;
    //Array Definitions
    int16_t DriveRightSpeeds [750];
    int16_t DriveLeftSpeeds [750];
    int16_t LiftSpeeds [750];
    int16_t ClawSpeeds [750];
    int16_t BuddyBotSpeeds [750];
    //Speed Variable Definitions   
    int16_t PreviousLeftSpeed = 0;
    int16_t PreviousRightSpeed = 0;
    int16_t CurrentRightSpeed = 0;
    int16_t CurrentLeftSpeed = 0;
    int16_t LiftSpeed = 0;
    int16_t ClawSpeed = 0;
    int16_t BuddyLiftSpeed = 0;
    //Pointer Definitions
    int16_t (*RightArray) = DriveRightSpeeds;

    void ReadStoredAuton()
    {
      //Retriving Values from storage commands will go here
      while(RightArrayProg < 750)
      {
        if(_NextSpeedUpdateMillis < millis())
        {
            _NextSpeedUpdateMillis = millis() + 20;
            CurrentRightSpeed = DriveRightSpeeds[RightArrayProg];
            RightArrayProg++;
            CurrentLeftSpeed = DriveLeftSpeeds[LeftArrayProg];
            LeftArrayProg++;
            LiftSpeed = LiftSpeeds[LiftArrayProg];
            LiftArrayProg++;
            ClawSpeed = ClawSpeeds[ClawArrayProg];
            ClawArrayProg++;
            BuddyLiftSpeed = BuddyBotSpeeds[BuddyBotArrayProg];
            BuddyBotArrayProg++;
        }

            
      }
    }
    
   

    //PIN Declarations
    static const int _DriveRightPWM = 0;
    static const int _DriveRightDir = 17; //14;
    static const bool _DriveRightReversed = true;
        
    static const int _DriveLeftPWM = 1;
    static const int _DriveLeftDir = 14;//17;
    static const bool _DriveLeftReversed = false;
        
    static const int _LiftMotorPWM = 4;//2;
    static const int _LiftMotorDir = 26;//21;
    static const bool _LiftMotorReversed = false;
        
    static const int _ClawMotorPWM = 3;
    static const int _ClawMotorDir = 21;
    static const bool _ClawMotorReversed = false;
        
    static const int _BuddyBotLiftPWM = 3;//4;
    static const int _BuddyBotLiftDir = 22;//26;
    static const bool _BuddyBotLiftReversed = false;
        

    static const uint8_t _Button0 = 0;
    static const uint8_t _LEDBuiltIn = 25;
    static const uint8_t _AutonLightSensor = 1;
    static const uint8_t LeftEncoderPin1 = 26;
    static const uint8_t LeftEncoderPin2 = 27;

    static const uint16_t _AutonLightSensorThreshold = 3000; //Value 0 - 4095

  private:
      long _NextDisplayMillis = 0;
      long _NextMotorControllerWriteMillis = 0;
      long _NextGetPrevSpeed = 0;
};
#endif

/*class Encoder;

    //Read the Encoders
    void EncRead() 
    {
        long oldPosition = 0;
        Encoder myEnc(26, 27);
        long NewLeftPos = myEnc.read();
    if (NewLeftPos != oldPosition) 
        oldPosition = NewLeftPos;
        Serial.println(NewLeftPos);
    }*/

