#ifndef FUNCTIONS_H
#define FUNCTIONS_H


typedef struct plist_node{										////POSTING LIST
	int textid;
	int count;
	struct plist_node* next;
}PostingList;

PostingList* addPostingList(PostingList* , int);

void freePostingList(PostingList*);

typedef struct trie_node{										////TRIE
	char letter;
	PostingList *plist;
	struct trie_node *horiz;
	struct trie_node *vert;
}Trie;

void search_DFTrie(char* ,Trie* );

void search_TFTrie(char* ,Trie* ,int );

Trie* addTrie(char* ,int ,Trie*);

void freeTrie(Trie *);

#endif