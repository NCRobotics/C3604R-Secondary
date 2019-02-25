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
#include "Preferences.h"

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
    Preferences preferences;
    XBOXRECV Xbox;
    
    //Variables for different modes
    bool PrecisionMode = false;
    bool IsArcadeMode = false;
    bool IsNoLimits = false;
    bool IsDebugMode = false;
    //Vars for speed checks
    float _NextSpeedUpdateMillis = 0;
    //Misc vars from stuff I added
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
      //This will start the storage access
      preferences.begin("Recording", false);
      //These preferences lines will get the speeds stored in an array
      preferences.getBytes("DriveRightSpeeds", &DriveRightSpeeds, sizeof(DriveRightSpeeds));
      preferences.getBytes("DriveLeftSpeeds", &DriveLeftSpeeds, sizeof(DriveLeftSpeeds));
      preferences.getBytes("LiftSpeeds", &LiftSpeeds, sizeof(LiftSpeeds));
      preferences.getBytes("ClawSpeeds", &ClawSpeeds, sizeof(ClawSpeeds));
      preferences.getBytes("BuddyBotSpeeds", &BuddyBotSpeeds, sizeof(BuddyBotSpeeds));
      while(RightArrayProg < 750)
      { //This will only run every 20 milliseconds
        if(_NextSpeedUpdateMillis < millis())
        {   
            _NextSpeedUpdateMillis = millis() + 20;
            //This line sets the speeds to what is in the array
            CurrentRightSpeed = DriveRightSpeeds[RightArrayProg];
            //This line sets the value in the array that is being read. 
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
        if(RightArrayProg > 750)
        { //This will end the storage session when the arrays are finished reading
          preferences.end();
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
//Some test stuff that didn't work
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

