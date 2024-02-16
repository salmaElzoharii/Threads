#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
    int* arrOfRows;
    int* arrOfCols;
    int* resultRow;
} ThreadArgs;
void *firstAlgorithm(void* arg)
{
    ThreadArgs* args = (ThreadArgs*)arg;
    int* arrOfRows = args->arrOfRows;
    int* arrOfCols = args->arrOfCols;
    int* resultRow = args->resultRow;
    int cols1 = arrOfRows[0];
    int cols2 = arrOfRows[1];
    int nth_thread = arrOfRows[2];
    int sum = 0;
  
    for (int i = 0; i < cols1; i++)
    {
        sum += arrOfRows[i + 3] * arrOfCols[i];
    }

    resultRow[nth_thread] = sum;

    pthread_exit(NULL);
}
void *secondAlgorithm(void* arg)
{
    ThreadArgs* args = (ThreadArgs*)arg;
    int* arrOfRows = args->arrOfRows;
    int* arrOfCols = args->arrOfCols;
    int* resultRow = args->resultRow;
    int cols1 = arrOfRows[0];
    int cols2 = arrOfRows[1];

    for (int k = 0; k < cols2; k++)
    {
        int sum = 0;
        for (int i = 0; i < cols1; i++)
        {
            sum += arrOfRows[i + 2] * arrOfCols[i + k * cols1];
        }
        resultRow[k] = sum;
    }
    pthread_exit(NULL);
}

int main()
{
    clock_t start_t1, end_t1,start_t2, end_t2;
    int i, j, rows1, cols1, rows2, cols2;
    int *arr;
    char filename[100];
    printf("Enter file name:");
    scanf("%s", filename);

    FILE* file = fopen(filename, "r"); 
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    fscanf(file, "%d %d", &rows1, &cols1);
    int** matrix1 = (int**)malloc(rows1 * sizeof(int*));
    for (i = 0; i < rows1; i++) {
        matrix1[i] = (int*)malloc(cols1 * sizeof(int));
        for (j = 0; j < cols1; j++) {
            fscanf(file, "%d", &matrix1[i][j]);
           
        }
    }

    fscanf(file, "%d %d", &rows2, &cols2);
    int** matrix2 = (int**)malloc(rows2 * sizeof(int*));
    for (i = 0; i < rows2; i++) {
        matrix2[i] = (int*)malloc(cols2 * sizeof(int));
        for (j = 0; j < cols2; j++) {
             fscanf(file, "%d", &matrix2[i][j]);
        }
    }
fclose(file); 
if(cols1==rows2)
{
    pthread_t *threads;
    threads = (pthread_t*)malloc((rows1 * cols2) * sizeof(pthread_t));
    int count = 0;
    int* resultFirstAlgorithm = (int*)malloc(rows1 * cols2 * sizeof(int));
    ThreadArgs* threadArgs1 = (ThreadArgs*)malloc((rows1 * cols2) * sizeof(ThreadArgs));

    start_t1 = clock();

    for(int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols2; j++)
        {
            threadArgs1[count].arrOfRows = (int*)malloc((3 + cols1) * sizeof(int));
            threadArgs1[count].arrOfCols = (int*)malloc((rows2) * sizeof(int));
            threadArgs1[count].resultRow = (int*)malloc(rows1 * cols2 * sizeof(int));
//passing cols1,cols2,number of thread which will be used later for
            threadArgs1[count].arrOfRows[0] = cols1;
            threadArgs1[count].arrOfRows[1] = cols2;
            threadArgs1[count].arrOfRows[2] = count;

            for (int k = 0; k < cols1; k++)
            {
                threadArgs1[count].arrOfRows[k + 3] = matrix1[i][k];//row elements of matrix one
            }

            for (int k = 0; k < rows2; k++)
            {
                threadArgs1[count].arrOfCols[k] = matrix2[k][j];//column elements of matrix2
            }

            threadArgs1[count].resultRow = resultFirstAlgorithm;

            pthread_create(&threads[count], NULL, firstAlgorithm, &threadArgs1[count]);
            count++;
        }
    }

    for (int i = 0; i < count; i++) {
        pthread_join(threads[i], NULL);
    }

    end_t1 = clock();

    printf("\nResult of the first algorithm:\n");
    for (int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols2; j++)
        {
            printf("%d ", resultFirstAlgorithm[i * cols2 + j]);
        }
        printf("\n");
    }
  
    printf("END1 %lf\n",(double)(end_t1-start_t1)/ CLOCKS_PER_SEC);

  //end of firstAlgorithm 
  //----------------------------------------------------------------------------------//   
  //start of second Algorithm
 
    int** resultMatrix = (int**)malloc(rows1 * sizeof(int*));
    for (i = 0; i < rows1; i++) {
        resultMatrix[i] = (int*)malloc(cols2 * sizeof(int));///save the result of each thread
    }

    int countThreads = 0;
    pthread_t* threads2 = (pthread_t*)malloc(rows1 * sizeof(pthread_t));//number of threads equal  number of rows of first matix
    ThreadArgs* threadArgs = (ThreadArgs*)malloc(rows1 * sizeof(ThreadArgs));
    start_t2=clock();
    for (int i = 0; i < rows1; i++)
    {
        threadArgs[i].arrOfRows = (int*)malloc((2 + cols1) * sizeof(int));
        threadArgs[i].arrOfCols = (int*)malloc((rows2 * cols2) * sizeof(int));
        threadArgs[i].resultRow = (int*)malloc(cols2 * sizeof(int));

        threadArgs[i].arrOfRows[0] = cols1;
        threadArgs[i].arrOfRows[1] = cols2;

        for (int j = 0; j < cols1; j++)
        {
            threadArgs[i].arrOfRows[j + 2] = matrix1[i][j];//passing row elements
        }

        int countIndex = 0;
        for (int j = 0; j < cols2; j++)
        {
            for (int k = 0; k < rows2; k++)
            {
                threadArgs[i].arrOfCols[countIndex] = matrix2[k][j];//passing columns elements of matrix2
                countIndex++;//incrementing count of arrOfCols
            }
        }

        threadArgs[i].resultRow = resultMatrix[i];

        pthread_create(&threads2[countThreads], NULL, secondAlgorithm, (void*)(&threadArgs[i]));
        countThreads++;
    }

    for (int i = 0; i < countThreads; i++)
    {
        pthread_join(threads2[i], NULL);
    }
end_t2=clock();
    // Print the result matrix
    printf("Result Matrix of second Algorithm:\n");
    for (i = 0; i < rows1; i++) {
        for (j = 0; j < cols2; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }
     printf("END2 %lf\n",(double)(end_t2-start_t2)/ CLOCKS_PER_SEC);

   
    for (i = 0; i < rows1; i++) {
        free(matrix1[i]);
        free(resultMatrix[i]);
        free(threadArgs[i].arrOfRows);
        free(threadArgs[i].arrOfCols);
    }
   
    free(threads2);
    free(threadArgs);


}
else{
    printf("Multiplication impossible!!\n");

}

    return 0;
}
