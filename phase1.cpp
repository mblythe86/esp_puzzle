#include <stdint.h>
#include <initializer_list>
#include <Arduino.h>

static const int dit_len = 200; //1/5 second?

int letter = 0;
int letter_pos = 0;
int next_transition = 0;

const int alphabet[27][20] PROGMEM = {
/*A*/ {8,  1,0,1,1,1,0,0,0},
/*B*/ {12, 1,1,1,0,1,0,1,0,1,0,0,0},
/*C*/ {14, 1,1,1,0,1,0,1,1,1,0,1,0,0,0},
/*D*/ {10, 1,1,1,0,1,0,1,0,0,0},
/*E*/ {4,  1,0,0,0},
/*F*/ {12, 1,0,1,0,1,1,1,0,1,0,0,0},
/*G*/ {12, 1,1,1,0,1,1,1,0,1,0,0,0},
/*H*/ {10, 1,0,1,0,1,0,1,0,0,0},
/*I*/ {6,  1,0,1,0,0,0},
/*J*/ {16, 1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0},
/*K*/ {12, 1,1,1,0,1,0,1,1,1,0,0,0},
/*L*/ {12, 1,0,1,1,1,0,1,0,1,0,0,0},
/*M*/ {10, 1,1,1,0,1,1,1,0,0,0},
/*N*/ {8,  1,1,1,0,1,0,0,0},
/*O*/ {14, 1,1,1,0,1,1,1,0,1,1,1,0,0,0},
/*P*/ {14, 1,0,1,1,1,0,1,1,1,0,1,0,0,0},
/*Q*/ {16, 1,1,1,0,1,1,1,0,1,0,1,1,1,0,0,0},
/*R*/ {10, 1,0,1,1,1,0,1,0,0,0},
/*S*/ {8,  1,0,1,0,1,0,0,0},
/*T*/ {6,  1,1,1,0,0,0},
/*U*/ {10, 1,0,1,0,1,1,1,0,0,0},
/*V*/ {12, 1,0,1,0,1,0,1,1,1,0,0,0},
/*W*/ {12, 1,0,1,1,1,0,1,1,1,0,0,0},
/*X*/ {14, 1,1,1,0,1,0,1,0,1,1,1,0,0,0},
/*Y*/ {16, 1,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0},
/*Z*/ {14, 1,1,1,0,1,1,1,0,1,0,1,0,0,0},
/*space*/ {7, 0,0,0,0,0,0,0}
};

char* message = "I LOVE PUZZLES\0";

void phase1_setup(){
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
}

void phase1_loop(){
  if((long)(millis() - next_transition) > 0){
    //Serial.println(String("morsestate: ") + letter + "  " + letter_pos + "  " + millis() + "   " + next_transition);
    next_transition = millis() + dit_len;
    letter_pos++;
    int letter_idx = (message[letter] == ' ') ? 26 : (message[letter] - 'A');
    if(letter_pos > alphabet[letter_idx][0]){
      letter++;
      letter_pos = 1;
      if(message[letter] == 0){
        letter = 0;
        letter_pos = 0;
        digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
        next_transition = millis() + dit_len*10;
        return;
      }
      letter_idx = (message[letter] == ' ') ? 26 : (message[letter] - 'A');
    }
    if(alphabet[letter_idx][letter_pos]){
      digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    }
    else{
      digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
}

