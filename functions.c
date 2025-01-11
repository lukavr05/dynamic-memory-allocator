#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BATCH 10
#define FREE '_'
#define STOP '@'
#define REMOVE 8

// defining the node for the linked list
struct node {
  int p;
  struct node *next;
};

// defining the space struct for the memory
struct space {
  char *memory;
  int len;
};

// generalising the print function
void printMemory(struct space *mem) {
  printf("memory=%s (%d)\n", mem->memory, mem->len);
}

// initialises the memory by allocating space and setting all empty values to
// FREE
void initialize(struct space *mem) {
  // allocate memory
  mem->memory = (char *)malloc(BATCH + 1);
  if (mem->memory == NULL) {
    return;
  }

  mem->len = BATCH;

  // set all the empty memory values to FREE
  for (int i = 0; i < BATCH; i++) {
    mem->memory[i] = FREE;
  }

  // set terminating character
  mem->memory[BATCH] = '\0';

  // output memory
  printf("initializing: ");
  printMemory(mem);
}

// clear the memory and free it
void clear(struct space *mem) {
  if (mem->memory != NULL) {
    printf("removing everything: ");
    printMemory(mem);
    free(mem->memory);
  }
}

// scan the memory and return the position of the first free block of the size
// passed in
int scanMemory(struct space *mem, int size) {
  int p = -1;
  int i = 0;

  while (p < 0 && i < mem->len) {
    // increase i until the memory location is marked with FREE
    while (i < mem->len && mem->memory[i] != FREE) {
      i++;
    }

    int t = 0;

    // increase t until the memory location is NOT FREE
    while (i + t < mem->len && mem->memory[i + t] == FREE && t < size) {
      t++;
    }

    // if t is the size we want, return it
    if (t == size) {
      p = i;
      break;
    }
    
    if (t > 0) {
      i += t;
    } else {
      i++;
    }
  }

  return p;
}

// adding a node to the head of the linked list
void addNode(struct node **head, int p) {
  // allocate memory to a new node
  struct node *new = (struct node *)malloc(sizeof(struct node));

  // set the next and p values of the new node
  new->p = p;
  new->next = *head;

  // set the new node as the head of the linked list
  *head = new;
}

// allocate memory for new words
int allocator(struct space *mem, int size, struct node **head) {
  int p = scanMemory(mem, size);
  if (p >= 0) {
    addNode(head, p);
  }
  return p;
}

// removing a node from the linked list
void removeNode(struct node **head, int p) {
  struct node *cur = *head, *prev;

  if (cur != NULL && cur->p == p) {
    *head = cur->next;
    free(cur);
    return;
  }

  while (cur != NULL && cur->p != p) {
    prev = cur;
    cur = cur->next;
  }

  if (cur == NULL) {
    return;
  }

  prev->next = cur->next;

  free(cur);
}

// deallocate the memory to remove a word
void deallocator(struct space *mem, int p, struct node **head) {
  int i = p;

  // while not at the end of the word, set to FREE
  while (i < mem->len && mem->memory[i] != STOP) {
    mem->memory[i] = FREE;
    i++;
  }
  
  // set the STOP value to FREE
  if (i < mem->len && mem->memory[i] == STOP) {
    mem->memory[i] = FREE;
  }

  // print the memory
  printf("deleting one word: ");
  printMemory(mem);

  // remove a node from the linked list
  removeNode(head, p);
}

// adding memory
void addMemory(struct space *mem) {
  int n = mem->len + BATCH;

  // allocating memory for the copy
  char *new = (char *)malloc((n + 1) * sizeof(char));

  // copying the memory 
  memcpy(new, mem->memory, mem->len);

  // copying first mem->len and setting the rest to FREE
  for (int i = mem->len; i < n; i++) {
    if (i < mem->len) {
      new[i] = mem->memory[i];
    } else {
      new[i] = FREE;
    }
  }

  // setting the terminating character
  new[n] = '\0';

  // freeing the old memory
  free(mem->memory);

  // setting the values for the new memory
  mem->memory = new;
  mem->len = n;

  printf("new memory added: ");
  printMemory(mem);
}

// a new allocator function that implements adding the memory using the linked list
int allocatorNew(struct space *mem, int size, struct node **head) {
  int p = scanMemory(mem, size);
  // while p is negative, add memory while it still exists
  while (p < 0) {
    addMemory(mem);
    p = scanMemory(mem, size);
  }

  // add a node with p as the head of the linked list
  addNode(head, p);
  return p;
}
