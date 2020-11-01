#ifndef BITUTILS_H
#define BITUTILS_H

/** Returns the value of x's 'b'th bit. */
#define BIT(x, b)        ((x) & (1 << (b)))

/** Returns x with its 'b'th bit set to 1. */
#define BIT_ON(x, b)     ((x) | (1 << (b)))

/** Returns x with its 'b'th bit set to 0. */
#define BIT_OFF(x, b)    ((x) & ~(1 << (b)))

#endif // BITUTILS_H