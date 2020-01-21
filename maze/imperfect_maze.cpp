#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct room {
	short int right, down;
	int set;
};
room **maze;
int width, height;

void Eller() {
	int h, w, i, j;
	int start, end;
	int set1, set2;
	int flag, cnt_room;
	srand(time(NULL));

	for (i = 0; i < height - 1; i++) {
		//�� ���� �� ����
		for (j = 0; j < width - 1;j++) {
			if (maze[i][j].set != maze[i][j + 1].set) {
				flag = rand() % 2;
				if (flag == 0) {
					maze[i][j].right = 0;
					if (maze[i][j + 1].set > maze[i][j].set) {
						set1 = maze[i][j].set;
						set2 = maze[i][j + 1].set;
					}
					else {
						set1 = maze[i][j + 1].set;
						set2 = maze[i][j].set;
					}
					for (h = 0;h <= i;h++) {
						for (w = 0;w < width;w++)
							if (maze[h][w].set == set2)maze[h][w].set = set1;
					}
				}
			}
		}

		//���� �ٷ� ��� �����
		start = 0;
		while (start < width) {
			for (j = start; j < width; j++) {
				if (maze[i][start].set == maze[i][j].set)
					end = j;
				else break;
			}
			//���ո��� ���� ���� ��� ����� �� �ϳ��� ���Ƿ� ����
			cnt_room = start + (rand() % (end - start + 1));
			maze[i][cnt_room].down = 0;
			maze[i + 1][cnt_room].set = maze[i][cnt_room].set;

			//�������� ������ ����� ������� ����
			for (j = start; j <= end; j++) {
				if (j < width && maze[i][j].down == 1 && maze[i][j + 1].down == 1) {
					flag = rand() % 2;
					if (flag == 0) {
						maze[i][j].down = 0;
						maze[i + 1][j].set = maze[i][j].set;
					}
				}
			}
			start = end + 1;
		}
	}

	//������ �� �� ����
	for (j = 0; j < width - 1; j++) {
		if (maze[height - 1][j].set != maze[height - 1][j + 1].set) {
			maze[height - 1][j].right = 0;
			if (maze[height - 1][j + 1].set > maze[height - 1][j].set) {
				set1 = maze[height - 1][j].set;
				set2 = maze[height - 1][j + 1].set;
			}
			else {
				set1 = maze[height - 1][j + 1].set;
				set2 = maze[height - 1][j].set;
			}

			for (h = 0;h < height; h++) {
				for (w = 0;w < width;w++)
					if (maze[h][w].set == set2) maze[h][w].set = set1;
			}
		}

	}

	return;
}

void deleteWall(){
	int dWall = ((width < height) ? width : height) / 2;
	int rand_w, rand_h, rand_dir, count = 0;
	int i, j;
	
	while (count < dWall) {
		rand_w = rand() % width;
		rand_h = rand() % height;
		rand_dir = rand() % 2;

		if (rand_dir == 0) {
			if (rand_w != width - 1 && maze[rand_w][rand_h].right) {
				maze[rand_w][rand_h].right = 0;
				count++;
			}
		}

		else {
			if (rand_h != height - 1 && maze[rand_w][rand_h].down) {
				maze[rand_w][rand_h].down = 0;
				count++;
			}
		}
	}

}

int main()
{
	int i, j;
	FILE* fw;
	printf("Width: ");
	scanf("%d", &width);
	printf("Height: ");
	scanf("%d", &height);

	//�̷� �޸��Ҵ�
	maze = (room**)malloc(sizeof(room*)*height);
	for (i = 0;i < height;i++)
		maze[i] = (room*)malloc(sizeof(room)*width);

	//�� �ʱ�ȭ
	int count = 1;
	for (i = 0;i < height;i++) {
		for (j = 0;j < width;j++) {
			maze[i][j].right = 1;
			maze[i][j].down = 1;
			maze[i][j].set = count;
			count++;
		}
	}

	//���� �̷� ����
	Eller();

	//���� ������ �ҿ��� �̷� ����
	deleteWall();

	//�̷� ���
	fw = fopen("imperfect_maze.maz", "w");

	fprintf(fw, "+");
	for (i = 0;i < width;i++) {
		fprintf(fw, "-+");
	}
	fprintf(fw, "\n");
	for (i = 0; i < height; i++) {
		fprintf(fw, "|");
		for (j = 0; j < width; j++) {
			fprintf(fw, " ");
			if (maze[i][j].right == 1) fprintf(fw, "|");
			else fprintf(fw, " ");
		}
		fprintf(fw, "\n+");

		for (j = 0; j < width; j++) {
			if (maze[i][j].down == 1) fprintf(fw, "-");
			else fprintf(fw, " ");
			fprintf(fw, "+");
		}
		fprintf(fw, "\n");
	}

	fclose(fw);
	return 0;
}
