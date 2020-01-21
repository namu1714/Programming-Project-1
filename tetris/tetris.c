#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;
	createRankList();
	initscr();
	noecho();
	keypad(stdscr, TRUE);	
	
	recRoot=(RecNode *)malloc(sizeof(RecNode));
	recRoot->lv=-1;
	recRoot->score=0;
	recRoot->f=field;
	constructRecTree(recRoot);
	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK:	rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}
	writeRankFile();
	destructRecTree(recRoot);
	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	recommend(recRoot);
	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j, k;
	for( k = 1; k <= 2; k++){ 
		for( i = 0; i < 4; i++ ){
			move(4+i+(k-1)*6,WIDTH+13);
			for( j = 0; j < 4; j++ ){
				if( block[nextBlock[k]][0][i][j] == 1 ){
					attron(A_REVERSE);
					printw(" ");
					attroff(A_REVERSE);
				}
				else printw(" ");
			}
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	for(int i=0; i<4; i++){
		for(int j=0; j<4;j++){
			if(block[currentBlock][blockRotate][i][j] == 1)
			{
				if((blockY+i)>=HEIGHT) return 0;
				if((blockX+j)<0 || (blockX+j)>=WIDTH) return 0;
				if(f[blockY+i][blockX+j]) return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	int x=blockX, y=blockY, blk=currentBlock, rot=blockRotate, shadowY;
	int i, j;
	switch(command){
		case KEY_UP:
			rot=(rot+3)%4;
			break;
		case KEY_DOWN:
			y--;
			break;
		case KEY_LEFT:
			x++;
			break;
		case KEY_RIGHT:
			x--;
	}
	
	shadowY = y;
	while(CheckToMove(field, nextBlock[0], rot, shadowY+1,x))
		shadowY++;

	for(i=0;i<4;i++){
		for(j=0;j<4;j++)
			if(block[blk][rot][i][j]==1){
				if(i+y>=0){
					move(y+i+1, x+j+1);
					printw(".");
					move(shadowY+i+1, x+j+1);
					printw(".");
				}
			}	
	}
	

	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	move(HEIGHT, WIDTH+10);
}

void BlockDown(int sig){
	int touched;
	if(CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)==1){
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else{
		if(blockY == -1)
			gameOver = 1;
		touched=AddBlockToField(field,nextBlock[0], blockRotate, blockY,blockX);
		score+=touched;
		score+=DeleteLine(field);
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand()%7;
		recommend(recRoot);
		DrawNextBlock(nextBlock);
		blockRotate=0;
		blockY=-1;
		blockX=WIDTH/2-2;
		DrawField();
		PrintScore(score);
	}
	timed_out=0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	int touched = 0;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(block[currentBlock][blockRotate][i][j] == 1){
				f[blockY+i][blockX+j] = 1;
				if(blockY+i+1==HEIGHT || f[blockY+i+1][blockX+j]==1)
					touched++;
			}
		}
	}
	return 10*touched;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	int lineFull, count = 0;
	int i,j,k;
	for(i=0; i<HEIGHT; i++){
		lineFull = 1;
		for(j=0; j<WIDTH; j++){
			if(f[i][j] == 0){
				lineFull = 0;
				break;
			}
		}
		if(lineFull == 1){
			count++;
			for(k=i;k>0;k--){
				for(j=0; j<WIDTH; j++){
					f[k][j] = f[k-1][j];
				}
			}
		}
	}
	return 100*count*count;
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	int i,j, shadowY=y;
	while(CheckToMove(field, nextBlock[0], blockRotate, shadowY+1, x))
		shadowY++;

	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0 ){
				move(i+shadowY+1, j+x+1);
				attron(A_REVERSE);
				printw("/");
				attroff(A_REVERSE);
			}
		}
	} 
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawRecommend();
	DrawBlock(y, x, blockID, blockRotate,' ');
	DrawShadow(y, x, blockID, blockRotate);
}

void createRankList(){
	int num, i, score;
	char name[NAMELEN];
	node_pointer pre,temp;
	FILE* fr;
	if((fr = fopen("rank.txt", "r"))==NULL) return;
	fscanf(fr, "%d\n", &num);
	len = num;	

	if(!num) return;
	fscanf(fr, "%s %d\n",name, &score);
	pre = (node_pointer)malloc(sizeof(Node));
	strcpy(pre->name, name);
	pre->score = score;
	pre->link = NULL;
	first = pre;
	
	for(i=1; i<num; i++){
		fscanf(fr, "%s %d\n", name, &score);
		temp = (node_pointer)malloc(sizeof(Node));
		strcpy(temp->name, name);
		temp->score = score;
		pre->link = temp;
		temp->link = NULL;
		pre = pre->link;
	}
	fclose(fr);
	return;		
}

void rank(){
	int from, to, nameflag;
	char name[NAMELEN];
	int del;
	int count;
	node_pointer temp, delNode;
	
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	
	switch(wgetch(stdscr)){
	case '1':
		echo();
		printw("X: ");
		scanw("%d", &from);
		printw("Y: ");
		scanw("%d", &to);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		
		if(from==NULL) from = 0;
		if(to==NULL) to = len;
		
		if(from>to){
			printw("\nsearch failure: no rank in the list\n");
			break;
		}
		temp = first;
		count = 0;
		count++;
		while(1){
			if(temp==NULL) break;
			if(count>=from && count<=to){
				printw(" %-17s| %-10d\n", temp->name, temp->score);
			}
			temp = temp->link;
			count++;
		}
		break;

	case '2':
		nameflag = 0;
		printw("input the name: ");
		echo();
		getstr(name);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");
			
		temp = first;
		while(temp != NULL){
			if(strcmp(temp->name, name)==0){
				printw(" %-17s| %-10d\n", temp->name, temp->score);
				nameflag = 1;
			}
			temp = temp->link;
		}
		if(nameflag == 0)
			printw("\nsearch failure: no name in the list\n");
		break;
	
	case '3':
		printw("input the rank: ");
		echo();
		scanw("%d", &del);
		noecho();

		if(del>len || del<=0){
			printw("\nsearch failure: the rank not in the list\n");
		}
		else{
			temp = first;
			delNode = first;
			for(count=1; count<del; count++){
				temp = delNode;
				delNode = delNode->link;
			}
			if(delNode == first) 
				{first = first->link;}
			else
				{temp->link = delNode->link;}
			free(delNode);
			len--;
			printw("\nresult: the rank deleted\n");
		}
		break;
	
	default: break;
	}
	getch();
}

void writeRankFile(){
	node_pointer cur, temp;
	FILE* fw;
	fw = fopen("rank.txt", "w");
	fprintf(fw, "%d\n", len);

	cur=first; 
	while(cur != NULL){
		fprintf(fw, "%s %d\n", cur->name, cur->score); 
		cur = cur->link;
	}

	cur = first;
	while(cur != NULL){
		temp = cur;
		cur = cur->link;
		free(temp);
	}
	fclose(fw);
	return;
}

void newRank(int score){
	char nameTemp[NAMELEN];
	node_pointer temp, cur, prev;
	clear();
	printw("your name: ");
	echo();
	getstr(nameTemp);
	noecho();
	
	len++;
	temp = (node_pointer)malloc(sizeof(Node));
	strcpy(temp->name, nameTemp);
	temp->score = score;
	cur = first;	
	
	if(first==NULL){
		temp->link = NULL;
		first = temp;
	}
	else{	
		while(1){
			if(cur==NULL){
				prev->link = temp;
				break;
			}
			else if(cur->score >= temp->score){
				prev = cur;
				cur = cur->link;
			}
			else{
				if(cur==first){
					temp->link = first;
					first = temp;
					break;
				}
				prev->link = temp;
				temp->link = cur;
				break;
			}
		}		
	}
	return;
}
void DrawRecommend(){
	if(CheckToMove(field, nextBlock[0], recommendR, recommendY, recommendX)==1)
		DrawBlock(recommendY, recommendX, nextBlock[0], recommendR, 'R');
}

void constructRecTree(RecNode *root){
	int i, h;
	RecNode **c=root->c;
	for(i=0;i<CHILDREN_MAX;++i){
		c[i]=(RecNode*)malloc(sizeof(RecNode));
		c[i]->lv=root->lv+1;
		c[i]->f=(char (*)[WIDTH])malloc(sizeof(char)*HEIGHT*WIDTH);
		if(c[i]->lv<VISIBLE_BLOCKS){
			constructRecTree(c[i]);
		}
	}
}

void destructRecTree(RecNode *root){
	int i, h;
	RecNode **c=root->c;
	for(i=0;i<CHILDREN_MAX;i++){
		if(c[i]->lv < VISIBLE_BLOCKS){
			destructRecTree(c[i]);
		}
		free(c[i]->f);
		free(c[i]);
	}
}

int calculScore(int lv, char f[HEIGHT][WIDTH], int r, int y, int x){
	return AddBlockToField(f,nextBlock[lv],r,y,x)+DeleteLine(f);
}

int recommend(RecNode *root){
	int r,x,y,rBoundary, lBoundary;
	int h,w;
	int eval;
	int max=0;//최고점수
	int maxR, maxY, maxX;
	int recommended = 0;
	int i=0;
	int lv=root->lv+1;
	RecNode **c=root->c;
	
	//Boundary 구하기
	for(r=0; r<NUM_OF_ROTATE; ++r){
		lBoundary = 3;
		for(h=0;h<BLOCK_HEIGHT;++h){
			for(w=0;w<BLOCK_WIDTH;++w){
				if(block[nextBlock[lv]][r][h][w]) break;
			}
			if(w<lBoundary){ 
				lBoundary=w;
			}
		}	
		lBoundary=0-lBoundary;
		rBoundary = 0;
		for(h=0;h<BLOCK_HEIGHT;++h){
			for(w=BLOCK_WIDTH-1;w>=0;--w){
				if(block[nextBlock[lv]][r][h][w]) break;
			}
			if(w>rBoundary){
				rBoundary=w;

			}
		}
		rBoundary=WIDTH-1-rBoundary;
		
		for(x=lBoundary;x<=rBoundary;x++,i++){
		//이전 레벨의 field 복원
			for(h=0;h<HEIGHT;h++){
				for(w=0;w<WIDTH;w++){
					c[i]->f[h][w]=root->f[h][w];
				}
			}


			//떨어질 수 있는 y 구하기
			y=0;
			if(CheckToMove(c[i]->f,nextBlock[lv],r,y,x)){
				while(CheckToMove(c[i]->f,nextBlock[lv],r,y+1,x)){
					y++;
				}
			}	
			else continue;

			//점수 계산
			c[i]->score=root->score+calculScore(lv,c[i]->f,r,y,x);
	
			if(lv<VISIBLE_BLOCKS-1)
				eval=recommend(c[i]);
			else
				eval=c[i]->score;
		
			if(max<eval){
				recommended=1;
				max=eval;
				maxR=r;
				maxY=y;
				maxX=x;
			}
		}
	}
	
	if(lv==0 && recommended){
		recommendR=maxR;
		recommendY=maxY;
		recommendX=maxX;
	}
	return max;
}

void recommendedPlay(){
	// user code
}
