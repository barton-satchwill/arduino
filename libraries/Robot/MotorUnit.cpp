#include <MotorUnit.h>
#include <AFMotor.h>

  	// AF_DCMotor* theMotor;
  	// int id;
  	// int x=0;
	int MotorUnit::counter = 0;
	long stoppedTime = 0;  
	const int MotorUnit::stoppedInterval = 200;


MotorUnit::MotorUnit(uint8_t themotorNumber, uint8_t frequency) {
	// AF_DCMotor theMotor(motorNumber, frequency);
	theMotor = new AF_DCMotor(themotorNumber, frequency);
	id = counter++;
	motorNumber = themotorNumber;
	motion = halted;
}

void MotorUnit::setSpeed(int theSpeed) {
 theMotor->setSpeed(theSpeed);
}

void MotorUnit::forward() {
	if (motion != prograde && motion != halted) {
		halt();
	}
	if (!wait()) {
		// if (motion != prograde) {Serial.println(String(id) + " : " + String(motorNumber) + " ----->");}
		motion = prograde;
		theMotor->run(FORWARD);
	}
}

void MotorUnit::backward() {
	if (motion != retrograde && motion != halted) {
		halt();
	}
	if (!wait()){
		// if (motion != retrograde) {Serial.println(String(id) + " : " + String(motorNumber) + " <-----");}
		motion = retrograde;
		theMotor->run(BACKWARD);
	}
}

void MotorUnit::halt() {
	// if (motion != halted) {
		// Serial.println(String(id) + " : " + String(motorNumber) + " ------");
		motion = halted;
		stoppedTime = millis();
		theMotor->run(RELEASE);
	// }
}


boolean MotorUnit::wait() {
  if(millis() - stoppedTime > stoppedInterval) {
    return false;
  }
  // Serial.println(String(id) + " : " + String(motorNumber) + " ......");
  return true;
}



