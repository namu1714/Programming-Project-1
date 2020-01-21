#include <float.h>
#include <vector>
#include <stdlib.h>
#include "..\ExternDoc.h"
#include "..\UI\Zoom.h"
#include "..\UI\MsgView.h"
#include "..\Graphics\DrawFunc.h"
#include "..\Example\Example.h"

#define ERROR_NUMBER -1

typedef struct element{
	int row;
	int col;
	int dir;
}element;
std::vector<element> S; //stack
std::vector<element> Q; //queue

//stack function
void push(element item){
	S.push_back(item);
}

element pop(){
	element tmp = S.back();
	S.pop_back();
	return tmp;
}

//queue function
void addq(element item){
	Q.push_back(item);
}

element deleteq(){
	element tmp = Q.front();
	Q.erase(Q.begin());
	return tmp;
}

//function prototype
static void drawDirect(CDC *pDC);
static void drawBuffered();

//Start of user code
std::vector<std::vector<bool> > maze;
int width, height;
bool **mark_dfs;
bool **mark_bfs;
element **parent;

int dx[]={-1,1,0,0};
int dy[]={0,0,-1,1};
bool DFSflag, BFSflag;
bool DFS_now, BFS_now;

void Error_Exit(char *s) {//For debugging only
	showMessage(s);
	exit(-1); //you may break here for possible error
}

/*****************************************************************
* function	: bool readFile(const char* filename)
* argument	: cons char* filename - filename to be opened
* return	: true if success, otherwise flase
* remark	: After read data file, phisycal view must be set;
*			  otherwise drawing will not be executed correctly.
*			  The window will be invalidated after readFile()
*			  returns true.
******************************************************************/

bool readFile(const char* filename){
	char c;
	FILE *fp;

	fp = fopen(filename, "rt");
	if(fp == NULL) {
		Error_Exit("Input Error!");
	}

	// Read first line to determine WIDTH.
	while((c = fgetc(fp)) != '\n') {
		if(c == EOF) {
			Error_Exit("Input Error!");
		}
		if(c == '+') {
			width++;
		}
	}
	width = width * 2 - 1;
	fclose(fp);

	// Fill the maze vector
	fp = fopen(filename, "rt");
	if(fp == NULL) {
		Error_Exit("Input Error!");
	}
	while(1) {
		std::vector<bool> line(width);

		for(int i = 0; i < width; i++) {
			c = fgetc(fp);

			if(c == EOF || c == '\n') {
				break;
			}

			// Fill buffer according to input character
			switch(c) {
			case '+':
			case '|':
			case '-':
				line[i] = true;
				break;
			case ' ':
				line[i] = false;
				break;
			default: 
				Error_Exit("Input Error!");
			}
		}

		if(c == EOF || c == '\n') {
			break;
		}

		maze.push_back(line);
		height++;

		// 다음줄로 이동
		while(1) {
			c = fgetc(fp);
			if(c == EOF || c == '\n') {
				break;
			}
		}

		if(c == EOF) break;
	}

	fclose(fp);
	setWindow(0, 0, 5*(width+4), 5*(height+4), 1); //화면 크기 맞추기

	DFSflag = false;
	BFSflag = false;
	DFS_now = false;
	BFS_now = false;
	return true;
}

/******************************************************************
* function	: bool FreeMemory()
*
* remark	: Save user data to a file
*******************************************************************/
void freeMemory(){
	width = 0;
	height = 0;
	std::vector<std::vector<bool> >().swap(maze);
	std::vector<element>().swap(S);
	std::vector<element>().swap(Q);
	if(DFSflag){
		for(int i=0;i<height; i++)
			free(mark_dfs[i]);
		free(mark_dfs);
	}
	if(BFSflag){
		for(int i=0;i<height; i++){
			free(mark_bfs[i]);
			free(parent[i]);
		}
		free(mark_bfs);
		free(parent);
	}
}

/**************************************************************
* function	: bool writeFile(const char* filename)
*
* argument	: const char* filename - filename to be written
* return	: true if success, otherwise false
* remark	: Save user data to a file
****************************************************************/
bool writeFile(const char* filename){
	//start of the user code
	bool flag;
	flag = 0;

	return flag;
	//end of usercode
}

/************************************************************************
* fucntion	: void drawMain(CDC* pDC)
*
* argument	: CDC* pDC - device context object pointer
* remark	: Main drawing function. Called by CMFC_MainView::OnDraw()
*************************************************************************/
void drawMain(CDC *pDC){
	//if direct drawing is defined
#if defined(GRAPHICS_DIRECT)
	drawDirect(pDC);
	//if buffered drawing is defined
#elif defined(GRAPHICS_BUFFERED)
	drawBuffered();
#endif
}

/************************************************************************
* function	: static void drawDirect(CDC *pDC
*
* argument	: CDC* pDC - device context object pointer
* remark	: Direct drawing routines here.
*************************************************************************/
static void drawDirect(CDC *pDC){
	//begin of user code
	//Nothing to write currently.
	//end of user code
}

/***********************************************************************
* function	: static void drawBuffered()
*
* argument	: CDC* pDC -0 device object pointer
* remark	: Buffered drawing routines here.
************************************************************************/
static void drawBuffered(){
	double x = 2.0;
	double y = 2.0;
	int i,j, pi, pj;

	// Draw a maze on screen that resembles the MAZE vector
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			if(maze[i][j]) {
				DrawSolidBox_I(x+5*j, y+5*i, x+5*(j+1), y+5*(i+1), 0, RGB(121,171,255), RGB(121,171,255));
			}
		}
	}

	//draw path
	if(DFSflag && DFS_now){
		for(i=0; i<height-1; i++){
			for(j=0; j<width-1; j++){
				if(mark_dfs[i][j] && mark_dfs[i][j+1])
					DrawLine_I(4.5+5*j, 4.5+5*i, 4.5+5*(j+1), 4.5+5*i, 5, RGB(166,166,166));
				if(mark_dfs[i][j] && mark_dfs[i+1][j])
					DrawLine_I(4.5+5*j, 4.5+5*i, 4.5+5*j, 4.5+5*(i+1), 5, RGB(166,166,166));
			}
		}
		for(i=0; i<S.size()-1; i++){
			DrawLine_I(4.5+5*S[i].col, 4.5+5*S[i].row, 4.5+5*S[i+1].col, 4.5+5*S[i+1].row, 5, RGB(50,50,50));
		}
	}

	if(BFSflag && BFS_now){
		for(i=0; i<height-1; i++){
			for(j=0; j<width-1; j++){
				if(mark_bfs[i][j] && mark_bfs[i][j+1])
					DrawLine_I(4.5+5*j, 4.5+5*i, 4.5+5*(j+1), 4.5+5*i, 5, RGB(166,166,166));
				if(mark_bfs[i][j] && mark_bfs[i+1][j])
					DrawLine_I(4.5+5*j, 4.5+5*i, 4.5+5*j, 4.5+5*(i+1), 5, RGB(166,166,166));
			}
		}

		i = height-2; j = width-2;
		while(parent[i][j].row>0 && parent[i][j].col>0){
			pi = parent[i][j].row; pj = parent[i][j].col;
			DrawLine_I(4.5+5*j, 4.5+5*i, 4.5+5*pj, 4.5+5*pi, 5, RGB(50,50,50));
			i = pi;
			j = pj;
		}
	}

	//draw point
	DrawSolidBox_I(3+5, 3+5, 6+5, 6+5, 0, RGB(47,237,40), RGB(47,237,40));
	DrawSolidBox_I(3+5*(width-2), 3+5*(height-2), 6+5*(width-2), 6+5*(height-2), 0, RGB(255,0,0), RGB(255,0,0));
}

void DFS(){
	int i, j;
	int row, col, dir, nextRow, nextCol;
	element position;
	bool found = false;
	DFS_now = true;
	BFS_now = false;
	if(!DFSflag){
		//mark 배열 생성
		mark_dfs = (bool**)malloc(sizeof(bool*)*height);
		for(i=0;i<height;i++)
		mark_dfs[i] = (bool*)malloc(sizeof(bool)*width);
		for(i=0;i<height;i++){
			for(j=0;j<width;j++)
				mark_dfs[i][j] = false;
		}

		//길 탐색
		position.row=1; position.col=1; position.dir=1;
		push(position);
		mark_dfs[1][1]=true;
		while(!(S.empty()) && !found){
			position = pop();
			row=position.row; col=position.col; dir=position.dir;
			while(dir < 4){
				nextRow = row+dy[dir];
				nextCol = col+dx[dir];
				if(nextRow == height-2 && nextCol == width-2){
					found=true;
					break;
				}
				else if(!maze[nextRow][nextCol] && !mark_dfs[nextRow][nextCol]){
					mark_dfs[nextRow][nextCol] = true;
					position.row=row; position.col=col; position.dir=++dir;
					push(position);
					row=nextRow; col=nextCol; dir=0;
				}
				else ++dir;
			}
		}
		if(found==true){
			mark_dfs[nextRow][nextCol]=true;
			position.row=row; position.col=col;
			push(position);
			position.row=height-2; position.col=width-2;
			push(position);
			DFSflag = true;
		}
	}
}

void BFS(){
	int i, j;
	int row, col, dir, nextRow, nextCol;
	element position;
	bool found = false;
	DFS_now = false;
	BFS_now = true;

	if(!BFSflag){
		//배열 생성
		mark_bfs = (bool**)malloc(sizeof(bool*)*height);
		parent = (element**)malloc(sizeof(element*)*height);
		for(i=0;i<height;i++){
			mark_bfs[i] = (bool*)malloc(sizeof(bool)*width);
			parent[i] = (element*)malloc(sizeof(element)*width);
		}
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				mark_bfs[i][j] = false;
				parent[i][j].col = -1;
				parent[i][j].row = -1;
				parent[i][j].dir = -1;
			}
		}

		//길 탐색
		position.row=1; position.col=1; position.dir=0;
		addq(position);
		mark_bfs[1][1]=true;
		while(!Q.empty() && !found){
			position = deleteq();
			row = position.row; col = position.col; dir = position.dir;
			for(dir=0;dir<4;dir++){
				nextRow = row+dy[dir];
				nextCol = col+dx[dir];
				if(nextRow == height-2 && nextCol == width-2){
					if(parent[nextRow][nextCol].row==-1){
						parent[nextRow][nextCol].row=row;
						parent[nextRow][nextCol].col=col;
					}
					found=true;
					break;
				}
				else if(!maze[nextRow][nextCol] && !mark_bfs[nextRow][nextCol]){
					parent[nextRow][nextCol].row=row;
					parent[nextRow][nextCol].col=col;
					position.row = nextRow; position.col = nextCol;
					position.dir = dir;
					addq(position);
					mark_bfs[nextRow][nextCol] = true;
				}
			}
		}
		if(found==true){
			mark_bfs[nextRow][nextCol] = true;
			BFSflag = true;
			position.row=nextRow; position.col=nextCol;
		}
	}
}