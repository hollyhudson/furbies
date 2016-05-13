/*
Furby Brain
*/

#define SOUND_IN 21
#define TILT 20
#define UPSIDE_DN 19
#define MOTOR_FOR 18
#define MOTOR_REV 17
#define XCAM_HOME 0
#define GEAR_ROT 1
#define XBACK 2
#define TUMMY 3

void setup() {
	pinMode( MOTOR_FOR, OUTPUT );
	pinMode( MOTOR_REV, OUTPUT );

	pinMode( SOUND_IN, INPUT );
	pinMode( TILT, INPUT_PULLUP );
	pinMode( UPSIDE_DN, INPUT_PULLUP );
	pinMode( GEAR_ROT, INPUT_PULLUP );
	pinMode( TUMMY, INPUT_PULLUP );

	pinMode( XCAM_HOME, INPUT_PULLUP );
	//digitalWrite( XCAM_HOME, HIGH ); // pullup
	pinMode( XBACK, INPUT_PULLUP );

}

void loop() 
{
	updateMotor();
}

void updateMotor()
{
	int sounds = analogRead(SOUND_IN); // returns int (0 to 1023)
	Serial.println(sounds - 960);

	bool forward_pressed = (digitalRead(XBACK) == LOW);	
	bool reverse_pressed = (digitalRead(TUMMY) == LOW);

	// Don't do anything if both are pressed
	if (!forward_pressed && !reverse_pressed) {
		digitalWrite(MOTOR_REV, LOW);
		digitalWrite(MOTOR_FOR, LOW);
	
	}

	if (forward_pressed) {
		digitalWrite(MOTOR_REV, LOW);
		digitalWrite(MOTOR_FOR, HIGH);
	}

	if (reverse_pressed) {
		digitalWrite(MOTOR_REV, HIGH);
		digitalWrite(MOTOR_FOR, LOW);
	}
}
