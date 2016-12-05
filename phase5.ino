const char phase5_passHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Congrats!</h1><p>You have passed phase 5.</p>"
  "<p>You may <a href=\"/phase6\">continue</a></p></body></html>";

const char phase5_promptHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Phase 5</h1>"
  "<p>Ok, I liked that.  Let's do it again.  Of course, you'll have to find yet another place to enter the password.</p>"
  "<p>Password: ThisIsStillFun!</p>"
  "<p>When you think you're ready, <a href='/phase5'>refresh</a> the page</p>"
  "<p>Need a <a href='/phase5_hint'>hint</a>?</p>"
  "</body></html>";

const char phase5_hintHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<p><b>T</b>his <b>C</b>hallenging <b>P</b>uzzle can be solved, I assure you!</p>"
  "<p>You may need a real computer, and may <b>N</b>eed <b>M</b>ore <b>A</b>dvanced <b>P</b>rograms to find your goal.</p>"
  "<p>Once the goal is found, there are other <b>N</b>i<b>C</b>e programs that are helpful.</p>"
  "<p>Additional hint (select to reveal):<font color='white'>By default, nmap only scans the most used ports.  You may have to use the -p option to scan additional ports</font></p>"
  "<p><a href=\"/phase5\">Back?</a></p></body></html>";

bool phase5_pw_entered = false;

void handle_phase5(){
  if(phase5_pw_entered){
    webServer.send_P(200, "text/html", phase5_passHTML);
    phase5_teardown();
    phase = 6;
    phase6_setup();
  }
  else{
    webServer.send_P(200, "text/html", phase5_promptHTML);
  }
}

void handle_phase5_hint(){
  webServer.send_P(200, "text/html", phase5_hintHTML);
}

WiFiServer real_phase5_server(8266);
WiFiServer *phase5_server = &real_phase5_server;
WiFiClient real_server_client;
WiFiClient *server_client = &real_server_client;

void phase5_setup(){
  webServer.on("/phase5", handle_phase5);
  webServer.on("/phase5_hint", handle_phase5_hint);
  //phase5_server = new WiFiServer(8266);
  phase5_server->begin();
  //server_client = new WiFiClient();
}

void phase5_teardown(){
  server_client->stop();
  //delete server_client;
  phase5_server->stop();
  //delete phase5_server;
}

void phase5_loop(){
  WiFiClient tmp = phase5_server->available();
  if(tmp){
    if(!server_client->connected()){
      server_client->stop();
    }
    *server_client = tmp;
    server_client->println("Enter password:");
  }

  if(server_client->available()){
    String input = server_client->readStringUntil('\n');
    if(input == "ThisIsStillFun!"){
      phase5_pw_entered = true;
      server_client->println("Correct password.");
      server_client->stop();
    }
    else{
      server_client->println("Incorrect password.\nEnter password:");
    }
  }
}

