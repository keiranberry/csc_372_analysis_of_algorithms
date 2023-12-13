#include "FFT.h"
#include "BMPImage.h"
#include "PNGImage.h"
#include <complex>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

const bool VERBOSE = false;

typedef vector<complex<double>> complex1D;
typedef vector<vector<complex<double>>> complex2D;


/*
Helper function to convert a 1D vector of double type, complex values to a w by h 2D
vector of double type, complex values
*/
complex2D makeComplex2D(complex1D v, unsigned int w, unsigned int h)
{
    complex2D newVector(w, complex1D(h));

    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            newVector[i][j] = v[i * w + j];
        }
    }
    return newVector;
}

/*
Helper function to convert a 2D vector of double type, complex values 1D
vector of double type, complex values in row-column order
*/
complex1D makeComplex1D(complex2D v)
{
    size_t w = v.size();
    size_t h = v[0].size();

    complex1D  newV(w * h);

    for (unsigned int i = 0; i < w; i++)
    {
        for (unsigned int j = 0; j < h; j++)
        {
            newV[i * h + j] = v[i][j];
        }
    }
    return newV;
}

/*
Helper function to convert a 1D vector of unsigned chars (pixel values) to
a 1D vector of double type, complex values
*/
complex1D convertToComplex(vector<unsigned char> v)
{
    complex1D newV(v.size());

    for (unsigned int i = 0; i < v.size(); i++)
    {
        newV[i] = complex<double>(v[i]);
    }
    return newV;
}

/*
Helper function to convert a 1D vector of double type, complex values  to
a 1D vector of unsigned chars (pixel values). This pulls only the "magnitude"
values from the complex number. It has minor bounds checking where magnitudes
over 255 are clamped to 255. If unnormalized complex arrays are sent in,
the resulting chars can all be 255.
*/
vector<unsigned char> convertToUnsignedChar(complex1D v)
{
    vector<unsigned char> newV(v.size());

    double magnitude;
    for (unsigned int i = 0; i < v.size(); i++)
    {
        magnitude = sqrt(v[i].real() * v[i].real() + v[i].imag() * v[i].imag());

        if (magnitude > 255)
            magnitude = 255;

        newV[i] = (unsigned char)magnitude;
    }

    return newV;
}



/*
Transpose a matrix by switching its rows and columns
*/
complex2D transpose(complex2D current)
{
    int cols = current[0].size();
    int rows = current.size();
    complex2D output(cols, complex1D(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            output[j][i] = current[i][j];
        }
    }

    return output;
}



/*
Transposes a matrix, runs FFT on its columns, transposes the matrix, runs FFT 
on its rows, then returns the resulting matrix. This can be called for either 
direction of the FFT
*/
complex2D twoDimensionalFFT(complex2D current, FFT::direction direction)
{
    complex1D currentRow;
    complex1D result(current[0].size());

    current = transpose(current);

    for (int i = 0; i < current.size(); i++)
    {
        //run fft on each column
        currentRow = current[i];
        FFT::fft(currentRow, result, direction);
        current[i] = result;
    }

    current = transpose(current);

    for (int i = 0; i < current.size(); i++)
    {
        //run fft on each row
        currentRow = current[i];
        FFT::fft(currentRow, result, direction);
        current[i] = result;
    }

    return current;
}



/*
Adds black bands to the center rows and columns of the 
matrix, with width of the bars passed in as pixels.
*/
complex2D addBlackBands(complex2D current, int pixels)
{
    int startCol = (current[0].size() - pixels) / 2;
    int startRow = (current.size() - pixels) / 2;

    for (int i = startCol; i < startCol + pixels; i++)
    {
        //fill columns with black bars
        for (int j = 0; j < current.size(); j++)
        {
            current[i][j] = complex<double>(0);
        }
    }

    for (int i = startRow; i < startRow + pixels; i++)
    {
        //fill rows with black bars
        for (int j = 0; j < current[0].size(); j++)
        {
            current[j][i] = complex<double>(0);
        }
    }

    return current;
}



/*
Normalizes the matrix by getting the maximum value and 
converting each value in the matrix to its correct value 
in the context of the maximum. Used to prepare a matrix to 
be output as an image.
*/
complex2D normalize(complex2D current)
{
    complex<double> maximum = complex<double>(0);
    
    for (int i = 0; i < current.size(); i++)
    {
        for (int j = 0; j < current[0].size(); j++)
        {
            if (j == 0 && i == 0)
            {
                //we are ignoring current[0][0]
                continue;
            }

            if (abs(current[i][j]) > abs(maximum))
            {
                //update the maximum value in the matrix
                maximum = current[i][j];
            }
        }
    }

    double maxMagnitude = abs(maximum);

    for (int i = 0; i < current.size(); i++)
    {
        for (int j = 0; j < current[0].size(); j++)
        {
            double currentMagnitude = abs(current[i][j]);
            //calculate normalized value using maximum magnitude
            int normalized = (currentMagnitude / maxMagnitude) * 255;

            //make sure normalized value is in the valid range
            if (normalized < 0)
            {
                normalized = 0;
            }
            else if (normalized > 255)
            {
                normalized = 255;
            }

            //fill in the matrix with the normalized values
            current[i][j] = complex<double>(normalized);
        }
    }

    return current;
}


int main(int argc, char* argv[])
{
    //example usage: project2.exe inputFile.xxx intermediateFile.xxx outputFile.xxx pixels

    string inputFileName = argv[1];
    string intermediateFileName = argv[2];
    string outputFileName = argv[3];
    int pixels = stoi(argv[4]);
    unsigned int width, height = 0;
    ifstream input;
    ofstream output, intermediate;
    vector<unsigned char> dataChar;
    complex1D data;
    complex2D data2D;
    complex2D normalized;

    string inputFileType = inputFileName.substr(inputFileName.length() - 3);
    string intermediateFileType = intermediateFileName.substr(intermediateFileName.length() - 3);
    string outputFileType = outputFileName.substr(outputFileName.length() - 3);

    input.open(inputFileName);

    if (inputFileType == "png" || inputFileType == "PNG")
    {
        dataChar = loadPNG(inputFileName, width, height);
    }
    else
    {
        dataChar = loadBMP(inputFileName, width, height);
    }

    //convert image from file to complex2D and run fft on it 
    data = convertToComplex(dataChar);
    data2D = makeComplex2D(data, width, height);
    data2D = twoDimensionalFFT(data2D, FFT::FORWARD);

    //add the black bands
    data2D = addBlackBands(data2D, pixels);

    //normalize the complex2D and convert it to unsigned char for output
    normalized = normalize(data2D);
    data = makeComplex1D(normalized);
    dataChar = convertToUnsignedChar(data);
    
    if (intermediateFileType == "png" || intermediateFileType == "PNG")
    {
        savePNG(intermediateFileName, width, height, dataChar);
    }
    else
    {
        saveBMP(intermediateFileName, width, height, dataChar);
    }

    //do the inverse fft on the matrix
    //normalize the complex2D and convert it to unsigned char for output
    data2D = twoDimensionalFFT(data2D, FFT::INVERSE);
    normalized = normalize(data2D);
    data = makeComplex1D(normalized);
    dataChar = convertToUnsignedChar(data);

    if (outputFileType == "png" || outputFileType == "PNG")
    {
        savePNG(outputFileName, width, height, dataChar);
    }
    else
    {
        saveBMP(outputFileName, width, height, dataChar);
    }

    return 0;
}

