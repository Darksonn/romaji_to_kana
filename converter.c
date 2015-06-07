#include "converter.h"
#include "converter_tree.h"
#include <stdlib.h>

void init_converter(converter conv, unsigned char should_this_be_freed) {
	conv->should_this_be_freed = should_this_be_freed;
	conv_tree tree = malloc(sizeof(struct convert_tree_node));
	init_conv_tree(tree, 1);
	conv->location = tree;
	conv->root = tree;
}

void free_converter(converter conv) {
	free_conv_tree(conv->root);
	if (conv->should_this_be_freed)
		free(conv);
}

char *feed_converter(converter conv, char c) {
	char *value = conv->location->value;
	conv_tree next = find_subnode(conv->location, c);
	if (next) {
		conv->location = next;
	} else {
		conv->location = conv->root;
	}
	return value;
}
unsigned char is_converter_reset(converter conv) {
	return conv->location == conv->root;
}

char *get_current_conversion(converter conv) {
	return conv->location->value;
}

