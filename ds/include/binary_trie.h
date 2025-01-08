#ifndef __BINARY_TRIE_H__
#define __BINARY_TRIE_H__

#include <stddef.h> /*size_t*/
#include <stdint.h> /*uint32_t*/

#define HEIGHT_OF_TRIE 4

typedef unsigned char trie_route[HEIGHT_OF_TRIE]; 

typedef struct trie_node trie_node_t;

trie_node_t* BTrieCreate(size_t height);


void BTrieDestroy(trie_node_t* root);


int GetNextNode(trie_node_t* node, uint32_t route, trie_route node_route);


size_t GetAmountOfFreeLeaves(trie_node_t* root);


int FreeLeaf(trie_node_t* node, uint32_t route);                                    

#endif	/*__BINARY_TRIE_H__*/