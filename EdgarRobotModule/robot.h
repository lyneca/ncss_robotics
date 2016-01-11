enum dir {
  up,
  down,
  left,
  right, 
  halt
};

enum face {
  happy,
  lookleft,
  lookright,
  sad,
  boop,
  lookup,
  lookdown,
  weird1,
  weird2,
};
void showArrow(dir arrow);
void writeGrid(byte grid[8]);
