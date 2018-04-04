extern bool matrix[8][4];

bool is_diag(int turn) {
  if(turn == 2 || turn == 6)
    return false;
   else
    return true;
}
void return_to_gray(Color from, int steps) {
  go_until_spokes(FRONT, true, 1);
  if(is_diag(from))
    go(FRONT, steps * 8.5, true);
  else
    go(FRONT, steps * 6, true);
  go(FRONT, 6, true);
  int dir = 180 - (from * 45);
  if (dir < 0)
    go(LEFT, abs(dir), false);
  else
    go(RIGHT, abs(dir), false);
  correct_front();
}
int check_matrix(Color col, bool forward) {
  if(forward) {
    for(int j = 0; j < 4; j++) {
      if(matrix[col][j] == false) {
        return j + 1;
      }
    }
  } else {
    for(int j = 3; j >= 0; j--) {
      if(matrix[col][j] == false) {
        return j + 1;
      }
    }  
  }
  return -1;
}
Color check_matrix_next(Color from) {
  for (int i = 1; i < 8; i++) {
    if (i != HOME) {
    for (int j = 0; j < 4; j++) {
        if(matrix[i][j] == false) {
          return i;
        }
      }  
    }
  }
  return HOME;
}
void go_to(Color color) {
  if(color == HOME) {
    go(RIGHT, 180, false);
    go(FRONT, 43, false);
    return;
  }
  int spokes = 0;
  int currentSpoke = check_matrix(color, true);
  if(currentSpoke == -1) {
    go_to(check_matrix_next(color));
    return;
  }
  go(RIGHT, color * 45, false);
  go_const(FRONT, 6, 1000, true);
  for(int i = 0; i < 4; i++) {
    currentSpoke = check_matrix(color, true);
    if(currentSpoke == -1) {
      go_to(check_matrix_next(color));
      break;
    }
    go_until_spokes(FRONT, true, currentSpoke - spokes);
    spokes++;
    delay(200);
    correct_angle();
    delay(200);
    strafe_align();
    if(is_diag(color))
      go_const(BACK, 5.5, 2000, true);
    else
      go_const(BACK, 6, 2000, true);
    delay(200);
    strafe_align();
    // first pickup
    Color found = pick_up();
    if(found == INVALID) {
//      go_const(FRONT, 6, 2000, true);
//      delay(500);
//      correct_angle();
//      strafe_align();
//      correct_angle();
//      delay(500);
//      go_const(BACK, 6, 2000, false);
//      // second pickup
//      found = pick_up();
      matrix[color][currentSpoke - 1] = true;
    }
    if(found != INVALID) {
      matrix[color][currentSpoke - 1] = true;
      go(FRONT, 10, false);
      delay(500);
      go(RIGHT, 180, false);
      return_to_gray(color, currentSpoke);
      if(found == GRAY) {
        go_const(BACK, 4, 1000, false);
        drop();
        go_const(FRONT, 4, 1000, false);
        correct_front();
        go_to(color+1);
      } else {
        return_to(found); 
      }
      break;
    } 
    drop();
    go_const(FRONT, 4, 1000, true);
  }
//  go(FRONT, 10, false);
//  delay(500);
//  go(RIGHT, 180, false);
//  return_to_gray(color, spokes);
//  go_to(color+1);
}
void go_to_gray(Color from) {
  int spokes = 0;
  if(is_diag(from))
    go_const(FRONT, 5, 1000, true);
  else
    go_const(FRONT, 2, 1000, true);
  for(int i = 0; i < 4; i++) {
    int currentSpoke = check_matrix(from, false);
    if(currentSpoke == -1) {
      return_to_gray(from, 4);
      go_to(check_matrix_next(from));
      break;
    }
    go_until_spokes(FRONT, true, 5 - currentSpoke - spokes);
    spokes = 5 - currentSpoke;
    delay(200);
    correct_angle();
    delay(200);
    strafe_align();
    if(is_diag(from))
      go_const(BACK, 7, 2000, false);
    else
      go_const(BACK, 6, 2000, false);
    delay(200);
    strafe_align();
    Color found = pick_up();
    if(found == INVALID) {
//      go_const(FRONT, 6, 2000, false);
//      delay(500);
//      correct_angle();
//      strafe_align();
//      correct_angle();
//      delay(500);
//      go_const(BACK, 6, 2000, false);
//      // second pickup
//      found = pick_up();
      matrix[from][currentSpoke - 1] = true;
    }
    if(found != INVALID) {
      matrix[from][currentSpoke - 1] = true;
      return_to_gray(from, currentSpoke);
      if(found == GRAY) {
        go_const(BACK, 4, 1000, false);
        drop();
        go_const(FRONT, 4, 1000, false);
        correct_front();
        go_to(from+1);
      } else {
        return_to(found); 
      }
      break;
    }
    drop();
    go_const(FRONT, 6, 1000, true);
  }
//  return_to_gray(from, spokes);
//  go_to(from+1);
}
void return_to(Color to) {
  go(RIGHT, to * 45, false );
  if(is_diag(to))
    go(FRONT, 44, true);
  else
    go(FRONT, 30, true);
  if(is_diag(to))
    go(FRONT, 9, false);
  else
    go(FRONT, 7, false);
  drop(); 
  delay(500);
  go(RIGHT, 180, false);
  go_to_gray(to);
}
