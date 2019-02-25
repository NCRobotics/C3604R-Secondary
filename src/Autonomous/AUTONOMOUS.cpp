#include "AUTONOMOUS.h"
#include "ROBOT.h"

AUTONOMOUS::AUTONOMOUS(ROBOT &refRobot) : Robot(refRobot)
{
    _MaxProgramNumber = 5;
}

void AUTONOMOUS::Loop()
{
    if (_AutonARMED)
    {
        if (Robot.State.AutonLightSensorActive)
        {
            LaunchQueued();
        }
    }

    if (_RunningAuton == "Auton1") ExecAuton1();
    else if(_RunningAuton == "Auton2") ExecAuton2();
    else if(_RunningAuton == "Auton3") ExecAuton3();
    else if(_RunningAuton == "Auton4") ExecAuton4();
    else if (_RunningAuton == "Auton5") ExecAuton5();

     _RunningAuton = "";
    delay(20);
}
void AUTONOMOUS::ExecAuton1()
{
    //Hits low flag and drives on the platform
    //Red Close Tile
    Robot.Drive.ForAsync(3000, 100, 100, 100);//Drives forward to hit flag
    Robot.Drive.ForAsync(4300, -100, -100, 100);//Drives backward
    Robot.Drive.ForAsync(1500, -100, 100, 100);//Turns
    Robot.Drive.ForAsync(2000, 255, 255, 100);//Climbs platform

  
}
void AUTONOMOUS::ExecAuton2()
    //Hits low flag and drives on the platform
    //Blue close tile
{   Robot.Drive.ForAsync(3000, 100, 100, 100);//Drives forward to hit flag
    Robot.Drive.ForAsync(4300, -100, -100, 100);//Drives backward
    Robot.Drive.ForAsync(1500, 100, -100, 100);//Turns
    Robot.Drive.ForAsync(2000, 255, 255, 100);//Climbs platform
    

}
void AUTONOMOUS::ExecAuton3()
{   //Drives on platform
    //Red far tile
    Robot.Drive.ForAsync(900, 100, 100, 100);//Drives forward
    Robot.Drive.ForAsync(1500, 100, -100, 100);//Turns
    Robot.Drive.ForAsync(2000,255,255,100);//Drives on platform

}
void AUTONOMOUS::ExecAuton4()
{   //Drives on platform
    //Blue far tile
    Robot.Drive.ForAsync(900, 100, 100, 100);//Drives forward
    Robot.Drive.ForAsync(1500, -100, 100, 100);//Turns
    Robot.Drive.ForAsync(2000,255,255,100);//Drives on platform
}
//Do not touch this. This is for your previously recorded auton.
void AUTONOMOUS::ExecAuton5()
{
    Robot.ReadStoredAuton();    
}