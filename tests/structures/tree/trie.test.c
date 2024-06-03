#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "structures/tree/trie.h"

// NOTE: NO UNIT TESTS INCLUDED, ONLY FOR MANUAL TESTING
int main(void)
{
    trie test;
    init_trie(&test);
    insert_trie(&test, "A");
    insert_trie(&test, "B");
    insert_trie(&test, "Bc");
    insert_trie(&test, "BB");
    insert_trie(&test, "...");
    insert_trie(&test, "ANO");
    print_trie(&test);

    printf("\n");

    delete_trie(&test, "BB");
    delete_trie(&test, "BB");
    print_trie(&test);
    destruct_trie(&test);

    return 0;
}
