#include "GPIO.h" 
#include <iostream>

int main(){

	int servo_id;
	int servo_angle;

	std::cout << "Please select one of the following servos: \n";
	std::cout << "Servo 1 - Base\n";
	std::cout << "Servo 2 - Bicep\n";
	std::cout << "Servo 3 - Elbow\n";
	std::cout << "Servo 4 - Wrist\n";
	std::cout << "Servo 5 - Gripper\n";
	std::cout << "Enter a number for a servo: ";
	
	std::cin >> servo_id;

	if (servo_id > 5 || servo_id < 1){
		std::cout << "Invalid servo id (" << servo_id << ")!";
		return 0;
	}

	std::cout << "Enter an angle between 0 and 180: ";
	
	std::cin >> servo_angle;

	if (servo_angle > 160 || servo_angle < 20){
                std::cout << "Invalid servo angle (" << servo_angle << ")!";
                return 0;
        }

	// 1 : Base = 13
	// 2 : Bicep = 10
	// 3 : Elbow = 11
	// 4 : Wrist = 12
	// 5 : Gripper 0

	int port_servo_map[6] = {0, 13, 10, 11, 12, 0};
	GPIO gpio(port_servo_map[servo_id]);

	int pulse_time = 600;

	pulse_time = pulse_time + (servo_angle * 10);

	gpio.GeneratePWM(20000, pulse_time, 200); 

	return 0;

}
