const char phase3_promptHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Phase 3</h1>"
  "<p>This one's a free-pass, assuming you've recognized a pattern.</p>"
  "<p>Need a <a href='/phase3_hint'>hint</a>?</p>"
  "</body></html>";

const char phase3_hintHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<p>This one will make more sense if you've been using a real web browser instead of a \"sign into this network\" pop-up.</p>"
  "<p><a href=\"/phase3\">Back?</a></p></body></html>";

const char phase4_passHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Congrats!</h1><p>You have passed phase 4.</p>"
  "<p>You may <a href=\"/phase5\">continue</a></p></body></html>";

const char phase4_promptHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Phase 4</h1>"
  "<p>Let's flip it around this time.  I'll give you the password, and you'll figure out where to enter it.</p>"
  "<p>Password: ThisIsFun!</p>"
  "<p>When you think you're ready, <a href='/phase4'>refresh</a> the page</p>"
  "<p>Need a <a href='/phase4_hint'>hint</a>?</p>"
  "</body></html>";

const char phase4_hintHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<p>I hope you're enjoying solving all of these <b>serial</b> puzzles!</p>"
  "<p>If not, I suppose I should <b>console</b> you.</p>"
  "<p>If so, don't worry, this isn't the <b>terminal</b> puzzle yet.</p>"
  "<p><a href=\"/phase4\">Back?</a></p></body></html>";

void handle_phase3(){
  webServer.send_P(200, "text/html", phase3_promptHTML);
}

void handle_phase3_hint(){
  webServer.send_P(200, "text/html", phase3_hintHTML);
}

bool phase4_pw_entered = false;

void handle_phase4(){
  if(phase4_pw_entered){
    webServer.send_P(200, "text/html", phase4_passHTML);
    //FIXME: prep phase 5
  }
  else{
    if(phase == 3){
      phase = 4;
      phase4_setup();
    }
    webServer.send_P(200, "text/html", phase4_promptHTML);
  }
}

void handle_phase4_hint(){
  webServer.send_P(200, "text/html", phase4_hintHTML);
}

void phase3_setup(){
  webServer.on("/phase3", handle_phase3);
  webServer.on("/phase3_hint", handle_phase3_hint);
  webServer.on("/phase4", handle_phase4);
}

void phase3_loop(){
  //nothing to do
}

void phase4_setup(){
  webServer.on("/phase4_hint", handle_phase4_hint);
}

int next_message = 0;

void phase4_loop(){
  //check serial
  if(Serial.available()){
    String input = Serial.readString();
    if(input == "ThisIsFun!"){
      phase4_pw_entered = true;
      Serial.println("Correct password.");
    }
    else{
      Serial.println("Incorrect password.\nEnter password:");
    }
  }
  
  if((long)(millis() - next_message) >= 0 && !phase4_pw_entered){
    next_message = millis() + 5000;
    Serial.println("Enter password:");
  }
}

