# HashMap

This hashmap works by creating a array of linked lists and associating
the "key" with a specific entry in the array. This is done through a
hash. Once the linked list is found it goes through it until it finds
a entry that has the "key" provided.

# Changing the hashing function

The default hashing function in this library is a custom made
one that can be found in hash.c. But it should be possible to use any
other hashing algorithm should you want to as long as it uses these
types:

```C
uint32_t hash_function(uint8_t*, size_t)
```

The hashing algorithm can be changed via changing the "hash_function"
pointer in the HashMap structure.

Important to note that the hashing function shall not be changed
after having added entries to the hashmap as the key to linked list
pairing will change.

# Call function once a entry has been deleted.

Should it be the case that the entries you put in the hashmap are for
example allocated to the heap you may want them to be automatically
freed upon deletion.

This can be done by changing the last value of

```C
int hashmap_add_entry(HashMap *m, char *key, void *value, void (*upon_deletion)(char *, void *), int do_not_allocate_key);
```

to your own function that can be called. For example:

```C
// SPDX-License-Identifier: 0BSD
void auto_free(char *key, void *value)
{
  printf("key: %s is being deleted\n", key);
  free(value);
}
```

# hashmap_add_entry()

This function adds a entry to the hashmap. The "key" passed to the
hashmap will be by default allocated before adding it. The "key" will
not be allocated should `do_not_allocate_key` be set to 1.

# Example

```C
// SPDX-License-Identifier: 0BSD
int main(void) {
  char *str = "cat";
  char *str1 = "dog";
  char *str2 = "snake";

  // Create the hashmap with 100 possible entries
  HashMap *m = hashmap_create(100);
  if (!m) {
    printf("hashmap_create() failed\n");
    return 1;
  }

  // Add the entries "apple", "banana", "orange" to the hashmap.
  // The "key" value passed to hashmap_add_entry will be automatically
  // allocated.
  hashmap_add_entry(m, "apple", str, NULL, 0);
  hashmap_add_entry(m, "banana", str1, NULL, 0);
  hashmap_add_entry(m, "orange", str2, NULL, 0);

  // Print out the entries created
  printf("apple: %s\n", (char *)hashmap_get_entry(m, "apple"));
  printf("banana: %s\n", (char *)hashmap_get_entry(m, "banana"));
  printf("orange: %s\n", (char *)hashmap_get_entry(m, "orange"));

  // Delete the entry "orange"
  hashmap_delete_entry(m, "orange");

  // Print the entry that was deleted(orange). It will now return
  // NULL.
  printf("orange: %s\n", (char *)hashmap_get_entry(m, "orange"));

  // Free the hashmap and all of the memory that was allocated.
  hashmap_free(m);
  return 0;
}
```
