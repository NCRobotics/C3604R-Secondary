 #include "ROBOT.h"
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

    pinMode(_Button0, INPUT_PULLUP);
    pinMode(_LEDBuiltIn, OUTPUT);
    digitalWrite(_LEDBuiltIn, LOW);
}
bool PrecisionMode = false;
bool IsArcadeMode = false;
bool IsNoLimits = false;
bool IsDebugMode = false;
bool AutonRecord = false;
float _NextSpeedUpdateMillis = 0;
int LeftHasBeenLimited = -10;
int RightHasBeenLimited = -10;
int DebugModeOutput = 0;
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
int16_t PreviousLeftSpeed = 0;
int16_t PreviousRightSpeed = 0;

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
        int16_t BuddyLiftSpeed =(Xbox.getButtonPress(R1, i)*255) - (Xbox.getButtonPress(L1, i)*255);  
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
        //Program to record autonomous is in progress. It will print out the time and the average of the speeds. 
        //You will only be able to do one thing at a time and not go backwards without reseting. 
        //This is because of how I have coded this. 
        if(AutonRecord)
        {
            if(_NextSpeedUpdateMillis < millis())
            {
               _NextSpeedUpdateMillis = millis() + 20;
               RecordRightSpeed = CurrentRightSpeed;
               RecordLeftSpeed = CurrentLeftSpeed;
               RecordLiftSpeed = LiftSpeed;
               RecordBuddyBotSpeed = BuddyLiftSpeed;
            }
            while(AutonRecord)
            {
            Serial.println("Right Average Speed");
            Serial.println(RightAverageFinal);
            Serial.println("Left Average Speed");
            Serial.println(LeftAverageFinal);
            Serial.println("Lift Average Speed");
            Serial.println(LiftAverageFinal);
            Serial.println("Buddy Bot Lift Average Speed");
            Serial.println(BuddyBotAverageFinal);
            delay(5000);
            }
            if(CurrentRightSpeed < 0 or CurrentRightSpeed > 0)
            {
                if(_NextSpeedUpdateMillis < millis());
                RightAverage = CurrentRightSpeed + RightAverage;
                NumOfRightAverages++;
                RightAverageFinal = RightAverage/NumOfRightAverages;
            }
            if(CurrentLeftSpeed < 0 or CurrentLeftSpeed > 0)
            {
                if(_NextSpeedUpdateMillis < millis());
                LeftAverage = CurrentLeftSpeed + LeftAverage;
                NumOfLeftAverages++;
                LeftAverageFinal = LeftAverage/NumOfLeftAverages;
            }
            if(LiftSpeed < 0 or LiftSpeed > 0)
            {
                if(_NextSpeedUpdateMillis < millis());
                LiftAverage = LiftSpeed + LiftAverage;
                NumOfLiftAverages++;
                LiftAverageFinal = LiftAverage/NumOfLiftAverages;
            }
            if(BuddyLiftSpeed < 0 or BuddyLiftSpeed > 0)
            {
                if(_NextSpeedUpdateMillis < millis());
                BuddyBotAverage = BuddyLiftSpeed + BuddyBotAverage;
                NumOfBuddyBotAverages++;
                BuddyBotAverageFinal = BuddyBotAverage/NumOfLiftAverages;
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
        Claw.OISetSpeed(Xbox.getButtonPress(A));
        BuddyBot.OISetSpeed(BuddyLiftSpeed);


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
        
        if (Xbox.getButtonClick(START))
        AutonRecord =!AutonRecord;

        if (Xbox.getButtonClick(BACK))
        IsDebugMode = !IsDebugMode;
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
        }/*
        else if (LeftHasBeenLimited = (1))
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(1.1);
            Yukon.OLED.print("Acceleration");
            Yukon.OLED.print("Has Been");
            Yukon.OLED.print("Limited (L)");
            Yukon.OLED.display();
        }
        else if (RightHasBeenLimited = (1))
        {
            Yukon.OLED.clearDisplay();
            Yukon.OLED.setCursor(0, 0);
            Yukon.OLED.setTextSize(1.1);
            Yukon.OLED.print("Acceleration");
            Yukon.OLED.print("Has Been");
            Yukon.OLED.print("Limited (R)");            
            Yukon.OLED.display();
        }*/
        
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
