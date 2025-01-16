#include "tetris.h"

void process_input(int c) {
  if (c == 'r') {
    userInput(Start, false);
  } else if (c == 'p') {
    userInput(Pause, false);
  } else if (c == 'q') {
    userInput(Terminate, false);
  } else if (c == KEY_LEFT) {
    userInput(Left, false);
  } else if (c == KEY_RIGHT) {
    userInput(Right, false);
  } else if (c == KEY_UP) {
    userInput(Up, false);
  } else if (c == KEY_DOWN) {
    userInput(Down, false);
  } else if (c == ' ') {
    userInput(Action, false);
  }
}

Game *get_current_game_info() {
  static Game game_info_ex = {0};
  return &game_info_ex;
}

void move_figure(Game *game) {
  if (game->new_input) {
    if (game->action == Terminate) {
      game->game_info.pause = 3;
      finish_game(game);
    } else if (game->action == Pause) {
      game->state = PAUSE;
      game->game_info.pause = 1;
    } else if (game->action == Left)
      move_right_or_left(game, -1);
    else if (game->action == Right)
      move_right_or_left(game, 1);
    else if (game->action == Down)
      move_to_bottom(game);
    else if ((game->action == Action || game->action == Up))
      process_rotation(game);
  }
  if (timer(game, game->game_info.speed) && game->game_info.pause != 1)
    game->state = SHIFTING;
  game->new_input = 0;
}

void pause_state(Game *game) {
  if (game->new_input) {
    if (game->action == Pause) {
      game->state = MOVING;
      game->game_info.pause = 0;
    } else if (game->action == Terminate) {
      game->game_info.pause = 3;
      finish_game(game);
    }
  }
  game->new_input = 0;
}

void state_machine(Game *game) {
  if (game->state == START) {
    if (game->new_input) {
      if (game->action == Start) {
        game->game_info.pause = 0;
        init_game(game);
      } else if (game->action == Terminate) {
        game->game_info.pause = 3;
      }
    }
    game->new_input = 0;
  }
  if (game->state == SPAWN) {
    create_new_falling(game);
  } else if (game->state == SHIFTING) {
    move_block_down(game);
  } else if (game->state == MOVING) {
    move_figure(game);
  } else if (game->state == GAME_OVER) {
    game->game_info.pause = 2;
    game->state = START;
  } else if (game->state == PAUSE) {
    pause_state(game);
  }
}

int **create_matrix(int rows, int columns) {
  int **matrix = (int **)calloc(rows, sizeof(int *));
  for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)calloc(columns, sizeof(int));
  }
  return matrix;
}

void free_matrix(int **matrix, int rows) {
  for (int i = 0; i < rows; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

void finish_game(Game *game) {
  if (game->game_info.score > game->game_info.high_score) {
    FILE *file = fopen("high_score.txt", "w");
    if (file) {
      fprintf(file, "%d", game->game_info.score);
      fclose(file);
    }
  }
  free_matrix(game->game_info.field, HEIGHT);
  free_matrix(game->game_info.next, 4);
  game->game_info.field = NULL;
  free(game->next);
  free(game->current);
  free(game->blocks);
  game->is_playing = 0;
}