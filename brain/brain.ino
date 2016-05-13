/*
Furby Brain
*/

#define SOUND_IN 21
#define TILT 20
#define UPSIDE_DN 19
#define MOTOR_FOR 4 // PWM
#define MOTOR_REV 5 // PWM
#define XCAM_HOME 0
#define GEAR_ROT 1
#define XBACK 2
#define TUMMY 3

int desired_position = 0;
int position;


void motor(int dir)
{
	if(dir == 0)
	{
		digitalWrite(MOTOR_REV, LOW);
		digitalWrite(MOTOR_FOR, LOW);
	} else
	if (dir > 0)
	{
		digitalWrite(MOTOR_REV, LOW);
		analogWrite(MOTOR_FOR, dir);
	} else {
		digitalWrite(MOTOR_FOR, LOW);
		analogWrite(MOTOR_REV, dir);
	}
}

void setup() {
	pinMode( MOTOR_FOR, OUTPUT );
	pinMode( MOTOR_REV, OUTPUT );

	pinMode( SOUND_IN, INPUT );
	pinMode( TILT, INPUT_PULLUP );
	pinMode( UPSIDE_DN, INPUT_PULLUP );
	pinMode( GEAR_ROT, INPUT_PULLUP );
	pinMode( TUMMY, INPUT_PULLUP );

	pinMode( XCAM_HOME, INPUT_PULLUP );
	pinMode( XBACK, INPUT_PULLUP );

	// find the home position
	Serial.println("Finding home");
	const int start_time = millis();

	motor(+150);

	while(digitalRead(XCAM_HOME))
	{
		if (millis() - start_time > 5000)
			break;
	}

	motor(0);

	position = 0;

	delay(5000);
}


int last_position_max;
bool last_rotation;
bool last_home_sensor;

int step = 0;
int positions[] = {
	10,
	20,
	30,
	40,
	50,
	60,
	70,
	80,
	90,
	100,
	110,
	120,
	130,
	50,
	10,
	50,
	90,
	130,
	90,
	50,
};

const int num_positions = sizeof(positions) / sizeof(*positions);
int last_millis;

void loop() 
{
/*
	bool forward_pressed = (digitalRead(XBACK) == LOW);	
	bool reverse_pressed = (digitalRead(TUMMY) == LOW);	

	if (forward_pressed)
	{
		desired_position = 10;
	} else
	if (reverse_pressed)
	{
		desired_position = 50;
	}
*/
	int now = millis();
	if (now - last_millis > 1000)
	{
		last_millis = now;
		desired_position = positions[step];
		step = (step + 1) % num_positions;
	}
	

	updateMotor();
}

void updateMotor()
{
	//int sounds = analogRead(SOUND_IN); // returns int (0 to 1023)
	//Serial.println(sounds - 960);

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

	Serial.print(rotation_step); Serial.print(' ');
	Serial.print(home_sensor); Serial.print(' ');

	if (desired_position == position) {
		motor(0);
	}

	int delta = desired_position - position;

	if (desired_position > position) {
		motor(+150);
		position += rotation_delta;
	}

	if (desired_position < position) {
		motor(-150);
		position -= rotation_delta;
	}

	Serial.print(position); Serial.print(' ');
	Serial.print(last_position_max); Serial.print(' ');
	Serial.println();
}
