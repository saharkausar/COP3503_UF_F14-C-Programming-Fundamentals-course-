/*
 Created by Sahar Kausar (Fall 2014 COP3503 - Introduction to Programming Fundamentals 2 with C++)
 Please do not copy or redistribute without permission.
 
 Assignment: Magic Square
*/

/**
What the program does:
 
This C++ program creates an N*N magic square, 
i.e., a square arrangement of the numbers 1, 2, , , in which the sum of rows, columns, and diagonals are the same. 
The user will specify the size of the square matrix: N. The value N must be an odd number between 3 and 15.

 
Example Run:
 
 For example, the program is expected to run in the following way:
 INPUT>> Enter the size of a magic square: 5
 OUTPUT>> One magic square is:
 17 24 1 8 15
 23 5 7 14 16
 4 6 13 20 22
 10 12 19 21 3
 11 18 25 2 9
 OUTPUT>> Checking the sums of every row: 65 65 65 65 65
 OUTPUT>> Checking the sums of every column: 65 65 65 65 65
 OUTPUT>> Checking the sums of every diagnoal: 65 65
 
**/

/**
How to Compile and Execute:
 
Download the file to your desired root folder on your computer. Open the terminal or executable IDE program to compile and run the file. Enter the following in the quotes (make sure to omit the quotes): "g++ -o (desiredNameOfFile) pa1.cpp"
 
For example, you may enter the following: "g++ -o pa1 pa1.cpp"
 
The program will then compile. Run the program by typing "./(desiredNameOfFile)"
 
In the above example, you may have entered: "./pa1" 
 
The program will then run and the user may follow the prompt. Enjoy!
 
**/


#include <iostream>

using namespace std;

int main()
{
	//Declare Variables
	int n;
	int row;
	int col;
	int startPos;

	//Number to start at
	int fillNumber = 1;

	//Use to iterate through array
	int x = 0;
	int y = 0;


	cout << "Enter the size of the magic square: ";
	cin >> n;

	while (n % 2 == 0)
	{
		cout << "The number you have entered is even.\nPlease enter an odd number: ";
		cin >> n;
	}

	startPos = ((n / 2) + 1);
	y = startPos - 1;
	
	//Allocate memory for a NxN dynamic array
	
	row = n;
	col = n;

	int** magicSquare = new int*[row];
	for (int i = 0; i < row; i++)
		magicSquare[i] = new int[col];


	//Fill Array with zeros

	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			magicSquare[i][j] = 0;
		}
	}
		
	cout << "One magic square is:" << endl;
	
	//Starting Position 
	magicSquare[x][y] = fillNumber;
	fillNumber++;
	
	//FIll Array with Numbers until out of spaces
	while (fillNumber <= n*n)
	{
		int prevX = x;
		int prevY = y;
		
		//Subtract 1 from x to move up through the array
		x = x - 1;
		//Wrapping vertically
		if (x < 0){
			x = n - 1;
		}

		//Add 1 to y to move to the right in the array
		y = y + 1;
		//Wrapping Horizaontally
		if (y == n){
			y = 0;
		}

		//If the segment in the array is already filled with a number return to 
		//previous position and decrement by 1 along the vertical.
		if (magicSquare[x][y] > 0){
			x = prevX + 1;
			y = prevY;
		}

		magicSquare[x][y] = fillNumber;

		//Increment fillNumber to get the next number
		fillNumber++;
	}

	//Output the Array to Console

	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			cout << magicSquare[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;

	//Declare Summation Variables
	int rowSum = 0;
	int colSum = 0;
	int diagnolSum = 0;

	//Add each row and print the result
	cout << "Checking the sums of every row: ";
	
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			int tempNum = magicSquare[i][j];
			rowSum += tempNum;
		}
		cout << rowSum << " ";
		rowSum = 0;
	}
	cout << endl;

	//Add each column and print the result
	cout << "Checking the sums of every column: ";
	
	for (int i = 0; i < col; ++i) {
		for (int j = 0; j < row; ++j) {
			int tempNum = magicSquare[i][j];
			colSum += tempNum;
		}
		cout << colSum << " ";
		colSum = 0;
	}
	cout << endl;

	//Add each diagonal and print the result
	cout << "Checking the sums of every diagonal: ";

	for (int i = 0; i < n; ++i) {
		int tempNum = magicSquare[i][i];
		diagnolSum += tempNum;
	}
		
	cout << diagnolSum << " ";
	diagnolSum = 0;

	for (int i = 0; i < n; ++i) {
		//Using (i - n) * (-1) -1, will allow the second coordinate to start in the bottom left and move towards the right
		int tempNum = magicSquare[i][(i - n) * (-1) -1];
		diagnolSum += tempNum;
	}

	cout << diagnolSum;
	cout << endl;

	//Deallocate the array columns
	for (int i = 0; i < row; i++)
		delete[] magicSquare[i];

	//Delete the dynamic array
	delete[] magicSquare;

	//End of Program
	return 0;
}
