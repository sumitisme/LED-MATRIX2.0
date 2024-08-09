const int DSRow = 1; // subject to change
const int DSCol = 2; // subject to change
const int DClock = 3; // subject to change
const int DLatchR = 4; // subject to change
const int DLatchC = 5; // subject to change

const int DClock2 = 13; // test

const int LU = 6;
const int LD = 7;

const int RU = 8;
const int RD = 9;

bool LeftUp;
bool LeftDown;

bool RightUp;
bool RightDown;


void refreshScreen();
void gameOver();
void showScreen();
void drawpixel(int px, int py, bool p);

struct position {
    float x;
    float y;
};

struct velocity {
    float x;
    float y;
};

byte Screen[8] = {0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000}; // This will be the screen. 64 bits.

class paddle {
public:
    position pos;
    paddle(int a) {
        pos.x = a * 7;
        pos.y = 0;
    }

    void setpaddle() {
        drawpixel(pos.x, pos.y - 1, 1);
        drawpixel(pos.x, pos.y, 1);
        drawpixel(pos.x, pos.y + 1, 1);
    }

    void moveup() {
        pos.y -= 1;
    }

    void movedown() {
        pos.y += 1;
    }
};

class ball {
public:
    position pos; // There are reasons for this
    velocity vel;

    ball() {
        pos.x = 5;
        pos.y = 4;
        vel.x = 0.5;
        vel.y = 0.5;
    }

    void setball() {
        drawpixel(pos.x, pos.y, 1);
    }

    void Move() {
        pos.x += vel.x;
        pos.y += vel.y;
        delay(20);
    }
};

void setup() {
      // put your setup code here, to run once:
      pinMode(DSRow, OUTPUT);
      pinMode(DSCol, OUTPUT);
      pinMode(DClock, OUTPUT);
      pinMode(DLatchR, OUTPUT);
      pinMode(DLatchC, OUTPUT);

      pinMode(DClock2, OUTPUT);

      pinMode(LU, INPUT);
      pinMode(LD, INPUT);
      pinMode(RU, INPUT);
      pinMode(RD, INPUT);

}

void loop() {
    paddle pL(0), pR(1);
    ball b;
    while(true) {
        RightUp = digitalRead(RU);
        LeftUp = digitalRead(LU);

        RightDown = digitalRead(RD);
        LeftDown = digitalRead(LD);
        // Yet to implement player inputs;
        b.Move();
        refreshScreen();     

        if((pL.pos.x == (b.pos.x - 1) && pL.pos.y == b.pos.y) || (pR.pos.x == (b.pos.x + 1) && pR.pos.y == b.pos.y)) {
            b.vel.x *= -1;    
        }

        if((pL.pos.x == (b.pos.x - 1) && pL.pos.y == b.pos.y + 1) || (pR.pos.x == (b.pos.x + 1) && pR.pos.y == b.pos.y + 1)) {
            b.vel.x *= -1;    
        }

        if((pL.pos.x == (b.pos.x - 1) && pL.pos.y == b.pos.y - 1) || (pR.pos.x == (b.pos.x + 1) && pR.pos.y == b.pos.y - 1)) {
            b.vel.x *= -1;    
        }

        if(RightUp == HIGH) {
            if(pR.pos.y > 0) {
                pR.pos.y -= 1;
            }
        }

        if(LeftUp == HIGH) {
            if(pL.pos.y > 0) {
                pL.pos.y -= 1;
            }
        }

        if(RightDown == HIGH) {
            if(pR.pos.y < 7) {
                pR.pos.y += 1;
            }
        }

        if(LeftDown == HIGH) {
            if(pL.pos.y < 7) {
                pL.pos.y += 1;   
            }
        }

        if(b.pos.y == 0 || b.pos.y == 8) {
            b.vel.y *= -1;
        }

        if(b.pos.x == 8) {
            // ToDo : Game over for right
            b.vel.x *= -1;
        }

        if(b.pos.x == -1) {
            // ToDo : Game over for left
            b.vel.x *= -1;
        }

        pL.setpaddle();
        pR.setpaddle();
        b.setball();

        showScreen();
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

      digitalWrite(DLatchC, LOW);
      digitalWrite(DLatchR, LOW);

      shiftOut(DSRow, DClock2, LSBFIRST, RowCycle);
      shiftOut(DSCol, DClock, LSBFIRST, ColCycle);
      //shiftOut(DSCol, DClock, LSBFIRST, ColCycle);
      
      digitalWrite(DLatchC, HIGH);
      digitalWrite(DLatchR, HIGH);
      
      
      RowCycle = RowCycle >> 1;
      
      delay(1); // The simulation is not running at lower values of the delay.
    } 
}

void gameOver() {
  for(int i = 0; i < 8; i++) {
      drawpixel(i, i, 1);
      drawpixel(7 - i, i, 1);
  }
  showScreen();
}
