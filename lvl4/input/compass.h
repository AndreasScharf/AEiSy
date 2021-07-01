#ifndef compass
   #define compass
#include <stdint.h>

/*  - F u n c t i o n  P r o t o t y p e s                               */
	void compass_init(void);
	int get_orientation(void);
	int get_heading(void);
	int8_t get_pitch(void);
	int8_t get_roll(void);
#endif
