#include "my_strmap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* a strMap provides a mapping between hashed strings and their plaintext
 * values, using a red-black tree to handle collisions
 * */


strMap *sm_new(unsigned int capacity){
    // allocate all necessary space for an uninitialized hash table
    strMap *map;
    Bucket *bucket;
    unsigned int i = 0;

    map = malloc(sizeof(strMap));
    map->count = capacity;
    map->buckets = malloc(map->count * sizeof(Bucket));
    memset(map->buckets, 0, map->count * sizeof(Bucket));

    bucket = map->buckets;
    for(i = 0; i < map->count; i++){
        bucket->count = 0;
    }

    return map;
}

static unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;
    unsigned int i;

    // hash function must be case-insensitive
    for(i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }

    // magic
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int sm_get(strMap *map, char *string)
{
    unsigned int index;
    Bucket *bucket;
    rb_node *ret;

    index = hash(string) % map->count;
    bucket = &(map->buckets[index]);

    // bucket always contains a node or empty
    if(bucket->strings == NULL){
        return 0;
    }
    // search the collision tree
    ret = rb_search(bucket->strings, string);
    if(ret){
        return 1;
    }
    return 0;
}

void sm_put(strMap *map, char *string){
    unsigned int index;
    Bucket *bucket;

    // special case, must remove trailing chars in some cases
    if(string[strlen(string) - 1] == '\r'){
        string[strlen(string) - 1] = '\0';
    }

    index = hash(string) % map->count;
    bucket = &(map->buckets[index]);

    // similar to sm_get: add to the collision tree
    // or create a tree
    if(bucket->strings == NULL){
        bucket->strings = rb_create_tree(string);
    } else {
        bucket->strings = rb_insert(bucket->strings, string);
    }
    bucket->count++;
}

int sm_get_count(strMap *map){
    // walk the hash table and accumulate counts for each slot
    unsigned int i = 0;
    int count = 0;
    Bucket *bucket = map->buckets;

    while(i < map->count){
        if(bucket && bucket->count)
            count += bucket->count;
        bucket++;
        i++;
    }
    return count;
}

void sm_dealloc(strMap *map){
    unsigned int i;
    Bucket *bucket = map->buckets;
    // deallocate the collision trees
    for(i = 0; i < map->count; i++){
        rb_dealloc(bucket->strings);
        bucket++;
    }
    // deallocate the rest of the table
    free(map->buckets);
    free(map);
}