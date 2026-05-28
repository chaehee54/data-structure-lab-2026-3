#include <deque>
#include "Location2D.h"
using namespace std;


int main() {
	deque<Location2D> locDeque;
	Location2D entry(1, 0);
	locDeque.push_front(entry);

	while (locDeque.empty() == false) {
		Location2D here = locDeque.front();
		locDeque.pop_front();

		int r = here.row, c = here.col;
		printf("(%d,%d)", r, c);
		if (map[r][c] == 'x') {
			printf("미로 탐색 성공\n");
			return 0;
		}

		else {
			map[r][c] = '.';
			if (isValidLoc(r - 1, c)) locDeque.push_front(Location2D(r - 1, c));
			if (isValidLoc(r + 1, c)) locDeque.push_front(Location2D(r + 1, c));
			if (isValidLoc(r, c - 1)) locDeque.push_front(Location2D(r, c - 1));
			if (isValidLoc(r, c + 1)) locDeque.push_front(Location2D(r, c + 1));
		}
	}
	printf("미로 탐색 실패\n");
}