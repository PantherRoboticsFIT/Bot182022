/*
To do:
	*check if speed is correct for intake
*/

#include "main.h"
#define expansionPiston 'A'

Motor intakeMotor {14,true,okapi::AbstractMotor::gearset::green,okapi::AbstractMotor::encoderUnits::degrees};
Motor catapultMotor {4,true,okapi::AbstractMotor::gearset::red,okapi::AbstractMotor::encoderUnits::degrees};
Motor rollerMotor {5,true,okapi::AbstractMotor::gearset::red,okapi::AbstractMotor::encoderUnits::degrees};

auto r = RotationSensor(15);

Controller controller;
pros::ADIDigitalOut expansion (expansionPiston);
ControllerButton launchCatapultButton (ControllerDigital::L1);
ControllerButton retractCatapultButton (ControllerDigital::L2);
ControllerButton intakeButton (ControllerDigital::R2);   
ControllerButton reverseIntakeButton (ControllerDigital::R1); 
//ControllerButton rollerButton (ControllerDigital::Y);
//ControllerButton reverseRollerButton (ControllerDigital::B);//may not be needed/wanted
ControllerButton expansionButton (ControllerDigital::up);
ControllerButton expansionFailsafeButton (ControllerDigital::down);

std::shared_ptr<OdomChassisController> drive =
	ChassisControllerBuilder()
		.withMotors({-1, 2,-3}, {11, -12,13})
		// Green gearset, 4 in wheel diam, 11.5 im wheel track
		// 36 to 60 gear ratio
			//.withGains(
        	//{2, 0, 0}, // Distance controller gains
        	//{.4, 0, 0.2}, // Turn controller gains
        	//{0.001, 0, 0.0001}  // Angle controller gains (helps drive straight)
    		//)
		.withDimensions({AbstractMotor::gearset::blue,(60.0/33.0)},{{3.25_in, 14.5_in}, imev5BlueTPR})
		.withOdometry(StateMode::CARTESIAN)
    	.buildOdometry(); // build an odometry chassis

std::shared_ptr<AsyncPositionController<double, double>> catapultControl =
	AsyncPosControllerBuilder()
		.withMotor(catapultMotor)
		.build();

void on_center_button() {
	
}

void initialize() {
	catapultMotor.setBrakeMode(AbstractMotor::brakeMode(2));
	catapultMotor.tarePosition();
	r.reset();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	while (true) {
		drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY),controller.getAnalog(ControllerAnalog::rightX));
		
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
		
		if (retractCatapultButton.isPressed())
		{
			//catapultControl->setTarget(-87);
		}
		if (launchCatapultButton.isPressed())
		{
			catapultControl->setTarget(0);
		}
	}
}
