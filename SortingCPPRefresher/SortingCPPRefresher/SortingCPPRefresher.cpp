#include "SortingCPPRefresher.h"

int main(int argc, char** argv)
{
    string test = argv[1];

    if (strcmp(argv[1], "t") == 0)
    {
        runWithTests();
    }

    else
    {
        runWithoutTests(argc, argv);
    }

    return 0;
}

void runWithTests()
{
    kWithTime bestK;
    bestK.time = 100;

    ofstream output;
    output.open("refresherOutput.csv");
    output << "k, 1st Time, 2nd Time, 3rd Time, Average" << endl;
    setIncrementAndSendToTests(output, 1, bestK);    //test k from 1-10
    setIncrementAndSendToTests(output, 10, bestK);   //test k from 10-100
    setIncrementAndSendToTests(output, 100, bestK);  //test k from 100-1000
    setIncrementAndSendToTests(output, 1000, bestK); //test k from 1000-10000
    cout << "Best K: " << bestK.value << endl;
    runOrderedAndRandomTests();                      //run the four final tests

    output.close();
}

void setIncrementAndSendToTests(ofstream& output, int incrementBy, kWithTime& bestK)
{
    int i, start, end, increment;
    float timeOne, timeTwo, timeThree, averageTime;
    auto startTime = chrono::system_clock::now();
    auto endTime = chrono::system_clock::now();

    switch (incrementBy)     //set values for the for loop depending on increment
    {
        case 1:
            start = 1;
            end = 11;
            increment = 1;
            break;
        case 10:
            start = 20;
            end = 101;
            increment = 10;
            break;
        case 100:
            start = 200;
            end = 1001;
            increment = 100;
            break;
        case 1000:
            start = 2000;
            end = 10001;
            increment = 1000;
            break;
    }

    for (i = start; i < end; i = i + increment)
    {
        output << i << ",";
        timeOne = testWithIncrements(output, i);           //get first time for k
        output << ",";
        timeTwo = testWithIncrements(output, i);           //get second time for k
        output << ",";
        timeThree = testWithIncrements(output, i);         //get third time for k
        averageTime = (timeOne + timeTwo + timeThree) / 3; //average them
        output << "," << averageTime << endl;

        if (averageTime < bestK.time)
        {
            bestK.time = averageTime;                      //if the new k is faster than the old
            bestK.value = i;                               //update the best k
        }
    }
}

float testWithIncrements(ofstream& output, int insertionLimit)
{
    int lowerBound = 0;
    int upperBound = 9999;
    int* list = getRandomList();                           //get a new random list
    
    auto startTime = chrono::system_clock::now();
    mergeSort(list, insertionLimit, lowerBound, upperBound);
    auto endTime = chrono::system_clock::now();
    if (!checkList(list))                                  //if the list isnt sorted right, say so
    {
        cout << "There was a problem sorting" << endl;
        exit(0);
    }

    auto timeTaken = chrono::duration_cast<chrono::duration<float>>(endTime - startTime);
    output << timeTaken.count();                           //output time taken

    delete[] list;
    return timeTaken.count();
}

void runOrderedAndRandomTests()
{
    int listOne[10000];
    int* listTwo = getRandomList();

    int i, randomNumber, negativeDecider;

    for (i = 0; i < 10000; i++)
    {
        listOne[i] = i;                       //fill sorted list
        randomNumber = 1 + (rand() % 30000);
        negativeDecider = 1 + (rand() % 2);

        if (negativeDecider % 2 == 1)
        {
            randomNumber = 0 - randomNumber;
        }

        listTwo[i] = randomNumber;           //fill unsorted list
    }

    auto startTime = chrono::system_clock::now();//sorted k=1
    mergeSort(listOne, 1, 0, 9999);
    auto endTime = chrono::system_clock::now();
    auto timeTaken = chrono::duration_cast<chrono::duration<float>>(endTime - startTime);
    cout << "K=1 Ordered list time in seconds: " << timeTaken.count() << endl;

    startTime = chrono::system_clock::now();//sorted k=n
    mergeSort(listOne, 10000, 0, 9999);
    endTime = chrono::system_clock::now();
    timeTaken = chrono::duration_cast<chrono::duration<float>>(endTime - startTime);
    cout << "K=N Ordered list time in seconds: " << timeTaken.count() << endl;
    
    startTime = chrono::system_clock::now();//random k=1
    mergeSort(listTwo, 1, 0, 9999);
    endTime = chrono::system_clock::now();
    timeTaken = chrono::duration_cast<chrono::duration<float>>(endTime - startTime);
    cout << "K=1 Random list time in seconds: " << timeTaken.count() << endl;
    
    listTwo = getRandomList();//reset to a random list
    startTime = chrono::system_clock::now();//random k=n
    mergeSort(listTwo, 10000, 0, 9999);
    endTime = chrono::system_clock::now();
    timeTaken = chrono::duration_cast<chrono::duration<float>>(endTime - startTime);
    cout << "K=N Random list time in seconds: " << timeTaken.count() << endl;

    delete[] listTwo;
}

bool checkList(int currentList[])
{
    for (int i = 0; i < sizeof(currentList) - 1; i++)
    {
        if (currentList[i] > currentList[i + 1])
        {
            return false;//if the list is out of order anywhere
        }
    }

    return true;
}

int* getRandomList()
{
    int* list = new int[10000];

    for (int i = 0; i < 10000; i++)
    {
        int randomNumber = 1 + (rand() % 30000);
        int negativeDecider = 1 + (rand() % 2);

        if (negativeDecider % 2 == 1)
        {
            randomNumber = 0 - randomNumber;
        }

        list[i] = randomNumber; //fill in a new list with random numbers -30000 to 30000
    }

    return list;
}

void runWithoutTests(int argc, char** argv)
{
    int i;
    int insertionLimit = stoi(argv[1]);
    int lowerBound = 0;
    int upperBound = argc - 3;
    int* list = new int[upperBound + 1];

    for (i = 0; i < upperBound + 1; i++)
    {
        list[i] = stoi(argv[i + 2]); //fill in array with command line arguments
    }

    mergeSort(list, insertionLimit, lowerBound, upperBound);

    for (i = 0; i < upperBound + 1; i++)
    {
        cout << list[i] << " "; //output the sorted list
    }

    delete[] list;
}

void merge(int* currentList, int lowerBound, int midpoint, int upperBound)
{   //merge algorithm from the book
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
{   //merge sort from the book
    if (lowerBound >= upperBound)
    {
        return;
    }

    if ((upperBound - lowerBound) < insertionLimit)
    {   //added this to be able to take in a k
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
{   //insertion sort from the book, args modified to take in bounds
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