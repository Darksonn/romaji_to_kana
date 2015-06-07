#include <stdlib.h>
#include "converter_tree.h"

static unsigned char should_value_be_freed(conv_tree tree) {
	return tree->should_be_freed_bits & 1;
}
static unsigned char should_this_be_freed(conv_tree tree) {
	return !!(tree->should_be_freed_bits & 2);
}


void init_conv_tree(conv_tree init, unsigned char should_this_be_freed) {
	init->value = 0;
	init->subnodes_len = 0;
	init->subnodes_buf_len = 0;
	init->subnodes = 0;
	init->should_be_freed_bits = should_this_be_freed * 2;
}
void free_conv_tree(conv_tree tree) {
	size_t i;
	for (i = 0; i < tree->subnodes_len; i++) {
		free_conv_tree(tree->subnodes[i].node);
	}
	free(tree->subnodes);
	if (should_value_be_freed(tree))
		free(tree->value);
	if (should_this_be_freed(tree))
		free(tree);
}

conv_tree find_subnode(conv_tree tree, char next_char) {
	size_t i;
	struct convert_tree_node_char_pair *current_subnode = tree->subnodes;
	for (i = 0; i < tree->subnodes_len; i++) {
		if (current_subnode->next_char == next_char) {
			return current_subnode->node;
		}
		++current_subnode;
	}
	return 0;
}
static void append_convert_tree_node_char_pair(conv_tree tree, struct convert_tree_node_char_pair pair) {
	if (tree->subnodes_len == tree->subnodes_buf_len) {
		size_t new_subnodes_len = tree->subnodes_buf_len * 2 + 1;
		struct convert_tree_node_char_pair *new_subnodes = malloc(sizeof(struct convert_tree_node_char_pair) * new_subnodes_len);
		size_t i;
		for (i = 0; i < tree->subnodes_len; i++) {
			new_subnodes[i] = tree->subnodes[i];
		}
		if (tree->subnodes)
			free(tree->subnodes);
		tree->subnodes = new_subnodes;
		tree->subnodes_buf_len = new_subnodes_len;
	}
	tree->subnodes[tree->subnodes_len++] = pair;
}

void add_character(conv_tree tree, char *value, char *path, size_t path_len, unsigned char should_value_be_freed_) {
	size_t i;
	for (i = 0; i < path_len; i++) {
		conv_tree pair = find_subnode(tree, path[i]);
		if (pair) {
			tree = pair;
		} else {
			struct convert_tree_node *next_node = malloc(sizeof(struct convert_tree_node));
			init_conv_tree(next_node, 1);
			struct convert_tree_node_char_pair next_pair;
			next_pair.next_char = path[i];
			next_pair.node = next_node;
			append_convert_tree_node_char_pair(tree, next_pair);
			tree = next_node;
		}
	}
	if (should_value_be_freed(tree))
		free(tree->value);
	tree->value = value;
	tree->should_be_freed_bits |= 1*should_value_be_freed_;
}

