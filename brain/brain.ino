/*
Furby Brain
*/

#define SOUND_IN 21
#define TILT 20
#define UPSIDE_DN 19
#define MOTOR_FOR 18 // 4 // PWM
#define MOTOR_REV 17 // 5 // PWM
#define XCAM_HOME 0
#define GEAR_ROT 1
#define XBACK 2
#define TUMMY 3

int desired_position = 0;
int position;
long last_millis;

// EARS
	int ears[] = {
		0,
		50,
		22,
		12,
		84,
		90,
		45,	
	};
	const int num_ears = sizeof(ears) / sizeof(*ears);
void ears_function()
{
	for (int i = 0; i < num_ears; i++){
		long start_time = millis();
		desired_position = ears[i];

		while (millis() - start_time < 150)
		{
			updateMotor();
			if (position == desired_position) break;
		}
	}
}

// EYES
	int eyes[] = {
		120,
		15,
		22,
		3,
	};
	const int num_eyes = sizeof(eyes) / sizeof(*eyes);
void eyes_function()
{
	for (int i = 0; i < num_eyes; i++){
		long start_time = millis();
		desired_position = eyes[i];

		while (millis() - start_time < 150)
		{
			updateMotor();
			if (position == desired_position) break;
		}
	}
}

// NOSE
	int nose[] = {
		36,
		39,
		20,
		2,
		150,
		132,
		93,
		68,
		77,
	};
	const int num_nose = sizeof(nose) / sizeof(*nose);
void nose_function()
{
	for (int i = 0; i < num_nose; i++){
		long start_time = millis();
		desired_position = nose[i];

		while (millis() - start_time < 150)
		{
			updateMotor();
			if (position == desired_position) break;
		}
	}
}

// TOES 
	int toes[] = {
		72,
		43,
		80,
		61,
	};
	const int num_toes = sizeof(toes) / sizeof(*toes);
void toes_function()
{
	Serial.println("toes");
	for (int i = 0; i < num_toes; i++){
		long start_time = millis();
		desired_position = toes[i];

		while (millis() - start_time < 150)
		{
			updateMotor();
			if (position == desired_position) break;
		}
	}
}
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
		digitalWrite(MOTOR_FOR, HIGH);
		//analogWrite(MOTOR_FOR, dir);
	} else {
		digitalWrite(MOTOR_FOR, LOW);
		digitalWrite(MOTOR_REV, HIGH);
		//analogWrite(MOTOR_REV, dir);
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
	Serial.println("Finding home in setup");
	const long start_time = millis();

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



void loop() 
{
	// The fastest it can run is 80, faster is just small twitches

	if (random(20000) == 0) {
		int choice = random(4);
					Serial.print("random what ");
					Serial.println(choice);
		switch(choice){
			case 0: ears_function(); break;
			case 1: eyes_function(); break;
			case 2: nose_function(); break;
			case 3: toes_function(); break;
			default:
					break;
		}

		// go back to home
		Serial.println("Finding home");
		const long start_time = millis();
		motor(+150);
	
		while(digitalRead(XCAM_HOME))
		{
			if (millis() - start_time > 5000)
				break;
		}

		motor(0);
	
		position = 0;
	}

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
