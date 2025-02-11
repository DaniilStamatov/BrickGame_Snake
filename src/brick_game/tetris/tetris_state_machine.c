#include "tetris.h"
void move_figure(Game *game, float dt) {
  if (game->new_input) {
    if (game->action == Terminate) {
      game->game_info.pause = 3;
      finish_game(game);
    } else if (game->action == Pause) {
      game->current_state = GAME_PAUSED;
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
  game->time += dt;
  if(game->time > game->game_info.speed && 
      game->game_info.pause != 1) {
    game->current_state = SHIFTING;
    game->time = 0;
  }
  game->new_input = 0;
}

void set_pause_state(Game *game) {
  if (game->new_input) {
    if (game->action == Pause) {
      game->current_state = MOVING;
      game->game_info.pause = 0;
    } else if (game->action == Terminate) {
      game->game_info.pause = 3;
      finish_game(game);
    }
  }
  game->new_input = 0;
}

void process_state_machine(Game *game, float dt) {
  if (game->current_state == START) {
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
  if (game->current_state == SPAWN) {
    create_new_falling(game);
  } else if (game->current_state == SHIFTING) {
    move_block_down(game);
  } else if (game->current_state == MOVING) {
    move_figure(game, dt);
  } else if (game->current_state == GAME_LOST) {
    game->game_info.pause = 2;
    game->current_state = START;
  } else if (game->current_state == GAME_PAUSED) {
    set_pause_state(game);
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
  game->next = NULL;
  free(game->current);
  free(game->blocks);
}