/*
 Created by: Sahar Hussain
 COP3503 Assignment #1 Magic Square
 Section: 8228
 */

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