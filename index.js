const timers = require('timers');

function rand(min, max) {
  return Math.floor(Math.random() * (max - min)) + min;
}

class Coords {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }

  getNear() {
    return {
      up: new Coords(this.x, this.y + 1),
      down: new Coords(this.x, this.y - 1),
      left: new Coords(this.x - 1, this.y),
      right: new Coords(this.x + 1, this.y),
      upleft: new Coords(this.x - 1, this.y + 1),
      upright: new Coords(this.x + 1, this.y + 1),
      downletf: new Coords(this.x - 1, this.y - 1),
      downright: new Coords(this.x + 1, this.y - 1),
    };
  }

  equals(coords) {
    return this.x === coords.x && this.y === coords.y;
  }
}

class Game {
  constructor(size, random) {
    const grid = [];
    const alives = [];
    for (let x = 0; x < size; x += 1) {
      grid[x] = [];
      for (let y = 0; y < size; y += 1) {
        if (random && rand(0, 100) < 30) {
          grid[x][y] = '*';
          alives.push(new Coords(x, y));
        }
        else grid[x][y] = ' ';
      }
    }

    this.grid = grid;
    this.alives = alives;
    this.size = size;
  }

  outOfBounds(coords) {
    const { x, y } = coords;
    if (x < 0 || y < 0) return true;
    if (x >= this.size || y >= this.size) return true;

    return false;
  }

  toAlive(coords) {
    if (this.outOfBounds(coords)) return;

    const { x, y } = coords;
    this.grid[x][y] = '*';
    this.alives.push(coords);

  }

  isAlive(coords) {
    const { x, y } = coords;

    if (this.outOfBounds(coords)) return false;

    return this.grid[x][y] === '*';
  }

  countPop(coords) {
    const near = coords.getNear();
    return Object.keys(near).reduce((pop, key) => this.isAlive(near[key]) ? pop + 1 : pop, 0);
  }

  cellNextState(coords) {
    const { x, y } = coords;
    const pop = this.countPop(coords);
    const isAlive = this.grid[x][y] === '*';

    if (isAlive && (pop === 2 || pop === 3)) {
      return '*';
    } else if (!isAlive && pop === 3) {
      return '*';
    }

    return ' ';
  }

  nextState() {
    const { alives } = this;

    const newGame = new Game(this.size);


    alives.forEach((coords) => {
      const all = {
        coords,
        ...coords.getNear(),
       };

      Object.keys(all).forEach((key) => {
        const here = all[key];
        if (!this.outOfBounds(here)) {
          const hereNext = this.cellNextState(here);
          if (hereNext === '*' && !newGame.isAlive(here)) {
            newGame.toAlive(here);
          }
        }
      });
    });

    return newGame;
  }

  print(start, end) {
    console.clear();
    const lines = this.grid.slice(start.x, end.x);
    lines.forEach((line) => console.log(line.slice(start.y, end.y).join('')));
  }
}

let game = new Game(1000, true);
// const prevStates = [game];

const offset = 200;
const x = 22;
const y = 80;

const start = new Coords(offset, offset);
const end = new Coords(x + offset, y + offset);

timers.setInterval(() => {
  game.print(start, end);
  game = game.nextState();
  // prevStates.push(game);
}, 10);
