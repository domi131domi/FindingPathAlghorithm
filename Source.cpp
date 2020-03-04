#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

struct Vertex {
	int x;
	int y;
	int cost[4] = { -1, -1, -1, -1 };	//0 lewo 1 prawo 2 gora 3 dol
	int h = 0;
	int f = 0;
	int g = 0;
	int from_X = -1;
	int from_Y = -1;
};

int findLeastF(std::vector<Vertex*>& open) {
	int leastF = open[0]->f;
	int where = 0;
	for (int i = 0; i < open.size(); i++) {
		if (leastF > open[i]->f) {
			leastF = open[i]->f;
			where = i;
		}
	}
	return where;
}

bool vertexIsIn(std::vector<Vertex*>& closed, int x, int y) {
	for (Vertex* ver : closed) {
		if (ver->x == x && ver->y == y)
			return true;
	}
	return false;
}

int calcHeuristic(int max, int from_x, int from_y) {
	return  from_x +(from_y - max + 1);
}

const int max = 7;


void showPath(int tab[max][max]) {

	Vertex mapa[max][max];
	for (int i = 0; i < max; i++)	//0 lewo 1 prawo 2 gora 3 dol
		for (int e = 0; e < max; e++) {
			if (e - 1 >= 0)
				mapa[i][e].cost[0] = tab[i][e - 1];
			if (e + 1 < max)
				mapa[i][e].cost[1] = tab[i][e + 1];
			if (i - 1 >= 0)
				mapa[i][e].cost[2] = tab[i - 1][e];
			if (i + 1 < max)
				mapa[i][e].cost[3] = tab[i + 1][e];
			mapa[i][e].x = e;
			mapa[i][e].y = i;
		}
	std::vector<Vertex*> open;
	std::vector<Vertex*> closed;
	open.push_back(&mapa[0][max - 1]);

	while (open.size() != 0) {
		int where = findLeastF(open);
		Vertex* q = open[where];
		if (q->x == 0 && q->y == max - 1) {
			break;
		}
		closed.push_back(q);
		open.erase(open.begin() + where);

		int neighbor_x, neighbor_y;

		for (int i = 0; i < 4; i++) {
			if (q->cost[i] != -1) {		//0 lewo 1 prawo 2 gora 3 dol
				neighbor_x = q->x;
				neighbor_y = q->y;

				if (i == 0)
					neighbor_x--;
				else if (i == 1)
					neighbor_x++;
				else if (i == 2)
					neighbor_y--;
				else if (i == 3)
					neighbor_y++;

				if (!vertexIsIn(closed, neighbor_x, neighbor_y)) {
					int temp = q->g + q->cost[i];
					bool to_change = false;
					if (!vertexIsIn(open, neighbor_x, neighbor_y)) {
						open.push_back(&mapa[neighbor_y][neighbor_x]);
						mapa[neighbor_y][neighbor_x].h = calcHeuristic(max, neighbor_x, neighbor_y);
						to_change = true;
					}
					else if (temp < mapa[neighbor_y][neighbor_x].g)
						to_change = true;

					if (to_change) {
						mapa[neighbor_y][neighbor_x].from_X = q->x;
						mapa[neighbor_y][neighbor_x].from_Y = q->y;
						mapa[neighbor_y][neighbor_x].g = temp;
						mapa[neighbor_y][neighbor_x].f = mapa[neighbor_y][neighbor_x].g + mapa[neighbor_y][neighbor_x].h;
					}
				}

			}
		}
	}

	char result[max][max];
	for (int i = 0; i < max; i++)
		for (int e = 0; e < max; e++)
			result[i][e] = ' ';


	int path_x = 0;
	int path_y = max - 1;
	while (!(path_x == max - 1 && path_y == 0)) {
		int temp = path_x;
		path_x = mapa[path_y][path_x].from_X;
		path_y = mapa[path_y][temp].from_Y;
		result[path_y][path_x] = '1';
	}
	result[max - 1][0] = '0';
	result[0][max - 1] = '0';
	for (int i = 0; i < max; i++) {
		for (int e = 0; e < max; e++)
			std::cout << result[i][e];
		std::cout << std::endl;
	}
}

int main() {
	/*int tab[max][max] ={  {6,1,1,0},
						  {6,1,1,1},
						  {5,6,6,1},
						  {0,1,1,1}
	};*/

	/*int tab[max][max] = { {1,1,1,1,0},
						  {1,20,20,20,20},
						  {1,1,1,1,1},
						  {20,9,20,20,1},
						  {0,1,1,1,1}
	};*/

	/*int tab[max][max] = { 
	{3,3,3,3,2,1,0},
	{3,3,3,1,1,1,2},
	{6,6,5,1,4,9,9},
	{9,9,9,1,1,1,1},
	{7,7,7,8,4,2,1},
	{4,4,5,4,4,2,1},
	{0,1,1,1,1,1,1}
	};*/

	/*int tab[max][max] = {
	{1,1,1,1,1,1,0},
	{20,20,20,20,20,20,1},
	{1,1,1,1,1,1,1},
	{1,20,20,20,20,20,20},
	{1,1,1,1,1,1,1},
	{20,20,20,20,20,20,1},
	{0,1,1,1,1,1,1}
	};*/

	std::ifstream file;
	file.open("map.txt");
	int tab[max][max];
	char letter;
	for (int i = 0; i < max; i++) {
		for (int e = 0; e < max; e++) {
			file >> letter;
			tab[i][e] = int(letter - '0');
		}
	}
	file.close();
	showPath(tab);
	
	system("pause");

}