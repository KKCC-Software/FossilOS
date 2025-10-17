/* src/keyboard.c - polled PS/2 keyboard reader (scancode set 1) */
#include "keyboard.h"
#include <stdint.h>

/* I/O helpers */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* simple scancode -> ASCII for common keys */
static const char scancode_to_ascii[128] = {
 /* 0x00 - 0x07 */ 0, 0, '1','2','3','4','5','6',
 /* 0x08 - 0x0f */ '7','8','9','0','-','=','\b','\t',
 /* 0x10 - 0x17 */ 'q','w','e','r','t','y','u','i',
 /* 0x18 - 0x1f */ 'o','p','[',']','\n',0,'a','s',
 /* 0x20 - 0x27 */ 'd','f','g','h','j','k','l',';',
 /* 0x28 - 0x2f */ '\'','`',0,'\\','z','x','c','v',
 /* 0x30 - 0x37 */ 'b','n','m',',','.','/',0,'*',
 /* 0x38 - 0x3f */ 0,' ',' ',0,0,0,0,0,
 /* rest filled with 0 */
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void keyboard_init(void) {
    /* nothing for polling mode */
    (void)inb; (void)outb;
}

/* blocking read of a scancode and translate; ignores break codes (0x80+) */
char keyboard_getchar(void) {
    while (1) {
        uint8_t status = inb(0x64);
        if (!(status & 1)) continue;
        uint8_t sc = inb(0x60);
        if (sc & 0x80) continue; /* break code */
        if (sc < sizeof(scancode_to_ascii)) {
            char c = scancode_to_ascii[sc];
            return c;
        }
    }
}
