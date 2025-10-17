/* src/vga.c */
#include "vga.h"

volatile uint16_t *VGA = (uint16_t *)0xB8000;
enum { VGA_WIDTH = 80, VGA_HEIGHT = 25 };

static size_t cursor_x = 0;
static size_t cursor_y = 0;

static void scroll_if_needed() {
    if (cursor_y < VGA_HEIGHT) return;
    for (size_t row = 1; row < VGA_HEIGHT; ++row) {
        for (size_t col = 0; col < VGA_WIDTH; ++col) {
            VGA[(row-1) * VGA_WIDTH + col] = VGA[row * VGA_WIDTH + col];
        }
    }
    const uint16_t blank = (0x0F << 8) | ' ';
    for (size_t col = 0; col < VGA_WIDTH; ++col) {
        VGA[(VGA_HEIGHT-1) * VGA_WIDTH + col] = blank;
    }
    cursor_y = VGA_HEIGHT - 1;
}

void vga_clear(uint8_t color) {
    uint16_t entry = ((uint16_t)color << 8) | ' ';
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) VGA[i] = entry;
    cursor_x = cursor_y = 0;
}

void vga_set_cursor(size_t x, size_t y) {
    if (x >= VGA_WIDTH) x = VGA_WIDTH - 1;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;
    cursor_x = x; cursor_y = y;
}

void vga_get_cursor(size_t *x, size_t *y) {
    if (x) *x = cursor_x;
    if (y) *y = cursor_y;
}

void vga_putc(char c, uint8_t color) {
    if (c == '\r') { cursor_x = 0; return; }
    if (c == '\n') { cursor_x = 0; cursor_y++; scroll_if_needed(); return; }
    if (c == '\t') {
        int spaces = 4 - (cursor_x % 4);
        for (int i = 0; i < spaces; ++i) vga_putc(' ', color);
        return;
    }
    if (c >= 0x20) {
        VGA[cursor_y * VGA_WIDTH + cursor_x] = ((uint16_t)color << 8) | (uint8_t)c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) { cursor_x = 0; cursor_y++; scroll_if_needed(); }
    }
}

void vga_puts(const char *s, uint8_t color) {
    for (size_t i = 0; s[i]; ++i) vga_putc(s[i], color);
}
