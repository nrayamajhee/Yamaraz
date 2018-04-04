bool matrix[8][4];
void init_matrix(){
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 4; j++) {
      if((i == HOME )|| (i == INVALID))  matrix[i][j] = true;
      else {
        if((ROUND == 1) && ((j % 2) == 1))
          matrix[i][j] = true;
        else if ((ROUND == 2) && ((j % 3) == 0))
          matrix[i][j] = true;
        else
          matrix[i][j] = false;
      }
    }
  }
}
bool is_diag(int turn) {
  if(turn == 2 || turn == 6)
    return false;
   else
    return true;
}
Color find_next_branch(Color from) {
  for (int i = 1; i < 8; i++) {
    if (i != WHITE && i != HOME) {
    for (int j = 0; j < 4; j++) {
        if(matrix[i][j] == false) {
          return i;
        }
      }  
    }
  }
  return HOME;
}
int find_coin_pos(Color col, bool forward) {
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
void rotate(Color from, Color to) {
   int diff = (to - from) * 45;
   if (diff < 0)
      diff = abs(diff) - 180;
    else
      diff = 180 - diff;
     go(LEFT, diff, false);
}
void return_to_color(Color to) {
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
  go_pick(to, false);
}
void return_to_gray(Color from, int steps) {
  go_until_spokes(FRONT, true, 1);
  if(is_diag(from))
    go(FRONT, steps * 8.5, true);
  else
    go(FRONT, steps * 6, true);
  go(FRONT, 7, true);
}
void get_out_of_box(Color color) {
  if(is_diag(color))
      go_const(FRONT, 6, 1000, true);
    else
      go_const(FRONT, 3, 1000, true);
}
void go_pick_next(Color color, bool fromCenter, int spokes) {
  if(fromCenter) {
    go(FRONT, 10, false);
    go(RIGHT, 180, false);
    return_to_gray(color, spokes);
  } else {
    return_to_gray(color, 5 - spokes);
  }
  int dest = find_next_branch(color);
  rotate(color, dest); 
  go_pick(dest, fromCenter);
}
void align_to_coin(Color color, bool fromCenter) {
  delay(200);
    correct_angle();
    delay(200);
    strafe_align();
    if(fromCenter) {
      if(is_diag(color))
        go_const(BACK, 5.5, 2000, true);
      else
        go_const(BACK, 6, 2000, true);
    } else {
      if(is_diag(color))
        go_const(BACK, 7, 2000, false);
      else
        go_const(BACK, 6, 2000, false);
    }
    delay(200);
    strafe_align();
}
void go_next_from_gray(Color from) {
  go_const(BACK, 4, 1000, false);
  drop();
  go_const(FRONT, 4, 1000, false);
  // avoid going home from yellow
  if (from+1 == HOME) {
      go(LEFT, 90, false);
    go_pick(from+2, true);
  } else {
    go(LEFT, 135, false);
    go_pick(from+1, true);
  }
}
void go_pick(Color color, bool fromCenter) {
  int spokes = 0;
  for(int i = 0; i < 4; i++) {
    // if first spoke make sure to get out of the box
     if(spokes == 0) get_out_of_box(color);
     // if there's no spoke to stop by for this branch
    int nextSpoke = find_coin_pos(color, fromCenter);
    if(nextSpoke == -1) {
      go_const(BACK, 6, 1000, true);
      go_pick_next(color, fromCenter, spokes);
      return;
    }
    // otherwise go towards a spoke
    if(fromCenter) {
      go_until_spokes(FRONT, nextSpoke - spokes, true);
      spokes = nextSpoke;
    } else {
      go_until_spokes(FRONT, 5 - nextSpoke - spokes, true); 
      spokes = 5 - nextSpoke;
    }
    align_to_coin(color, fromCenter);
    Color found = pick_up();
    if(found != INVALID) {
      matrix[color][nextSpoke - 1] = true;
      if(fromCenter) {
        go(FRONT, 10, false);
        delay(500);
        go(RIGHT, 180, false);
      }
      return_to_gray(color, nextSpoke);
      // after it is at gray
      if(found == GRAY) {
         go_next_from_gray(color);
        // if not gray
      } else {
        rotate(color, found); 
        return_to_color(found); 
      }
      break;
    } else {
    // if coin not found
    // write code to repeat chekcing...
    
      matrix[color][nextSpoke - 1] = true;
    }
    drop();
    go_const(FRONT, 6, 1000, true);
  }
}
