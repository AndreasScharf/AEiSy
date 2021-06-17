#ifndef display
   #define display
#include <stdint.h>

/*  - F u n c t i o n  P r o t o t y p e s                               */
	void display_init(void);
	void write_text(char  string[], int line, int column);
	void clear_display(void);
	void draw_pixel(int x, int y);
	void clear_pixel(int x, int y);
	void draw_circle(int x, int y, int radius);
	void fill_circle(int x, int y, int radius);
#endif
