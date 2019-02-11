#include "AUTONOMOUS.h"
#include "ROBOT.h"

AUTONOMOUS::AUTONOMOUS(ROBOT &refRobot) : Robot(refRobot)
{
    _MaxProgramNumber = 4;
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

     _RunningAuton = "";
    delay(20);
}
//You need to travel about 1140 ms to go a whole tile
void AUTONOMOUS::ExecAuton1()
{
    //Hits low flag and drives on the platform
    //Red Close Tile
    Robot.Drive.ForAsync(3000, 100, 100, 100);
    Robot.Drive.ForAsync(4300, -100, -100, 100);
    Robot.Drive.ForAsync(1500, -100, 100, 100);
    Robot.Drive.ForAsync(2000, 255, 255, 100);

  
}
void AUTONOMOUS::ExecAuton2()
    //Blue close tile
{   Robot.Drive.ForAsync(3000, 100, 100, 100);
    Robot.Drive.ForAsync(4300, -100, -100, 100);
    Robot.Drive.ForAsync(1500, 100, -100, 100);
    Robot.Drive.ForAsync(2000, 255, 255, 100);
    

}
void AUTONOMOUS::ExecAuton3()
{   //Red far tile
    Robot.Drive.ForAsync(900, 100, 100, 100);
    Robot.Drive.ForAsync(1500, 100, -100, 100);
    Robot.Drive.ForAsync(2000,255,255,100);

}
void AUTONOMOUS::ExecAuton4()
{   //Blue far tile
    Robot.Drive.ForAsync(900, 100, 100, 100);
    Robot.Drive.ForAsync(1500, -100, 100, 100);
    Robot.Drive.ForAsync(2000,255,255,100);
}