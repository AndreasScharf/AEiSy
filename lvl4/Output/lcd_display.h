#ifndef display
   #define display
#include <stdint.h>

/*  - F u n c t i o n  P r o t o t y p e s                               */
	void display_init(void);
	void write_text(char  string[], int line, int column);
	void clear_display(void);
	void draw_pixel(int x, int y);
	void clear_pixel(int x, int y);
	void draw_rectangle(int x, int y, int width, int height);
	void fill_rectangle(int x, int y, int width, int height);
	void draw_circle(int x, int y, int radius);
	void fill_circle(int x, int y, int radius);
	void draw_ellipse(int x1, int y1, int x2, int y2, int radius);
	void fill_ellipse(int x1, int y1, int x2, int y2, int radius);
	void draw_hs_logo(void);
	void play_animation(void);
	void plot_distance(int x, int y, int length);
	
	
#endif
