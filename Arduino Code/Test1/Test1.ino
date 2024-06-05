const int DSRow = 1; // subject to change
const int DSCol = 2; // subject to change
const int DClock = 3; // subject to change
const int DLatchR = 4; // subject to change
const int DLatchC = 5; // subject to change

#define BORDERS 0 // playground is from 0 to 7
#define BORDERE 7

byte Screen[8] = {0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000}; // This will be the screen. 64 bits.

void setup() {
  // put your setup code here, to run once:
  pinMode(DSRow, OUTPUT);
  pinMode(DSCol, OUTPUT);
  pinMode(DClock, OUTPUT);
  pinMode(DLatchR, OUTPUT);
  pinMode(DLatchC, OUTPUT);
}

void loop() {
  int x = 0, y = 0;
  int velx = 1;
  int vely = 0;

  while(true) {
    if(x < BORDERS || x > BORDERE) {
      velx *= -1;
    }
    
    if(y < BORDERS || y > BORDERE) {
      vely *= -1;
    }
    refreshScreen();
    drawpixel(x, y, 1);
    showScreen();
    x += velx;
    y += vely;
  }
//  gameOver();
}

void refreshScreen() {
  for(int i = 0; i < 8; i++) {
    Screen[i] = 0x00;
  }
  return;
}

void drawpixel(int px, int py, bool p) {
  // pixel drawing.
  byte Mask;

  if (p == 1) {
    Mask = 0b10000000;
    Mask = Mask >> px;
    Screen[py] |= Mask;
  }
  else {
    Mask = 0b01111111;
    Mask = Mask >> px;
    Screen[py] &= Mask;  
  } 
  return;
}

void showScreen() {
  byte RowCycle = 0b10000000; // 8 bits
  byte ColCycle;
  
  for(int i = 0; i < 8; i++) {
      ColCycle = Screen[i];

      digitalWrite(DLatchR, LOW);
      shiftOut(DSRow, DClock, LSBFIRST, RowCycle);
      digitalWrite(DLatchR, HIGH);

      digitalWrite(DLatchC, LOW);
      shiftOut(DSCol, DClock, LSBFIRST, ColCycle);
      digitalWrite(DLatchC, HIGH);
    
      RowCycle = RowCycle >> 1;
      delay(20); // The simulation is not running at lower values of the delay.
    } 
}

void gameOver() {
  for(int i = 0; i < 8; i++) {
      drawpixel(i, i, 1);
      drawpixel(7 - i, i, 1);
  }
  showScreen();
}

void drawLine(int ax, int ay, int bx, int by, bool p) {
  
}
