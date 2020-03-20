#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
#define GRID_SIZE 1000

void clear() {
  printf("\e[1;1H\e[2J");
}

int randRange(int lower, int upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

typedef struct {
  int x;
  int y;
} Coords;

Coords newCoords(int x, int y) {
  Coords new;
  new.x = x;
  new.y = y;

  return new;
}

void *getNear(Coords coords, Coords **near) {
  int x = coords.x;
  int y = coords.y;

  *near = (Coords *) malloc(sizeof(Coords) * 8);

  *near[0] = newCoords(x, y + 1);
  *near[1] = newCoords(x, y - 1);
  *near[2] = newCoords(x - 1, y);
  *near[3] = newCoords(x + 1, y);
  *near[4] = newCoords(x - 1, y + 1);
  *near[5] = newCoords(x + 1, y + 1);
  *near[6] = newCoords(x - 1, y - 1);
  *near[7] = newCoords(x + 1, y - 1);
}

int OFB(Coords coords) {
  int x = coords.x;
  int y = coords.y;

  if (x < 0 || y < 0) return 1;
  if (x >= GRID_SIZE || y >= GRID_SIZE) return 1;

  return 0;
}

void addToAlives(Coords **alives, int *alivesSize, Coords coords) {
  if (OFB(coords) == 0) {
    *alivesSize += 1;
    if (*alivesSize == 1) {
      *alives = (Coords*) malloc(sizeof(Coords) * (*alivesSize));
    } else {
      *alives = (Coords*) realloc(*alives, sizeof(Coords) * (*alivesSize));
    }
    *alives[(*alivesSize) - 1] = coords;
  }
}

void toAlive(Coords **alives, int *alivesSize, char **state, Coords coords) {
  if (OFB(coords) == 0) {
    addToAlives(alives, alivesSize, coords);
    state[coords.x][coords.y] = '*';
  }
}

int isAlive(char **state, Coords coords) {
  int x = coords.x;
  int y = coords.y;

  if (OFB(coords) == 1) return 0;
  if (state[x][y] == '*') return 1;
  return 0;
}

int countPop(char **state, Coords coords) {
  Coords *near;

  getNear(coords, &near);

  int pop = 0;

  for (int i = 0; i < 8; i += 1) {
    pop += isAlive(state, near[i]);
  }

  return pop;
}

char cellNextState(char **state, Coords coords) {
  int x = coords.x;
  int y = coords.y;
  int pop = countPop(state, coords);
  int alive = isAlive(state, coords);

  if (alive == 1 && (pop == 2 || pop == 3)) return '*';
  if (alive == 0 && pop == 3) return '*';
  return ' ';
}

void nextState(Coords *alives, int alivesSize, char **state, char ***nextState, Coords **nextAlives, int *nextAlivesSize) {
  for (int i = 0; i < alivesSize; i += 1) {
    Coords coords = alives[i];
    Coords *near;

    getNear(coords, &near);

    Coords here = coords;

    for (int j = -1; j < 8; j += 1) {
      if (j > 0) here = near[j];

      char hereNext = cellNextState(state, here);

      if (hereNext == '*' && isAlive(state, here) == 0) {
        toAlive(nextAlives, nextAlivesSize, state, here);
      }
    }
  }
}

void print(char **state, Coords start, Coords end) {
  clear();
  for (int x = start.x; x < end.x; x += 1) {
    for (int y = start.y; y < end.y; y += 1) {
      printf("%c", state[x][y]);
    }
    printf("\n");
  }
}

void empty(char ***state, Coords **alives, int *alivesSize, int random) {
  *state = (char**) malloc(GRID_SIZE * sizeof(char*));
  *alivesSize = 0;

  for (int x = 0; x < GRID_SIZE; x += 1) {
    printf("%d\n", x);
    (*state)[x] = (char*) malloc(GRID_SIZE * sizeof(char));
    printf("%p\n", *state[x]);
    // for (int y = 0; y < GRID_SIZE; y += 1) {
    //   if (random == 1 && randRange(0, 100) < 30) {
    //     toAlive(alives, alivesSize, *state, newCoords(x, y));
    //   } else *state[x][y] = ' ';
    // }
  }
}

int main() {
  int offset = 200;
  int x = 22;
  int y = 80;

  Coords start = newCoords(offset, offset);
  Coords end = newCoords(x + offset, y + offset);

  char **state;
  Coords *alives;
  int alivesSize = 10;

  printf("antes: %p\n", state);
  empty(&state, &alives, &alivesSize, 1);
  printf("depois: %p\n", state);
  // print(state, start, end);


  // while(1) {
  //   print(game, start, end);
  //   // delete_Game(game);
  //   sleep(1);
  // }

  return 0;
}
