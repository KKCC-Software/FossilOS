/* src/vga.h - simple VGA text helpers */
#ifndef VGA_H
#define VGA_H

#include <stddef.h>
#include <stdint.h>

void vga_clear(uint8_t color);
void vga_putc(char c, uint8_t color);
void vga_puts(const char *s, uint8_t color);
void vga_set_cursor(size_t x, size_t y);
void vga_get_cursor(size_t *x, size_t *y);

#endif
