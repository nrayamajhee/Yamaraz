#define ROUND  1
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
    for(int j = 3; j >= 0; j--) {
      if(matrix[col][j] == false) {
        return j + 1;
      }
    }
  } else {
    for(int j = 0; j < 4; j++) {
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
     if(debug.nav){
      Serial.println ("turning left by");
      Serial.print (from);
      Serial.print ("    ");
      Serial.print ("to ");
      Serial.print ("    ");
      Serial.print (diff);    
     }
}
void return_to_gray(Color from, int steps) {
  if(debug.nav) {
  Serial.println ( " " );
  Serial.print("Going to gray from color = ");
  Serial.print (from);
  Serial.print ("   by     ");
  Serial.print (steps);  
  }
  if(steps == 0) {
    go_const(BACK, 6, 1000, false);
    steps++;
  }
  go_until_spokes(FRONT, true, 1000, 1);
  int spd = LOW_SPD;
  if(steps > 1)  spd = HIGH_SPD;
  if(is_diag(from))
    go(FRONT, steps * 8.5, spd, true);
  else
    go(FRONT, steps * 6, spd, true);
  if(is_diag(from))
      go_const(FRONT, 10, 1000, false);
    else
      go_const(FRONT, 8, 1000, false);
  }

void return_to_color(Color to) {
  if(debug.nav) {
    Serial.println ("returning to ");
    Serial.print (to);  
  }
  if(is_diag(to))
    go(FRONT, 44, HIGH_SPD, true);
  else
    go(FRONT, 30,  HIGH_SPD, true);
  if(is_diag(to))
    go(FRONT, 10, false);
  else
    go(FRONT, 8,  false);
  drop(); 
  delay(500);
  go(RIGHT, 180, false);
  delay(500);
  go_pick(to, false);
}
void go_next_from_gray(Color from) {
  go_const(BACK, 3, 1000, false);
  drop();
  go_const(FRONT, 3, 1000, false);
  int dest = find_next_branch(from);
  rotate(from, dest); 
  go_pick(dest, true);
}
void get_out_of_box(Color color, bool fromCenter) {
  if(!fromCenter) {
    if(is_diag(color))
      go_const(FRONT, 5, 1000, false);
    else
      go_const(FRONT, 3, 1000, false);
  } else {
     if(is_diag(color))
      go_const(FRONT, 8, 1000, false);
    else
      go_const(FRONT, 5, 1000, false);
  }
    strafe_align();
  
}
void go_pick_next(Color color, bool fromCenter, int spokes) {
  if(debug.nav) {
    Serial.println ( "   " );
    Serial.println("turning 180 and going from");
    Serial.print(color);
    Serial.print ("by  ");
  }
  if(fromCenter) {
    go(FRONT, 7, false);
    go(RIGHT, 180, false);
    if(debug.nav)
    Serial.print(spokes);  
    
    return_to_gray(color, spokes);
  } else {
    if(debug.nav)
    Serial.print(5- spokes);  
    
    return_to_gray(color, 4 - spokes);
  }
  if(debug.nav) {
    Serial.println (" ");
    Serial.println("Now in Gray");
  }
  
  int dest = find_next_branch(color);
  rotate(color, dest); 
  if(dest == HOME) {
    correct_front();
    go(FRONT, 43, HIGH_SPD, false);
    exit(0);
  } else go_pick(dest, true);
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
        go_const(BACK, 6.5, 2000, true);
      else
        go_const(BACK, 6, 2000, true);
    }
    delay(200);
    strafe_align();
}
void align_to_coin_const(Color color, bool fromCenter) {
    delay(200);
    correct_angle();
    delay(200);
    strafe_align();
    if(fromCenter) {
      if(is_diag(color))
        go_const(BACK, 1.5, 2000, true);
      else
        go_const(BACK, 2.25, 2000, true);
    } else {
      if(is_diag(color))
        go_const(BACK, 2.75, 2000, false);
      else
        go_const(BACK, 2.25, 2000, false);
    }
    delay(200);
    strafe_align();
}
extern unsigned long start_time;
void go_pick(Color color, bool fromCenter) {
  long time_frame;
  if(ROUND == 1) time_frame = 270000;
  else if(ROUND == 2) time_frame = 330000;
  else if(ROUND == 3) time_frame = 450000;
  if((millis() - start_time) >= time_frame) {
    if(!fromCenter) { 
      get_out_of_box(color, fromCenter);
      return_to_gray(color, 4);
    }
    rotate(color, HOME); 
    correct_front();
    go(FRONT, 41, HIGH_SPD, false);
    correct_right();
    exit(0);
  }
  if(debug.nav) {
    Serial.println ("   ");
    Serial.print("Going ");
    Serial.println(color);  
  }
  int spokes = 0;
  for(int i = 0; i < 4; i++) {
     if(spokes == 0) get_out_of_box(color, fromCenter);
     int nextSpoke = find_coin_pos(color, fromCenter);
    if(debug.nav) {
      Serial.print(" for ");
      Serial.println(nextSpoke);  
    }    
    if(nextSpoke == -1) {
//      Serial.println("Exhausted this color");
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
    Color found = pick_up_validate();
    if (found == INVALID) {
    // if coin not found
    // write code to repeat chekcing...
      drop_gracefully();
      go_const(BACK, 2, 2000, true);
      go_until_spokes(FRONT, 1, 2000, true);
      align_to_coin_const(color, fromCenter);
      found = pick_up_validate();
    }
    if (found == INVALID) found = GRAY;
    matrix[color][nextSpoke - 1] = true;
    if(found != INVALID) {
      if(fromCenter) {
        go(FRONT, 11, 1000, false);
        delay(500);
        go(RIGHT, 180, false);
      }
      return_to_gray(color, nextSpoke);
      // after it is at gray
      if(found == GRAY || found == INVALID) {
         go_next_from_gray(color);
        // if not gray
      } else {
        rotate(color, found); 
        return_to_color(found); 
      }
      break;
    } 
      drop_gracefully();
      go_const(FRONT, 6, 1000, true);
  }
  go_pick_next(color, fromCenter,  spokes );
}
