#ifndef motor
   #define motor

typedef enum{
	FORWARD = 0,
	LEFT = 1,
	RIGHT = 2
} driving_states;


/*  - F u n c t i o n  P r o t o t y p e s                               */
	void motor_init(void);	
	void drive(int reverse);
	void set_direction(driving_states direction);
	void stop_motors(void);
#endif
