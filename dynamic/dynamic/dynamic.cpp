#include "dynamic.h"

int main(int argc, char** argv) {
    ifstream input;
    string fileName = argv[1];
    mailCollection collection;
    int lines, stations, currentNum;

    chrono::time_point<chrono::system_clock> startTime, endTime;
    chrono::duration<double> runtime;


    input.open(fileName);
    input >> lines;
    input >> stations;
    collection.results.resize(stations + 1);

    for (int i = 0; i <= stations; i++)
    {
        collection.results[i].resize(lines);
        for (int j = 0; j < collection.results[i].size(); j++)
        {
            collection.results[i][j].cost = -1;
        }
    }

    collection.costAtLocation.resize(stations + 1);

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < stations; j++)
        {
            input >> currentNum;
            collection.costAtLocation[j].push_back(currentNum);
        }
    }

    collection.costAtLocation[stations].resize(lines);
    collection.costToNextStation.resize(stations + 1);
    collection.costToOtherLine.resize(stations + 1);

    for (int i = 0; i < stations; i++)
    {
        collection.costToOtherLine[i].resize(lines);

        for (int j = 0; j < lines; j++)
        {
            for (int k = 0; k < lines; k++)
            {
                input >> currentNum;

                if (j == k)
                {
                    collection.costToNextStation[i].push_back(currentNum);
                    collection.costToOtherLine[i][j].push_back(0);
                }

                else
                {
                    collection.costToOtherLine[i][j].push_back(currentNum);
                }
            }
        }
    }
    collection.costToNextStation[0].resize(lines);
    collection.costToOtherLine[stations].resize(lines);


    startTime = chrono::system_clock::now();
    findMailRoute(stations, collection);
    endTime = chrono::system_clock::now();
    runtime = endTime - startTime;
    cout << runtime.count() << endl;

    int totalCost, lastLine, currentCost;

    for (int i = 0; i < lines; i++)
    {
        if (i == 0)
        {
            totalCost = collection.results[stations][i].cost;
            lastLine = collection.results[stations][i].lastLine;
        }

        else if (collection.results[stations][i].cost < totalCost)
        {
            totalCost = collection.results[stations][i].cost;
            lastLine = collection.results[stations][i].lastLine;
        }
    }

    currentCost = totalCost;

    vector<result> finalResults;
    finalResults.resize(stations + 1);

    finalResults[stations].cost = currentCost;
    finalResults[stations].lastLine = lastLine;

    for (int i = stations - 1; i >= 0; i--)
    {
        finalResults[i].cost = collection.results[i][lastLine].cost;
        finalResults[i].lastLine = collection.results[i + 1][lastLine].lastLine;
        lastLine = finalResults[i].lastLine;
    }

    cout << "Total Time: " << totalCost << endl << "Line progress: " << endl;
    for (int i = 0; i < stations + 1; i++)
    {
        cout << finalResults[i].cost << "(" << finalResults[i].lastLine << ")" << endl;
    }

    return 0;
}

void findMailRoute(int stationColumn, mailCollection& collection)
{
    int rows = collection.costAtLocation[stationColumn].size();
    if (stationColumn == 0)
    {
        for (int i = 0; i < rows; i++)
        {
            collection.results[stationColumn][i].cost = collection.costAtLocation[stationColumn][i];
            collection.results[stationColumn][i].lastLine = i;
        }
        return;
    }

    result currentBest = {-1, INT_MAX};
    int temp;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            if (collection.results[stationColumn - 1][j].cost == -1)
            {
                findMailRoute(stationColumn - 1, collection);
            }
            if (i == j)
            {
                //GRADING: REUSE
                temp = 
                    collection.results[stationColumn - 1][i].cost +        //cost of fastest route to previous station
                    collection.costToNextStation[stationColumn - 1][i] +   //cost from previous station to current
                    collection.costAtLocation[stationColumn][i];           //cost at current station
                if (temp < currentBest.cost)
                {
                    currentBest.cost = temp;
                    currentBest.lastLine = j;
                }
            }

            else
            {
                temp =
                    collection.results[stationColumn - 1][j].cost +        //cost of fastest route to previous station (diff line)
                    collection.costToNextStation[stationColumn - 1][i] +   //cost from previous station to current
                    collection.costToOtherLine[stationColumn - 1][j][i] +  //cost from j to i at previous station
                    collection.costAtLocation[stationColumn][i];           //cost at current station
                if (temp < currentBest.cost)
                {
                    currentBest.cost = temp;
                    currentBest.lastLine = j;
                }
            }
        }
        //GRADING: UPDATE
        collection.results[stationColumn][i] = currentBest;
        currentBest = { -1, INT_MAX };
    }
}