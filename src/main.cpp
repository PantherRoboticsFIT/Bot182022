/*
To do:
	*check if speed is correct for intake
*/

#include "main.h"
#include <memory>
#include <string>
#define expansionPiston 'A'
static int catapultAdust = 0;

Motor intakeMotor {14,true,okapi::AbstractMotor::gearset::green,okapi::AbstractMotor::encoderUnits::degrees};
Motor catapultMotor {4,true,okapi::AbstractMotor::gearset::red,okapi::AbstractMotor::encoderUnits::degrees};
Motor rollerMotor {5,true,okapi::AbstractMotor::gearset::red,okapi::AbstractMotor::encoderUnits::degrees};

auto r = RotationSensor(15,true);
pros::ADIDigitalIn catapultLimit ('H');

Controller controller;
pros::ADIDigitalOut expansion (expansionPiston);
ControllerButton launchCatapultButton (ControllerDigital::L1);
ControllerButton retractCatapultButton (ControllerDigital::L2);
ControllerButton intakeButton (ControllerDigital::R2);   
ControllerButton reverseIntakeButton (ControllerDigital::R1); 
ControllerButton rollerButton (ControllerDigital::X);
//ControllerButton reverseRollerButton (ControllerDigital::B);//may not be needed/wanted
ControllerButton expansionButton (ControllerDigital::up);
ControllerButton expansionFailsafeButton (ControllerDigital::down);

bool armset =true;

std::shared_ptr<OdomChassisController> drive =
	ChassisControllerBuilder()
		.withMotors({-1, 2,-3}, {11, -12,13})
		// Green gearset, 4 in wheel diam, 11.5 im wheel track
		// 36 to 60 gear ratio
			.withGains(
        	{0.005, 0, 0.00}, // Distance controller gains
        	{0.002, 0.00401, 0}, // Turn controller gains  ***Lower the I value by 0.0002
        	{0.002, 0, 0.0001}  // Angle controller gains (helps drive straight)
    		)
		.withMaxVelocity(200)
		.withDimensions({AbstractMotor::gearset::blue,(40.0/33.0)},{{3.375_in, 14.5_in}, imev5BlueTPR})
		.withOdometry(StateMode::CARTESIAN)
    	.buildOdometry(); // build an odometry chassis

std::shared_ptr<AsyncPositionController<double, double>> catapultControl =
	AsyncPosControllerBuilder()
		.withMotor(catapultMotor)
		.build();
std::shared_ptr<AsyncController<double, double>> catapultVelocityControl=
	AsyncVelControllerBuilder()
	.withMotor(catapultMotor)
	.build();

void launchCatapult(){
	while(catapultLimit.get_value()){
		catapultMotor.moveVelocity(-100);
	}
	catapultMotor.moveVelocity(0);
}
void retractCatapult(){
	while(!catapultLimit.get_value()){
		catapultMotor.moveVelocity(-100);
	}
	catapultMotor.moveVelocity(0);
}

void on_center_button() {
	
}

void initialize() {
	catapultMotor.setBrakeMode(AbstractMotor::brakeMode(2));
	catapultMotor.tarePosition();
	
	
	
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	retractCatapult();
	pros::delay(500);
	drive->driveToPoint({0_ft,1.5_in});
	drive->waitUntilSettled();
	rollerMotor.moveVelocity(-200);
	pros::delay(500);
	rollerMotor.moveVelocity(0);
	pros::delay(500);
	drive->driveToPoint({0_ft,-2_in},true);
	drive->waitUntilSettled();
	drive->turnToAngle(95_deg);
	drive->waitUntilSettled();
	drive->setState({0_in, 0_in, 0_deg});
	drive->driveToPoint({0_ft,2_ft});
	drive->waitUntilSettled();
	drive->turnToAngle(52_deg);
	drive->waitUntilSettled();
	drive->setState({0_in, 0_in, 0_deg});
	drive->driveToPoint({0_ft,10.5_ft});
	drive->waitUntilSettled();
	drive->setState({0_in, 0_in, 0_deg});
	drive->turnToAngle(-50_deg);
	drive->waitUntilSettled();
	//drive->driveToPoint({0_ft,2_ft});
	// pros::delay(500);
	// drive->driveToPoint({0_ft,2_ft});
	// drive->waitUntilSettled();
	// drive->turnToAngle(90_deg);
	// drive->waitUntilSettled();
	// pros::delay(2000);
	// drive->driveToPoint({2_ft,2_ft});
	// drive->waitUntilSettled();
	// drive->turnToAngle(-90_deg);
	// drive->waitUntilSettled();
	// pros::delay(500);
	// launchCatapult();
	//drive->driveToPoint({1_ft,2_ft});
	//drive->turnToAngle(90_deg);
}
double degrees;
std::string str;

void opcontrol() {
	while (true) {
		drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY),controller.getAnalog(ControllerAnalog::rightX)*0.75);
		
		if (intakeButton.changedToPressed())
		{
			intakeMotor.moveVelocity(-200);
		}
		else if(intakeButton.changedToReleased())
		{
			intakeMotor.moveVoltage(0);
		}

		if (reverseIntakeButton.changedToPressed())
		{
			intakeMotor.moveVelocity(200);
		}
		else if(reverseIntakeButton.changedToReleased())
		{
			intakeMotor.moveVoltage(0);
		}

		if (rollerButton.changedToPressed())
		{
			rollerMotor.moveVelocity(-200);
		}
		else if(rollerButton.changedToReleased())
		{
			rollerMotor.moveVoltage(0);
		}
		
		if (retractCatapultButton.isPressed())
		{
			//degrees = r.controllerGet();
			//str = std::to_string(degrees);
			//controller.setText(1, 1, str);
			//r.reset();
			//while(r.controllerGet()<70){
			//	catapultMotor.moveVelocity(-50);
			//}
			//catapultControl->setTarget(-650);
			//degrees = r.controllerGet();
			//str = std::to_string(degrees);
			//controller.setText(1, 1, str);
			armset=false;
			/*while(!catapultLimit.get_value()){
				catapultMotor.moveVelocity(-100);
				catapultVelocityControl->setTarget(-100);
			}*/
			//catapultMotor.moveVelocity(0);
			//catapultVelocityControl->setTarget(0);
			
		}
		if(!armset && !catapultLimit.get_value()){
			catapultMotor.moveVelocity(-100);
		}else{
			catapultMotor.moveVelocity(0);
			armset=true;
		}
		if (launchCatapultButton.isPressed())
		{
			while(catapultLimit.get_value()){
				catapultMotor.moveVelocity(-100);
				//catapultVelocityControl->setTarget(-100);
			}
			catapultMotor.moveVelocity(0);
			//catapultVelocityControl->setTarget(0);
			//catapultMotor.moveRelative(-50, -50);
			/*catapultControl->setTarget(-800);
			catapultControl->waitUntilSettled();
			pros::delay(500);
			catapultControl->setTarget(-1050+catapultAdust);
			catapultControl->waitUntilSettled();
			pros::delay(1000);
			catapultMotor.tarePosition();
			catapultAdust-=100;*/
			//while(r.get()<85){
			//	catapultMotor.moveVelocity(200);
			//}
		}
	}
}
