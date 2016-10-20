#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

int **zeroFramedAry;
int neighborAry[8];
int numRows, numCols, minVal, maxVal, newMin, newMax=0;

void initial(string fileName) {
	string str;
	int row = 1, col = 1;
	ifstream myfile(fileName);
	if (myfile.is_open()) {
		int order=0;
		while(myfile>>str) {
			order++;
			if(order==1) numRows = stoi(str);
			else if(order==2) {
				numCols = stoi(str);
				zeroFramedAry = new int*[numRows+2];
				for (int i = 0; i < numRows+2; i++)
					zeroFramedAry[i] = new int[numCols+2];
				for(int i=0; i<numRows+2; i++) {
					zeroFramedAry[i][0] = 0;
					zeroFramedAry[i][numCols+1] = 0;
				}
				for(int j=0; j<numCols+2; j++) {
					zeroFramedAry[0][j] = 0;
					zeroFramedAry[numRows+1][j] = 0;
				}
			} else if(order==3) minVal = stoi(str);
			else if(order==4) {maxVal = stoi(str); newMin=maxVal;}
			else {
				zeroFramedAry[row][col++] = stoi(str);
				if(col>numCols) {row++;	col=1;}
			}
		}
		myfile.close();
	} else {
		cout << "cannot open " << fileName << endl;
		exit(1);
	}
}

void PrettyPrintDistance(ofstream& myfile) {
	for (int i = 1; i < numRows+1; i++) {
		for (int j = 1; j < numCols+1; j++) {
			if (zeroFramedAry[i][j] >0)
				myfile << (int)(zeroFramedAry[i][j]+0.5)<<" ";
			else
				myfile << "  ";
		}
		myfile << endl;
	}
}

void loadNeighbors(int row, int col) {
	neighborAry[0]=(int)(zeroFramedAry[row-1][col-1]+sqrt(2));
	neighborAry[1]=zeroFramedAry[row-1][col]+1;
	neighborAry[2]=(int)(zeroFramedAry[row-1][col+1]+sqrt(2));
	neighborAry[3]=zeroFramedAry[row][col-1]+1;
	neighborAry[4]=zeroFramedAry[row][col+1]+1;
	neighborAry[5]=(int)(zeroFramedAry[row+1][col-1]+sqrt(2));
	neighborAry[6]=zeroFramedAry[row+1][col]+1;
	neighborAry[7]=(int)(zeroFramedAry[row+1][col+1]+sqrt(2));
}

int findMinNeighbor(int row, int col, int pass) {
	int temp=0;
	if(pass==1) {
		temp=neighborAry[0];
		for(int i=1; i<4; i++)
			temp=(neighborAry[i]<temp? neighborAry[i]:temp);
	}
	if(pass==2) {
		temp=zeroFramedAry[row][col];
		for(int i=4; i<8; i++)
			temp=(neighborAry[i]<temp? neighborAry[i]:temp);
	}
	return temp;
}

void firstPassEucleanDistance() {
	for(int i=1; i<numRows+1; i++)
		for(int j=1; j<numCols+1; j++) {
			loadNeighbors(i,j);
			if(zeroFramedAry[i][j]>0)
				zeroFramedAry[i][j]=findMinNeighbor(i,j,1);
		}
}

void secondPassEucleanDistance() {
	for(int i=numRows; i>0; i--)
		for(int j=numCols; j>0; j--) {
			loadNeighbors(i,j);
			if(zeroFramedAry[i][j]>0)
				zeroFramedAry[i][j]=findMinNeighbor(i,j,2);
		}
}

void EucleanDistanceTransform(char **argv) {
	initial(argv[1]);
	ofstream myfile(argv[3]);
	if (myfile.is_open()) {
		firstPassEucleanDistance();
		myfile<<"The result of Pass-1:"<<endl;
		PrettyPrintDistance(myfile);
		myfile<<endl<<"--------------------------------------------------------------------"<<endl;
		secondPassEucleanDistance();
		myfile<<"The result of Pass-2:"<<endl;
		PrettyPrintDistance(myfile);
		myfile.close();
	} else {
		cout << "cannot write " << argv[2] << endl;
		exit(1);
	}
	myfile.open(argv[2]);
	if (myfile.is_open()) {
		for(int i=1; i<numRows+1; i++)
			for(int j=1; j<numCols+1; j++) {
				newMin=(zeroFramedAry[i][j]<newMin? zeroFramedAry[i][j]:newMin);
				newMax=(zeroFramedAry[i][j]>newMax? zeroFramedAry[i][j]:newMax);
			}
		myfile<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<endl;
		PrettyPrintDistance(myfile);
		myfile.close();
	} else {
		cout << "cannot write " << argv[2] << endl;
		exit(1);
	}
}

int main(int argc, char** argv) {
	EucleanDistanceTransform(argv);
	cout << "All work done!" << endl;
	return 0;
}
