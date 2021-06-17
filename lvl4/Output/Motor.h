#ifndef motor
   #define motor



typedef enum{
	STRAIGHT = 0,
	LEFT = 1,
	RIGHT = 2
} driving_states;


/*  - F u n c t i o n  P r o t o t y p e s                               */
	void motor_init(void);	
	void odometer_init(void);
	void drive(int reverse);
	void drive_distance(int reverse, int distance);
	void drive_degree(int reverse, int degree);
	void set_direction(driving_states direction);
	void stop_motors(void);
	int CounterLeft(void);
	int CounterRight(void);
#endif
