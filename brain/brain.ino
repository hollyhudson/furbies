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


int position;
int last_position_max;
bool last_rotation;
bool last_home_sensor;

void loop() 
{
	updateMotor();
}

void updateMotor()
{
	int sounds = analogRead(SOUND_IN); // returns int (0 to 1023)
	//Serial.println(sounds - 960);

	bool forward_pressed = (digitalRead(XBACK) == LOW);	
	bool reverse_pressed = (digitalRead(TUMMY) == LOW);

	bool home_sensor = digitalRead(XCAM_HOME) == LOW;
	bool rotation_step = digitalRead(GEAR_ROT) == LOW;
	int rotation_delta = 0;

	if (rotation_step)
	{
		// if the last read of the sensor was low,
		// then this is a real step.
		if (!last_rotation)
			rotation_delta = 1;
	}
	last_rotation = rotation_step;

	if (home_sensor)
	{
		if (!last_home_sensor)
		{
			last_position_max = position;
			position = 0;
		}
	}
	last_home_sensor = home_sensor;

	Serial.print(forward_pressed); Serial.print(' ');
	Serial.print(reverse_pressed); Serial.print(' ');
	Serial.print(rotation_step); Serial.print(' ');
	Serial.print(home_sensor); Serial.print(' ');

	// Don't do anything if both are pressed
	if (!forward_pressed && !reverse_pressed) {
		digitalWrite(MOTOR_REV, LOW);
		digitalWrite(MOTOR_FOR, LOW);
	}

	if (forward_pressed) {
		digitalWrite(MOTOR_REV, LOW);
		digitalWrite(MOTOR_FOR, HIGH);
		position += rotation_delta;
	}

	if (reverse_pressed) {
		digitalWrite(MOTOR_REV, HIGH);
		digitalWrite(MOTOR_FOR, LOW);
		position -= rotation_delta;
	}

	Serial.print(position); Serial.print(' ');
	Serial.print(last_position_max); Serial.print(' ');
	Serial.println();
}
