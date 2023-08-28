#include "SortingCPPRefresher.h"

int main(int argc, char** argv)
{
    int i;
    int insertionLimit = stoi(argv[1]);
    int lowerBound = 0;
    int upperBound = argc - 3;
    int* list = new int[upperBound + 1];
    int test = stoi(argv[1]);
    
    for (i = 0; i < upperBound + 1; i++)
    {
        list[i] = stoi(argv[i + 2]);
    }

    mergeSort(list, insertionLimit, lowerBound, upperBound);

    for (i = 0; i < upperBound + 1; i++)
    {
        cout << list[i] << " ";
    }

    delete[] list;
    return 0;
}

void merge(int* currentList, int lowerBound, int midpoint, int upperBound)
{
    int lowerLength = midpoint - lowerBound + 1;
    int upperLength = upperBound - midpoint;
    int* left = new int[lowerLength];
    int* right = new int[upperLength];
    int i, j, k;

    for (i = 0; i < lowerLength; i++)
    {
        left[i] = currentList[lowerBound + i];
    }

    for (j = 0; j < upperLength; j++)
    {
        right[j] = currentList[midpoint + j + 1];
    }

    i = 0;
    j = 0;
    k = lowerBound;

    while (i < lowerLength && j < upperLength)
    {
        if (left[i] < right[j])
        {
            currentList[k] = left[i];
            i++;
        }

        else
        {
            currentList[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < lowerLength)
    {
        currentList[k] = left[i];
        i++;
        k++;
    }

    while (j < upperLength)
    {
        currentList[k] = right[j];
        j++;
        k++;
    }

    delete[] left;
    delete[] right;
}

//GRADING: MERGE
void mergeSort(int* currentList, int insertionLimit, int lowerBound, int upperBound)
{
    if (lowerBound >= upperBound)
    {
        return;
    }

    if ((upperBound - lowerBound) < insertionLimit)
    {
        insertionSort(currentList, lowerBound, upperBound);
        return;
    }

    int midpoint = (upperBound + lowerBound) / 2;

    mergeSort(currentList, insertionLimit, lowerBound, midpoint);
    mergeSort(currentList, insertionLimit, midpoint + 1, upperBound);
    merge(currentList, lowerBound, midpoint, upperBound);
}

//GRADING: INSERTION
void insertionSort(int* currentList, int lowerBound, int upperBound)
{
    int i, j;
    int key;

    for (i = lowerBound + 1; i <= upperBound; i++)
    {
        key = currentList[i];
        j = i - 1;

        while (j >= 0 && currentList[j] > key)
        {
            currentList[j + 1] = currentList[j];
            j--;
        }
        
        currentList[j + 1] = key;
    }
}