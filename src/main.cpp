/*TODO:
*check if speed is correct for intake
*
*/

#include "main.h"
#include "pros/misc.hpp"
#include <memory>
#include <string>
#define expansionPiston 'A'

Motor intakeMotor{2, true, okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees};
Motor catapultMotor{1, true, okapi::AbstractMotor::gearset::red, okapi::AbstractMotor::encoderUnits::degrees};
Motor rollerMotor{5, true, okapi::AbstractMotor::gearset::red, okapi::AbstractMotor::encoderUnits::degrees};

auto r = RotationSensor(15, true);
pros::ADIDigitalIn catapultLimit('H');

Controller controller;
pros::ADIDigitalOut expansion(expansionPiston);
ControllerButton launchCatapultButton(ControllerDigital::L1);
ControllerButton retractCatapultButton(ControllerDigital::L2);
ControllerButton intakeButton(ControllerDigital::R2);
ControllerButton reverseIntakeButton(ControllerDigital::R1);
ControllerButton rollerButton(ControllerDigital::X);
ControllerButton reverseRollerButton (ControllerDigital::B);//may not be needed/wanted
ControllerButton expansionButton(ControllerDigital::down);
//ControllerButton expansionFailsafeButton(ControllerDigital::down);

bool armset = true;

std::shared_ptr<OdomChassisController> drive =
    ChassisControllerBuilder()
        .withMotors({-11, 12, -13}, {18, -19, 20})
        // Green gearset, 4 in wheel diam, 11.5 im wheel track
        // 36 to 60 gear ratio
        .withGains(
            {0.005, 0, 0.00},    // Distance controller gains
            {0.002, 0.00401, 0}, // Turn controller gains  ***Lower the I value by 0.0002
            {0.002, 0, 0.0001}   // Angle controller gains (helps drive straight)
            )
        .withMaxVelocity(100)
        .withDimensions({AbstractMotor::gearset::blue, (40.0 / 33.0)}, {{3.375_in, 14.5_in}, imev5BlueTPR})
        .withOdometry(StateMode::CARTESIAN)
        .buildOdometry(); // build an odometry chassis

std::shared_ptr<AsyncPositionController<double, double>> catapultControl =
    AsyncPosControllerBuilder()
        .withMotor(catapultMotor)
        .build();
std::shared_ptr<AsyncController<double, double>> catapultVelocityControl =
    AsyncVelControllerBuilder()
        .withMotor(catapultMotor)
        .build();

void launchCatapult()
{
   while (catapultLimit.get_value())
   {
      catapultMotor.moveVelocity(-100);
   }
   catapultMotor.moveVelocity(0);
}
void retractCatapult()
{
   while (!catapultLimit.get_value())
   {
      catapultMotor.moveVelocity(-70);
   }
   catapultMotor.moveVelocity(0);
}

void on_center_button()
{
}

void initialize()
{
   catapultMotor.setBrakeMode(AbstractMotor::brakeMode(2));
   // catapultMotor.tarePosition();
   // catapultMotor.moveVelocity(-100);
   // pros::delay(700);
   // catapultMotor.moveVelocity(0);
   //  drive->moveDistance({-6.5_in});
   //  pros::delay(1000);
}

void disabled()
{

   catapultMotor.setBrakeMode(AbstractMotor::brakeMode(2));
   // catapultMotor.moveVelocity(-100);
   // pros::delay(700);
   // catapultMotor.moveVelocity(0);
   // drive->moveDistance({-6.5_in});
   // pros::delay(1000);
}

void competition_initialize()
{
   catapultMotor.setBrakeMode(AbstractMotor::brakeMode(2));
   // catapultMotor.moveVelocity(-100);
   // pros::delay(700);
   // catapultMotor.moveVelocity(0);
   // drive->moveDistance({-6.5_in});
   // pros::delay(1000);
}

double error;
/**
 * @brief 
 * 
 * @param distance 
 */
void testDrivePID(int distance){

}
/**
 * @brief 
 * The hoop is 114 inches y, 40 inches on the x from starting.
 */
void autonomous()
{
   drive->setState({56_in,0_ft,0_deg});

   
   
   //intakeMotor.moveRelative(-180, -100);
   //retractCatapult();
   /**
    * This code will do the quater rotation to be under 18 inches
    */
   // catapultMotor.moveVelocity(-100);
   // pros::delay(700);
   // catapultMotor.moveVelocity(0);
   /*
   pros::delay(500);
   drive->moveDistanceAsync({4_in});
   pros::delay(500);
   rollerMotor.moveVelocity(-30);
   pros::delay(500);
   rollerMotor.moveVelocity(0);
   pros::delay(500);
   drive->moveDistance({-6.5_in});
   drive->waitUntilSettled();
   */
   /**
    * Alternitive if -177 deg doesn't work
    */
   // drive->turnToPoint({-1_ft, -9_ft});
   /*
   drive->turnAngle(-177_deg);
   drive->waitUntilSettled();
   drive->moveDistance({-2.5_in});
   drive->waitUntilSettled();
   */
   /**
    * These delays can be adjusted as needed.
    */
                     // drive->setState({0_ft,0_ft,0_deg});
                     // drive->driveToPoint({0_ft,1.5_ft});
                     // retractCatapult();
                     // pros::delay(500);
                     // launchCatapult();
                     // pros::delay(500);
                     // retractCatapult();
   // pros::delay(2000);
   // intakeMotor.moveVelocity(200);
   // pros::delay(2000);
   // intakeMotor.moveVelocity(0);
   // pros::delay(2000);
   // launchCatapult();
   // pros::delay(1000);
   // retractCatapult();
}
double degrees;
std::string str;

void opcontrol()
{
   while (true)
   {
      drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightX) * 0.75);

      if (intakeButton.changedToPressed() && catapultLimit.get_value())
      {
         intakeMotor.moveVelocity(-400);
      }
      else if (intakeButton.changedToReleased()&& catapultLimit.get_value())
      {
         intakeMotor.moveVoltage(0);
      }

      if (reverseIntakeButton.changedToPressed()&& catapultLimit.get_value())
      {
         intakeMotor.moveVelocity(400);
      }
      else if (reverseIntakeButton.changedToReleased()&& catapultLimit.get_value())
      {
         intakeMotor.moveVoltage(0);
      }

      if (rollerButton.changedToPressed())
      {
         rollerMotor.moveVelocity(-200);
      }
      else if (rollerButton.changedToReleased())
      {
         rollerMotor.moveVoltage(0);
      }

      if (reverseRollerButton.changedToPressed())
      {
         rollerMotor.moveVelocity(200);
      }
      else if (reverseRollerButton.changedToReleased())
      {
         rollerMotor.moveVoltage(0);
      }

      if (retractCatapultButton.isPressed())
      {
         // degrees = r.controllerGet();
         // str = std::to_string(degrees);
         // controller.setText(1, 1, str);
         // r.reset();
         // while(r.controllerGet()<70){
         //	catapultMotor.moveVelocity(-50);
         // }
         // catapultControl->setTarget(-650);
         // degrees = r.controllerGet();
         // str = std::to_string(degrees);
         // controller.setText(1, 1, str);
         armset = false;
         /*while(!catapultLimit.get_value()){
            catapultMotor.moveVelocity(-100);
            catapultVelocityControl->setTarget(-100);
         }*/
         // catapultMotor.moveVelocity(0);
         // catapultVelocityControl->setTarget(0);
      }
      if (!armset && !catapultLimit.get_value())
      {
         catapultMotor.moveVelocity(-100);
      }
      else
      {
         catapultMotor.moveVelocity(0);
         catapultMotor.setBrakeMode(AbstractMotor::brakeMode(2));

         armset = true;
      }
      if (launchCatapultButton.isPressed())
      {
         while (catapultLimit.get_value())
         {
            catapultMotor.moveVelocity(-100);
            // catapultVelocityControl->setTarget(-100);
         }
         catapultMotor.moveVelocity(0);
         // catapultVelocityControl->setTarget(0);
         // catapultMotor.moveRelative(-50, -50);
         /*catapultControl->setTarget(-800);
         catapultControl->waitUntilSettled();
         pros::delay(500);
         catapultControl->setTarget(-1050+catapultAdust);
         catapultControl->waitUntilSettled();
         pros::delay(1000);
         catapultMotor.tarePosition();
         catapultAdust-=100;*/
         // while(r.get()<85){
         //	catapultMotor.moveVelocity(200);
         // }
      }
      if (expansionButton.isPressed())
      {
         expansion.set_value(true);
         pros::delay(250);
         expansion.set_value(false);
      }
   }
}