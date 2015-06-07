#ifndef _CONVERTER_TREE_H
#define _CONVERTER_TREE_H

#include <stddef.h>

struct convert_tree_node;

struct convert_tree_node_char_pair {
	struct convert_tree_node *node;
	char next_char;
};

struct convert_tree_node {
	char *value;
	unsigned char should_be_freed_bits;//1 = value, 2 = this
	size_t subnodes_len;
	size_t subnodes_buf_len;
	struct convert_tree_node_char_pair *subnodes;
};

typedef struct convert_tree_node *conv_tree;


void init_conv_tree(conv_tree, unsigned char should_this_be_freed);
void free_conv_tree(conv_tree tree);
conv_tree find_subnode(conv_tree tree, char next_char);
void add_character(conv_tree tree, char *value, char *path, size_t path_len, unsigned char should_value_be_freed);

#endif
