#include <iostream>

using namespace std;

struct Tree
{
	int data;
	int weight;
	Tree* left = nullptr;
	Tree* right = nullptr;
};


void leftToRight(Tree* root);
int sizeTree(Tree* p);
int checkSumTree(Tree* p);
float sumPathLengths(Tree* root, int L = 1);
float averagePathLength(Tree* root);
int maxHeight(int a, int b);
int heightTree(Tree* p);
int weightTree(Tree* root);
float weightedAverageHeightTree(Tree* p);
