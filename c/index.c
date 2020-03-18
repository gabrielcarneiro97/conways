#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

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

Coords new_Coords(int x, int y) {
  Coords new;
  new.x = x;
  new.y = y;

  return new;
}

Coords *Coords_getNear(Coords coords, Coords *arr) {
  int x = coords.x;
  int y = coords.y;

  arr = (Coords *) malloc(sizeof(Coords) * 8);


  arr[0] = new_Coords(x, y + 1);
  arr[1] = new_Coords(x, y - 1);
  arr[2] = new_Coords(x - 1, y);
  arr[3] = new_Coords(x + 1, y);
  arr[4] = new_Coords(x - 1, y + 1);
  arr[5] = new_Coords(x + 1, y + 1);
  arr[6] = new_Coords(x - 1, y - 1);
  arr[7] = new_Coords(x + 1, y - 1);

  return arr;
}

typedef struct {
  char **grid;
  Coords *alives;
  int alives_size;
  int size;
} Game;

int Game_outOfBounds(Game game, Coords coords) {
  int x = coords.x;
  int y = coords.y;

  if (x < 0 || y < 0) return 1;
  if (x >= game.size || y >= game.size) return 1;

  return 0;
}

Game Game_addToAlives(Game game, Coords coords) {
  if (Game_outOfBounds(game, coords) == 0) {
    game.alives_size += 1;
    if (game.alives_size == 1) {
      game.alives = (Coords*) malloc(sizeof(Coords) * game.alives_size);
    } else {
      game.alives = (Coords*) realloc(game.alives, sizeof(Coords) * game.alives_size);
    }
    game.alives[game.alives_size - 1] = coords;
  }
  return game;
}

Game Game_toAlive(Game game, Coords coords) {
  if (Game_outOfBounds(game, coords) == 0) {
    game = Game_addToAlives(game, coords);
    game.grid[coords.x][coords.y] = '*';
  }

  return game;
}

int Game_isAlive(Game game, Coords coords) {
  int x = coords.x;
  int y = coords.y;

  if (Game_outOfBounds(game, coords) == 1) return 0;
  if (game.grid[x][y] == '*') return 1;
  return 0;
}

int Game_countPop(Game game, Coords coords) {
  Coords *near;

  near = Coords_getNear(coords, near);

  int pop = 0;

  for (int i = 0; i < 8; i += 1) {
    pop += Game_isAlive(game, near[i]);
  }

  return pop;
}

char Game_cellNextState(Game game, Coords coords) {
  int x = coords.x;
  int y = coords.y;
  int pop = Game_countPop(game, coords);
  int isAlive = Game_isAlive(game, coords);

  if (isAlive == 1 && (pop == 2 || pop == 3)) return '*';
  if (isAlive == 0 && pop == 3) return '*';
  return ' ';
}

Game new_Game(int size, int random) {
  Game game;
  game.grid = (char**) malloc(size * sizeof(char*));
  game.size = size;
  game.alives_size = 0;

  for (int x = 0; x < size; x += 1) {
    game.grid[x] = (char*) malloc(size * sizeof(char));
    for (int y = 0; y < size; y += 1) {
      if (random == 1 && randRange(0, 100) < 30) {
        game = Game_toAlive(game, new_Coords(x, y));
      } else game.grid[x][y] = ' ';
    }
  }

  return game;
}

void delete_Game(Game game) {
  free(game.grid);
  free(game.alives);
}

Game Game_nextState(Game game) {
  Coords *alives = game.alives;
  int alives_size = game.alives_size;

  Game newGame = new_Game(game.size, 0);

  for (int i = 0; i < alives_size; i += 1) {
    Coords coords = alives[i];
    Coords *near;

    near = Coords_getNear(coords, near);

    Coords here = coords;

    for (int j = -1; j < 8; j += 1) {
      if (j > 0) here = near[j];

      char hereNext = Game_cellNextState(game, here);

      if (hereNext == '*' && Game_isAlive(game, here) == 0) {
        newGame = Game_toAlive(newGame, here);
      }
    }
  }

  return newGame;
}

void Game_print(Game game, Coords start, Coords end) {
  clear();
  for (int x = start.x; x < end.x; x += 1) {
    for (int y = start.y; y < end.y; y += 1) {
      printf("%c", game.grid[x][y]);
    }
    printf("\n");
  }
}

int main() {
  int offset = 200;
  int x = 22;
  int y = 80;

  Coords start = new_Coords(offset, offset);
  Coords end = new_Coords(x + offset, y + offset);

  Game game = new_Game(1000, 1);

  while(1) {
    Game_print(game, start, end);
    // delete_Game(game);
    game = Game_nextState(game);
    sleep(1);
  }
}
