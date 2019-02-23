 #include "ROBOT.h"
 #include "Preferences.h"
 Preferences preferences;
ROBOT::ROBOT(YETI_YUKON &rYukon) : Yukon(rYukon),
     DriveRight(_DriveRightPWM, &Yukon.PWM, _DriveRightDir, _DriveRightReversed), 
     DriveLeft(_DriveLeftPWM, &Yukon.PWM, _DriveLeftDir, _DriveLeftReversed), 
     LiftMotor(_LiftMotorPWM, &Yukon.PWM, _LiftMotorDir, _LiftMotorReversed), 
     ClawMotor(_ClawMotorPWM, &Yukon.PWM, _ClawMotorDir, _ClawMotorReversed), 
     BuddyBotLift(_BuddyBotLiftPWM, &Yukon.PWM, _BuddyBotLiftDir, _BuddyBotLiftReversed), 
    Drive(*this),
    Lift(*this),
    Claw(*this),
    BuddyBot(*this),
                                   Auton(*this),
    
        Xbox(&Usb)
    

{
}

void ROBOT::Setup()
{
    Usb.Init();
    DriveRight.Init();
    DriveLeft.Init();
    LiftMotor.Init();
    ClawMotor.Init();
    BuddyBotLift.Init();
    preferences.begin("Recording", false);

    pinMode(_Button0, INPUT_PULLUP);
    pinMode(_LEDBuiltIn, OUTPUT);
    digitalWrite(_LEDBuiltIn, LOW);
}

    bool Recording = false;
    bool RecordMode = false;
    bool TimerStarted = false;
    long AutonTimer = 0;
    long AutonStopTime = 0;
    int16_t MovementTime = 0;
    String Comma = ",";
    String DriveAsyncCommand = "Robot.Drive.ForAsync(";
    String Endparen = ")";
    int16_t RecordRightSpeed = 0;
    int16_t RecordLeftSpeed = 0;
    int16_t RecordLiftSpeed = 0;
    int16_t RecordClawSpeed = 0;
    int16_t RecordBuddyBotSpeed = 0;
    int16_t RightAverage = 0;
    int16_t LeftAverage = 0;
    int16_t LiftAverage = 0;
    int16_t BuddyBotAverage = 0;
    int16_t NumOfRightAverages = 0;
    int16_t NumOfLeftAverages = 0;
    int16_t NumOfLiftAverages = 0;
    int16_t NumOfBuddyBotAverages = 0;
    int16_t RightAverageFinal = 0;
    int16_t LeftAverageFinal = 0;
    int16_t LiftAverageFinal = 0;
    int16_t BuddyBotAverageFinal = 0;
    int16_t DriveRightSpeeds [750];
    int RightArrayPos = 0;
    int LeftArrayPos = 0;
    int LiftArrayPos = 0;
    int ClawArrayPos = 0;
    int BuddyBotArrayPos = 0;
    int16_t DriveLeftSpeeds [750];
    int16_t LiftSpeeds [750];
    int16_t ClawSpeeds [750];
    int16_t BuddyBotSpeeds [750];

    //Function declaration for auton recording   
    void ReadStoredAuton()
    {
        
    }
    void StoreAuton()
    {

    }

    
void ROBOT::Loop()
{
     //Read The Controller
    Usb.Task();
    
        if (Xbox.XboxReceiverConnected)
        {
        for (uint8_t i = 0; i < 4; i++)
        {
        if (Xbox.Xbox360Connected[i])
        {
        int16_t CurrentRightSpeed  = (Yukon.XBOXJoystickTo255(Xbox.getAnalogHat(LeftHatY, i), 7500));
        int16_t CurrentLeftSpeed = (Yukon.XBOXJoystickTo255(Xbox.getAnalogHat(RightHatY, i), 7500));
        int16_t ClawSpeed = (Xbox.getButtonPress(A))*255;
       if (_NextGetPrevSpeed < millis())
        {
        _NextGetPrevSpeed = millis() + 30;
        CurrentLeftSpeed = PreviousLeftSpeed;
        CurrentRightSpeed = PreviousRightSpeed;
        }

        /*if (IsNoLimits == false)
        {    
            if ((CurrentLeftSpeed - PreviousLeftSpeed) > 2)
            {   
                CurrentLeftSpeed = (PreviousLeftSpeed + 2);
                LeftHasBeenLimited = 0;
            }

            if ((CurrentRightSpeed - PreviousRightSpeed) > 2)
            {
                CurrentRightSpeed = (PreviousRightSpeed + 2);
                RightHasBeenLimited = 0;
            }
        }*/
        int16_t LiftSpeed = (Xbox.getButtonPress(R2, i) - Xbox.getButtonPress(L2, i));
        int16_t BuddyLiftSpeed = ((Xbox.getButtonPress(R1, i)*255) - (Xbox.getButtonPress(L1, i)*255));  
        if(IsArcadeMode)
        {
            CurrentRightSpeed =  (Yukon.XBOXJoystickTo255(Xbox.getAnalogHat(LeftHatX, i), 7500)) + (Yukon.XBOXJoystickTo255(Xbox.getAnalogHat(LeftHatY, i), 7500));
            CurrentLeftSpeed =   (Yukon.XBOXJoystickTo255(Xbox.getAnalogHat(LeftHatY, i), 7500)) -  (Yukon.XBOXJoystickTo255(Xbox.getAnalogHat(LeftHatX, i), 7500));
        }
        
        if(PrecisionMode)
        {
            CurrentLeftSpeed = CurrentLeftSpeed * .5;
            CurrentRightSpeed = CurrentRightSpeed  * .5;
        }

        while(RecordMode)
        {  
            while (Recording)
            {
                if(_NextSpeedUpdateMillis < millis())
                {
                    _NextSpeedUpdateMillis = millis() + 20;
                    DriveRightSpeeds [RightArrayPos] = CurrentRightSpeed;
                    DriveLeftSpeeds  [LeftArrayPos] = CurrentLeftSpeed;
                    LiftSpeeds       [LiftArrayPos] = LiftSpeed;
                    ClawSpeeds       [ClawArrayPos] = ClawSpeed;
                    BuddyBotSpeeds   [BuddyBotArrayPos] = BuddyLiftSpeed;
                    RightArrayPos++;
                    LeftArrayPos++;
                    LiftArrayPos++;
                    ClawArrayPos++;
                    BuddyBotArrayPos++;
                    if(RightArrayPos > 750)
                    {
                        RightArrayPos = 750;
                    }
                    if(LeftArrayPos > 750)
                    {
                        LiftArrayPos = 750;
                    }
                    if(LiftArrayPos > 750)
                    {
                        LiftArrayPos = 750;
                    }
                    if(ClawArrayPos > 750)
                    {
                        ClawArrayPos = 750;
                    }
                    if(BuddyBotArrayPos > 750)
                    {
                        BuddyBotArrayPos = 750;
                    }
                }
              
            }
        }
    




        //Press the back button to engage Debug Mode. 
        //Press it again to toggle the output to the display. 
        if(IsDebugMode)
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(1);

            if (DebugModeOutput == 0)
            Yukon.OLED.clearDisplay();

            if (DebugModeOutput == 1)
            { Yukon.OLED.print("Current Right Speed");
             Yukon.OLED.println (CurrentRightSpeed);}
           
           if (DebugModeOutput == 2)
           {Yukon.OLED.print("Current Left Speed");
            Yukon.OLED.println(CurrentLeftSpeed);}

            if (DebugModeOutput == 3)
            {Yukon.OLED.print("Previous Right Speed");
            Yukon.OLED.println (PreviousRightSpeed);}

            if (DebugModeOutput == 4)
            {Yukon.OLED.print("Previous Left Speed");
            Yukon.OLED.println(PreviousLeftSpeed);}

            if (DebugModeOutput == 5)
            {Yukon.OLED.print ("Light Sensor Threshold");
             Yukon.OLED.println (_AutonLightSensorThreshold);}

            if (DebugModeOutput == 7)
            {
                DebugModeOutput = 0;
            }
        }

        Drive.OISetSpeed(CurrentRightSpeed, CurrentLeftSpeed);
        Lift.OISetSpeed(LiftSpeed*0.5);
        Claw.OISetSpeed(ClawSpeed);
        BuddyBot.OISetSpeed(BuddyLiftSpeed);
        Serial.println((Xbox.getButtonPress(R1, i)*255) - (Xbox.getButtonPress(L1, i)*255));


        if (Xbox.getButtonClick(LEFT))
        Auton.QueuePrev();
        
        if (Xbox.getButtonClick(RIGHT))
        Auton.QueueNext();
        
        if (Xbox.getButtonClick(DOWN))
        Auton.ToggleArmed();
        
        if (Xbox.getButtonClick(Y))
        PrecisionMode = !PrecisionMode;

        if (Xbox.getButtonClick(X))
        Auton.LaunchQueued();

        if (Xbox.getButtonClick(B))
        IsArcadeMode = !IsArcadeMode;
        
        if (Xbox.getButtonClick(BACK))
        RecordMode =!RecordMode;

        if (Xbox.getButtonClick(START))
        Recording =!Recording;
        
        if (Xbox.getButtonClick(L3))
        IsDebugMode = !IsDebugMode;
        
        if (Xbox.getButtonClick(R3))
        DebugModeOutput = (DebugModeOutput) + 1;
        

        if (Xbox.getButtonClick(XBOX))
        Auton.ToggleLockArmed();
        }
        }
        }

        if(digitalRead(_Button0) == LOW)
        {
            digitalWrite(_LEDBuiltIn, HIGH);
            Yukon.SetupWIFI();
            delay(1000);
        }

     //Read The Sensors 
    uint16_t LightSensorVal = analogRead(33);
    State.AutonLightSensorActive = (LightSensorVal <= _AutonLightSensorThreshold);
    Serial.println(LightSensorVal); 

    //Write To Motor Controllers
    if (_NextMotorControllerWriteMillis < millis())
    {
    _NextMotorControllerWriteMillis = millis() + 20; 
    DriveRight.SetMotorSpeed(State.DriveRightSpeed);
    DriveLeft.SetMotorSpeed(State.DriveLeftSpeed);
    LiftMotor.SetMotorSpeed(State.LiftMotorSpeed);
    ClawMotor.SetMotorSpeed(State.ClawMotorSpeed);
    BuddyBotLift.SetMotorSpeed(State.BuddyBotLiftSpeed); 
    }

    //Write the Display
    if (_NextDisplayMillis < millis())
    {
        _NextDisplayMillis = millis() + 250;

        if(Auton.IsArmLocked())
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);
            Yukon.OLED.println("LOCKED! ");
            Yukon.OLED.setTextSize(1);
            Yukon.OLED.print(Auton.QueuedProgramName());
            Yukon.OLED.display();
        }
        else if (Auton.IsArmed())
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);
            Yukon.OLED.print("ARMED ");
            Yukon.OLED.println(LightSensorVal);
            Yukon.OLED.setTextSize(1);
            Yukon.OLED.print(Auton.QueuedProgramName());

            Yukon.OLED.display();
        }
        else if(Auton.QueuedProgramName() != "")
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(2);

            Yukon.OLED.print(Auton.QueuedProgramName());

            Yukon.OLED.display();
        }
        else if(PrecisionMode)
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(1.1);
            Yukon.OLED.print("Precsion Mode");
            Yukon.OLED.display();
        }
        
        else if(IsArcadeMode)
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(1.1);
            Yukon.OLED.print("Arcade Mode!");
            Yukon.OLED.display();
        }
        else if (IsDebugMode)
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(1);
            Yukon.OLED.print("Debug Mode!");
            Yukon.OLED.display();
        }
        else
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(1);
            Yukon.OLED.println("NC Robotics!");
            Yukon.OLED.println("C3604R");
            Yukon.OLED.display();
        }
    }
}
