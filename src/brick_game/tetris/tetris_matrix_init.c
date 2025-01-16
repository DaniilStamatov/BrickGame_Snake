#include "tetris.h"

TetrominoMap *init_all_block_types() {
  TetrominoMap *blocks = malloc(7 * sizeof(TetrominoMap));
  for (int i = 0; i < 7; i++) {
    init_new_map_block(&blocks[i], i);
  }
  return blocks;
}

void init_new_map_block(TetrominoMap *blocks, int type) {
  if (type == I) {
    init_block_I(blocks);
  } else if (type == L) {
    init_block_L(blocks);
  } else if (type == J) {
    init_block_J(blocks);
  } else if (type == O) {
    init_block_O(blocks);
  } else if (type == S) {
    init_block_S(blocks);
  } else if (type == T) {
    init_block_T(blocks);
  } else if (type == Z) {
    init_block_Z(blocks);
  }
}

void init_block_I(TetrominoMap *tetris) {
  position states[4][4] = {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
                           {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
                           {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
                           {{0, 1}, {1, 1}, {2, 1}, {3, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_L(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
                           {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
                           {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
                           {{0, 0}, {0, 1}, {1, 1}, {2, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_J(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
                           {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
                           {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                           {{0, 1}, {1, 1}, {2, 0}, {2, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_O(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                           {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                           {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                           {{0, 1}, {0, 2}, {1, 1}, {1, 2}}};
  fill_block_matrix(tetris, states);
}

void init_block_S(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
                           {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                           {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
                           {{0, 0}, {1, 0}, {1, 1}, {2, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_T(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
                           {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
                           {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
                           {{0, 1}, {1, 0}, {1, 1}, {2, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_Z(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
                           {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
                           {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                           {{0, 1}, {1, 0}, {1, 1}, {2, 0}}};
  fill_block_matrix(tetris, states);
}
