bool is_diag(int turn) {
  if(turn == 2 || turn == 6)
    return false;
   else
    return true;
}
void go_to(Color color) {
  int spokes = 0;
  go(RIGHT, color * 45, false);
  go(BACK, 8, true);
  for(int i = 0; i < 4; i++) {
    go_until_spokes(BACK, true, 1);
    if(is_diag(color)) {
      go_const(BACK, 8, 2000, true);
      strafe_align();
      go_const(FRONT, 1.5, 2000, true);
      strafe_align();
    } else {
      go_const(BACK, 6, 2000, true);
      strafe_align();
      go_const(FRONT, 0.5, 2000, true);
      strafe_align();
    }
    spokes++;
    pick_up();
    Color found = calculate_color();
    if(found != INVALID) {
      go(BACK, 2, false);
      go(RIGHT, 180, false);
      return_to(found, spokes, color);
      break;
    }
    go_const(FRONT, 3, 2000, true);
  }
}
void return_to(Color to, int steps, Color from) {
  go_until_spokes(BACK, true, 1);
  if(is_diag(from))
    go(BACK, steps * 8.5, true);
  else
    go(BACK, steps * 6, true);
  go(BACK, 6, true);
  int dir = 180 - (from * 45);
  if (dir < 0)
    go(LEFT, abs(dir), false);
  else
    go(RIGHT, abs(dir), false);
  correct_front();
  go(RIGHT, to * 45, false );
  if(is_diag(to))
    go(BACK, 56, true);
  else
    go(BACK, 40, true);
  go(RIGHT, 180, false);
  drop(); 
  go_to_gray(to);
}
void go_to_gray(Color from) {
  int spokes = 0;
  go(BACK, 5, true);
  for(int i = 0; i < 4; i++) {
    go_until_spokes(BACK, true, 1);
    go_const(BACK, 6, 2000, true);
    strafe_align();
    if(is_diag(from))
      go_const(FRONT, 2, 2000, true);
    strafe_align();
    spokes++;
    pick_up();
    Color found = calculate_color();
    if(!is_diag(from))
      go_const(FRONT, 3, 2000, true);
    if(found != INVALID) {
      go_const(FRONT, 3, 2000, true);
      return_to(found, (4 - spokes), from);
      break;
    }
    go_const(FRONT, 3, 2000, true);
  }
}
void run_periphery() {
  for(int i = 0; i < 4; i++) {
    go(BACK, 60, true);
    delay(1000);
    go(RIGHT, 90, false);
  }
}
