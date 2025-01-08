#include <stdio.h>

#include "binary_trie.h"

#define UNUSED(x) (void)(x)

int main()
{
	trie_node_t* root = TrieCreate(8);

	TrieDestroy(root);

	return 0;
}
