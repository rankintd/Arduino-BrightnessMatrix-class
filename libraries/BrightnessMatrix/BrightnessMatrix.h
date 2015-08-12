#ifndef BrightnessMatrix_h
#define BrightnessMatrix_h
#ifdef __cplusplus

#include "elapsedMillis.h"

const int nROWS = 8;
const int nCOLS = 8;

const int PINS[nROWS] = {10, 11, 12, 13, 14, 15, 16, 17};   //The pins used to control the anode of the display

const int ROW_DELAY_TIME = 160;

const int DEFAULT_MAX_BRIGHTNESS = 15;

typedef byte ElemType;
ElemType whoops = 0;					//accessed only when an array index is out of bounds

class BrightnessMatrix
{
protected:
	ElemType brArray[nROWS][nCOLS];				//will consist of values from 0 to maxBrightnessLED

	int displayCount;
	int maxBrightnessLED;
	int showZeroBr;
	
public:
	BrightnessMatrix();
	BrightnessMatrix(const ElemType arr[][nCOLS]);
	BrightnessMatrix(const ElemType arr[][nCOLS], int max_Br, int show_Zero);

	void showRow(int rowNo) const;				//make private/protected, possibly

	void show(unsigned int millisec);
	
	void clear();
	void importArray(const ElemType arr[][nCOLS]);
	void importArray(const ElemType arr[][nCOLS], int max_Br, int show_Zero);

	void setShowZero(int show_Zero);

	void wrap(int xShift, int yShift);
	void swapRows(int rowA, int rowB);
	void swapCols(int colA, int colB);

	void reorient(int dir);

	void rotate90()				{if(nROWS==nCOLS) reorient(1);}
	void rotate180()			{reorient(2);}
	void rotate270()			{if(nROWS==nCOLS) reorient(3);}
	void transpose()			{if(nROWS==nCOLS) reorient(7);}
	void flipVert()				{reorient(6);}
	void flipAntidiag()			{if(nROWS==nCOLS) reorient(5);}
	void flipHoriz()			{reorient(4);}

	//ObjectName(a, b) returns a reference to brArray[a][b] (or to a dummy element if out of range)
	ElemType& operator()(int row, int col)
	{
		if(row < 0 || row >= nROWS || col < 0 || col >= nCOLS) return whoops;
		return brArray[row][col];
	}
};

//default constructor
BrightnessMatrix::BrightnessMatrix()
{
	clear();
	displayCount = 0;
	maxBrightnessLED = DEFAULT_MAX_BRIGHTNESS;
	showZeroBr = 0;
}

//constructor
BrightnessMatrix::BrightnessMatrix(const ElemType arr[][nCOLS])
{
	importArray(arr);
}

//constructor
//***ASSUMES THE ARRAY ARGUMENT PASSED IN HAS AT LEAST nROWS ROWS***
BrightnessMatrix::BrightnessMatrix(const ElemType arr[][nCOLS], int max_Br, int show_Zero)
{
	importArray(arr, max_Br, show_Zero);
}

//outputs the indicated row to the LED board based on the contents of brArray.
void BrightnessMatrix::showRow(int rowNo) const
{
	if(rowNo < 0 || rowNo >= nROWS) return;

	ElemType n;
	int j, coeff;

	if(maxBrightnessLED <= 6)
	{
		coeff = 12/maxBrightnessLED;
		for (j=0; j<nCOLS; j++)
		{
			n=brArray[rowNo][j];
			if(n>maxBrightnessLED) n=maxBrightnessLED;

			if(n==maxBrightnessLED)
				digitalWrite(2+j, LOW);
			else if(displayCount % (coeff*(maxBrightnessLED-n)) == 0 && (n>0 || showZeroBr))
				digitalWrite(2+j, LOW);
			else
				digitalWrite(2+j, HIGH);
		}
	}
	else
	{
		for (j=0; j<nCOLS; j++)
		{
			n=brArray[rowNo][j];
			if(n>maxBrightnessLED) n=maxBrightnessLED;

			if(n==maxBrightnessLED)											//always display
				digitalWrite(2+j, LOW);
			else if(n==maxBrightnessLED-1 && displayCount % 3 > 0)			//display 2/3 of the time
				digitalWrite(2+j, LOW);
			else if(displayCount % (maxBrightnessLED-n) == 0 && (n>0 || showZeroBr))
				digitalWrite(2+j, LOW);
			else
				digitalWrite(2+j, HIGH);
		}
	}

	digitalWrite(PINS[rowNo], HIGH);
	delayMicroseconds(ROW_DELAY_TIME);
}

//displays the board represented by brArray for (at least) the specified number of milliseconds.
void BrightnessMatrix::show(unsigned int millisec)
{
	int k;
	elapsedMillis eM;
	while(eM < millisec)
	{
		for(k=0; k<nROWS; k++)
		{
			showRow(k);
			digitalWrite(PINS[k], LOW);
		}
		displayCount++;
		//if(displayCount>99999999) displayCount=0;
	}
}

//sets all entries in brArray to zero
void BrightnessMatrix::clear()
{
	int i, j;
	for(i=0; i<nROWS; i++)
		for(j=0; j<nCOLS; j++)
			brArray[i][j] = 0;
}

//see comment below
void BrightnessMatrix::importArray(const ElemType arr[][nCOLS])
{
	importArray(arr, DEFAULT_MAX_BRIGHTNESS, 0);
}

//sets the entries in brArray to values from 0 to maxBrightnessLED based on the input array.
//the input array should contain values from 0 to 255 (representing a grayscale image).
//zero brightness will correspond to the smallest values in the input array;
//maxBrightnessLED will correspond to the largest values.
//***ASSUMES THE ARGUMENT PASSED IN HAS AT LEAST nROWS ROWS***
void BrightnessMatrix::importArray(const ElemType arr[][nCOLS], int max_Br, int show_Zero)
{
	setShowZero(show_Zero);

	if(max_Br<1) max_Br=1;
	else if(max_Br>32) max_Br=32; 
	maxBrightnessLED = max_Br;

	displayCount = 0;

	int i, j;

	ElemType n;
	
	int max=0;
	int min=255;
	int diff;

	for(i=0; i<nROWS; i++)
		for(j=0; j<nCOLS; j++)
		{
			n=arr[i][j];
			if(n>max) max=n;
			if(n<min) min=n;
		}
	diff = max-min+1;
	
	for(i=0; i<nROWS; i++)
		for(j=0; j<nCOLS; j++)
		{
			n=arr[i][j]-min;			//now 0 <= n <= diff-1
			brArray[i][j] = ((maxBrightnessLED+1)*n)/diff;

			if(brArray[i][j]<0)
				brArray[i][j]=0;
			else if(brArray[i][j]>maxBrightnessLED)
				brArray[i][j]=maxBrightnessLED;
		}
}

//sets whether LEDs with zero brightness are shown
void BrightnessMatrix::setShowZero(int show_Zero)
{
	showZeroBr = show_Zero;
}

//shifts the values of theArray by the specified values, wrapping around when passing the edge.
//the value originally in theArray[0][0] will end up in theArray[vShift][hShift] (for indices in range)
void BrightnessMatrix::wrap(int vShift, int hShift)
{
	ElemType temp[nROWS][nCOLS];
	int i, j;
	for(i=0; i<nROWS; i++)
		for(j=0; j<nCOLS; j++)
			temp[i][j] = brArray[i][j];

	for(i=0; i<nROWS; i++)
		for(j=0; j<nCOLS; j++)
		{
			int r = (i+vShift) % nROWS;
			if(r<0) r+=nROWS;

			int c = (j+hShift) % nCOLS;
			if(c<0) c+=nCOLS;

			brArray[r][c] = temp[i][j];
		}
}

//swaps the contents of the specified rows.
//(does nothing if an index is out of range)
void BrightnessMatrix::swapRows(int rowA, int rowB)
{
	if(rowA < 0 || rowA >= nROWS || rowB < 0 || rowB >= nROWS || rowA == rowB) return;

	ElemType temp[nCOLS];
	int i;
	for(i=0; i<nCOLS; i++)
	{
		temp[i] = brArray[rowA][i];
		brArray[rowA][i] = brArray[rowB][i];
		brArray[rowB][i] = temp[i];
	}
}

//swaps the contents of the specified columns.
//(does nothing if an index is out of range)
void BrightnessMatrix::swapCols(int colA, int colB)
{
	if(colA < 0 || colA >= nCOLS || colB < 0 || colB >= nCOLS || colA == colB) return;

	ElemType temp[nROWS];
	for(int i=0; i<nROWS; i++)
	{
		temp[i] = brArray[i][colA];
		brArray[i][colA] = brArray[i][colB];
		brArray[i][colB] = temp[i];
	}
}

//rearranges the values in brArray to correspond to a rotation/reflection of the LED board
void BrightnessMatrix::reorient(int dir)
{
	if(dir <= 0 || dir >= 8) return;
	if((nROWS != nCOLS) && (dir % 2)) return;

	ElemType temp[nROWS][nCOLS];
	int i, j;
	for(i=0; i<nROWS; i++)
		for(j=0; j<nCOLS; j++)
			temp[i][j] = brArray[i][j];

	switch(dir)							//case 7-n is the transpose of case n
	{
	case 1:		//rotate90
		for(i=0; i<nROWS; i++)
			for(j=0; j<nCOLS; j++)
				brArray[i][j] = temp[nCOLS-1-j][i];
		break;
	case 2:		//rotate180
		for(i=0; i<nROWS; i++)
			for(j=0; j<nCOLS; j++)
				brArray[i][j] = temp[nROWS-1-i][nCOLS-1-j];
		break;
	case 3:		//rotate270
		for(i=0; i<nROWS; i++)
			for(j=0; j<nCOLS; j++)
				brArray[i][j] = temp[j][nROWS-1-i];
		break;
	case 4:		//flipHoriz
		for(i=0; i<nROWS; i++)
			for(j=0; j<nCOLS; j++)
				brArray[i][j] = temp[i][nCOLS-1-j];
		break;
	case 5:		//flipAntidiag
		for(i=0; i<nROWS; i++)
			for(j=0; j<nCOLS; j++)
				brArray[i][j] = temp[nCOLS-1-j][nROWS-1-i];
		break;
	case 6:		//flipVert
		for(i=0; i<nROWS; i++)
			for(j=0; j<nCOLS; j++)
				brArray[i][j] = temp[nROWS-1-i][j];
		break;
	case 7:		//transpose
		for(i=0; i<nROWS; i++)
			for(j=0; j<nCOLS; j++)
				brArray[i][j] = temp[j][i];
		break;
	}
}

#endif
#endif