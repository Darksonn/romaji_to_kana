#ifndef _CONVERTER_H
#define _CONVERTER_H

#include "converter_tree.h"
#include <stddef.h>

struct converter {
	conv_tree root, location;
	unsigned char should_this_be_freed;
};
typedef struct converter *converter;

void init_converter(converter conv, unsigned char should_this_be_freed);
void free_converter(converter conv);
unsigned char feed_converter(converter conv, char c);
char *get_current_conversion(converter conv);
unsigned char is_converter_reset(converter conv);

#endif

