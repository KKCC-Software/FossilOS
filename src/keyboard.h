/* src/keyboard.h */
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
char keyboard_getchar(void); /* blocking; returns ASCII or 0 for non-printable */

#endif
