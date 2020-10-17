#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 10

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()% (100 - 1 +1);
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%d ", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

// merge function for merging two parts 
void merge(int low, int mid, int high) { 
  int left[mid - low + 1];
  int right[high - mid];

  // sizeOfLeft is size of left part and sizeOfRight is size 
  // of right part 
  int sizeOfLeft = mid - low + 1;
  int sizeOfRight = high - mid;

  // storing values in left part 
  int i;
  for (i = 0; i < sizeOfLeft; i++) 
      left[i] = array[i + low]; 

  // storing values in right part 
  for (i = 0; i < sizeOfRight; i++) 
      right[i] = array[i + mid + 1]; 

  int k = low; 
  i = 0; 

  // merge left and right in ascending order 
  int j = 0; 
  while (i < sizeOfLeft && j < sizeOfRight) { 
      if (left[i] <= right[j]) 
          array[k++] = left[i++]; 
      else
          array[k++] = right[j++]; 
  } 

  // insert remaining values from left 
  while (i < sizeOfLeft) { 
      array[k++] = left[i++]; 
  } 

  // insert remaining values from right 
  while (j < sizeOfRight) { 
      array[k++] = right[j++]; 
  } 
} 

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void* mergeSort(void* args) {
  struct StartEndIndexes *arg = args;
  int low = arg->start;
  int high = arg->end;

  // evaluating mid point 
  int mid = low + (high - low) / 2; 
  if (low < high) { 
    struct StartEndIndexes *s = malloc(sizeof (struct StartEndIndexes));
    s->start = low;
    s->end = mid;
    mergeSort(s); 
    struct StartEndIndexes *s1 = malloc(sizeof (struct StartEndIndexes));
    s1->start = mid + 1;
    s1->end = high;
    mergeSort(s1); 
    merge(low, mid, high); 
  } 
}

int main() {
  srand(time(0));
  StartEndIndexes sei;
  sei.start = 0;
  sei.end = SIZE - 1;
  
  // 1. Fill array with random numbers.
  fillArrayWithRandomNumbers(array);
  
  // 2. Print the array.
  printf("Unsorted array: ");
  printArray(array);
  
  // 3. Create a 2 threads for merge sort.
  pthread_t tid;
  pthread_t tid2;
  
  // 4. Wait for mergesort to finish.
  printf("Creating thread: 1\n");
  struct StartEndIndexes *s1 = malloc(sizeof (struct StartEndIndexes));
  s1->start = 0;
  s1->end = SIZE/2;
  int rc = pthread_create(&tid, NULL, mergeSort, s1);
  if (rc) {
    exit(-1);    
  }    

  printf("Creating thread: 2");
  struct StartEndIndexes *s2 = malloc(sizeof (struct StartEndIndexes));
  s2->start = SIZE/2;
  s2->end = SIZE;
  int rc1 = pthread_create(&tid2, NULL, mergeSort, s2);
  if (rc1) {
    exit(-1);    
  }  
  pthread_join(tid, NULL);  
  pthread_join(tid2, NULL);


  merge(0, 5, 10);

  free(s1);
  free(s2);

  
  // 5. Print the sorted array.
  printf("\nSorted array:   ");
  printArray(array);

  return 0;
}
