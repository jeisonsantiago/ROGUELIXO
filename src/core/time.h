#ifndef TIME_H
#define TIME_H

#define FIXED_DT (1.0f / 60.0f)
void  time_begin_frame(void); /* once per frame: adds frame time */
int   time_step(void);        /* returns 1 while a fixed step is due */
float time_alpha(void);       /* 0..1 leftover, used for drawing */


#endif // TIME_H
