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

const char* message = "I LOVE PUZZLES";

const char phase1_passHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Congrats!</h1><p>I hope you do love puzzles!  ;-)</p>"
  "<p>You may <a href=\"/phase2\">continue</a></p></body></html>";

const char phase1_prompt_headHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>";
auto safe_phase1_prompt_headHTML = reinterpret_cast<const __FlashStringHelper *>(phase1_prompt_headHTML);

const char phase1_prompt_tailHTML[] PROGMEM = ""
  "<h1>Hi There!</h1><p>I thought it would be kind of boring to just give you an unprogrammed board, so I've embedded a little game.</p>"
  "<p>There's no reward at the end (aside from personal satisfaction), but to continue you need to enter the password.</p>"
  "<p>Need a <a href='/phase1_hint'>hint</a>?</p>"
  "<form method='GET' action='phase1'>"
    "<input type='text' placeholder='password' name='password'/>"
    "<br /><input type='submit' value='Go!'/></form>"
  "</body></html>";
auto safe_phase1_prompt_tailHTML = reinterpret_cast<const __FlashStringHelper *>(phase1_prompt_tailHTML);

const char phase1_hintHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<p>Hmm...this doesn't bode well.</p>"
  "<p>Hopefully the answer will come in a <b>flash</b> of inspiration...</p>"
  "<p><a href=\"/phase1\">Back?</a></p></body></html>";
  
void handle_phase1(){
  if(webServer.args() > 0){
    //we're trying a password!
    String password = webServer.arg("password");
    password.toUpperCase();
    if(password == message){
      webServer.send_P(200, "text/html", phase1_passHTML);
      digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
      phase = 2;
      phase2_setup();
    }
    else{
      webServer.send(200, "text/html", String(safe_phase1_prompt_headHTML) + "<p>Bad password</p>" + safe_phase1_prompt_tailHTML);
    }
  }
  else{
    webServer.send(200, "text/html", String(safe_phase1_prompt_headHTML) + safe_phase1_prompt_tailHTML);
  }
}

void handle_phase1_hint(){
  webServer.send_P(200, "text/html", phase1_hintHTML);
}

void phase1_setup(){
  webServer.on("/", handle_phase1);
  webServer.on("/phase1", handle_phase1);
  webServer.on("/phase1_hint", handle_phase1_hint);
  webServer.on("/generate_204", handle_phase1);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  webServer.on("/fwlink", handle_phase1);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
}

void phase1_loop(){
  if((long)(millis() - next_transition) >= 0){
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

