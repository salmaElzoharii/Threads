
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
typedef struct {
    int left;
    int right;
    int* arr;
} ThreadArgs;
void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 
  
    // Create temp arrays 
    int L[n1], R[n2]; 
  
    // Copy data to temp arrays 
    // L[] and R[] 
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1 + j]; 
  
    // Merge the temp arrays back 
    // into arr[l..r] 
    // Initial index of first subarray 
    i = 0; 
  
    // Initial index of second subarray 
    j = 0; 
  
    // Initial index of merged subarray 
    k = l; 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } 
        else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    // Copy the remaining elements 
    // of L[], if there are any 
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    // Copy the remaining elements of 
    // R[], if there are any 
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
}  
void *mergeSort(void* arg) 
{ 
    ThreadArgs* args = (ThreadArgs*)arg;
    int l = args->left;
    int r = args->right;
    int *arr=args->arr;
    if (l < r) { 
        
        int m = l + (r - l) / 2; 
   pthread_t thread1, thread2;
    ThreadArgs* threadArgs = (ThreadArgs*)malloc((2) * sizeof(ThreadArgs));
    threadArgs[0].left=l;
    threadArgs[0].right=m;
    threadArgs[0].arr=arr;
    threadArgs[1].left=m+1;
    threadArgs[1].right=r;
    threadArgs[1].arr=arr;
        pthread_create(&thread1, NULL,(void *) mergeSort, &threadArgs[0]); 
        pthread_create(&thread2, NULL, (void *)mergeSort, &threadArgs[1]); 
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        merge(arr, l, m, r); 
    } 
} 

int main()
{
    int numOfElements;
   
    char filename[100];
    printf("Enter file name:");
    scanf("%s", filename);

    FILE* file = fopen(filename, "r"); 
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

   
    fscanf(file, "%d ", &numOfElements);
    int *arr = (int*)malloc(numOfElements * sizeof(int));
    for (int i = 0; i < numOfElements; i++) {
     fscanf(file, "%d ", &arr[i]);   
    }
    fclose(file);
    pthread_t thread;
 ThreadArgs args;
 args.left=0;
 args.right=numOfElements-1;
 args.arr=arr;
  pthread_create(&thread, NULL,(void *) mergeSort, &args) ;
  pthread_join(thread, NULL);
   printf("Sorted Array: ");
    for (int i = 0; i < numOfElements; i++) {
        printf("%d ", arr[i]);
    }
    
    return 0;
}

    