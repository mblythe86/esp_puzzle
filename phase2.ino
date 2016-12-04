const char phase2_passHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Congrats!</h1><p>I think you </p>"
  "<p>You may <a href=\"/phase3\">continue</a></p></body></html>";

const char phase2_prompt_part1HTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<p><font color='";
const char phase2_prompt_part2HTML[] PROGMEM = ""
  "'>25</font></p>"
  "<p><font color='";
const char phase2_prompt_part3HTML[] PROGMEM = ""
  "'>36</font></p>"
  "<p><font color='";
const char phase2_prompt_part4HTML[] PROGMEM = ""
  "'>70</font></p>"
  "<p><font color='";
const char phase2_prompt_part5HTML[] PROGMEM = ""
  "'>81</font></p>"
  "<p>When you think you're ready, <a href='/phase2'>refresh</a> the page</p>"
  "<p>Need a <a href='/phase2_hint'>hint</a>?</p>"
  "</body></html>";

auto safe_phase2_prompt_part1HTML = reinterpret_cast<const __FlashStringHelper *>(phase2_prompt_part1HTML);
auto safe_phase2_prompt_part2HTML = reinterpret_cast<const __FlashStringHelper *>(phase2_prompt_part2HTML);
auto safe_phase2_prompt_part3HTML = reinterpret_cast<const __FlashStringHelper *>(phase2_prompt_part3HTML);
auto safe_phase2_prompt_part4HTML = reinterpret_cast<const __FlashStringHelper *>(phase2_prompt_part4HTML);
auto safe_phase2_prompt_part5HTML = reinterpret_cast<const __FlashStringHelper *>(phase2_prompt_part5HTML);

const char phase2_hintHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<p>I was hoping to <b>connect</b> with you through this gift.</p>"
  "<p><a href=\"/phase2\">Back?</a></p></body></html>";
  
bool all_connections_made = false;
bool conn[4] = {false, false, false, false};

void handle_phase2(){
  if(all_connections_made){
    webServer.send_P(200, "text/html", phase2_passHTML);
  }
  else{
    webServer.send(200, "text/html", String(safe_phase2_prompt_part1HTML)
                                     + (conn[0]?"green":"red")
                                     + safe_phase2_prompt_part2HTML
                                     + (conn[1]?"green":"red")
                                     + safe_phase2_prompt_part3HTML
                                     + (conn[2]?"green":"red")
                                     + safe_phase2_prompt_part4HTML
                                     + (conn[3]?"green":"red")
                                     + safe_phase2_prompt_part5HTML
                                     );
  }
}

void handle_phase2_hint(){
  webServer.send_P(200, "text/html", phase2_hintHTML);
}

std::array<int,9> ports{ {D0, D1, D2, D3, D4, D5, D6, D7, D8} };
std::array<std::pair<int,int>,17> matching_ports;
int current_port = 0;
bool current_state = false;
int next_check = 0;

void phase2_setup(){
  matching_ports[D0] = std::make_pair(D7,0);
  matching_ports[D1] = std::make_pair(D8,3);
  matching_ports[D2] = std::make_pair(D5,2);
  matching_ports[D3] = std::make_pair(D6,1);
  matching_ports[D4] = std::make_pair(D4,-1);
  matching_ports[D5] = std::make_pair(D2,2);
  matching_ports[D6] = std::make_pair(D3,1);
  matching_ports[D7] = std::make_pair(D0,0);
  matching_ports[D8] = std::make_pair(D1,3);
  webServer.on("/phase2", handle_phase2);
  webServer.on("/phase2_hint", handle_phase2_hint);
  for(int port: ports){
    pinMode(port, INPUT_PULLUP);
  }
  //these pins have external pull-downs
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
}

void phase2_loop(){
  if((long)(millis() - next_check) >= 0){
    for(int port: ports){
      if(port == ports[current_port]){
        //do nothing
      }
      else if(!(digitalRead(port) ^ current_state)){ //pins match
        if(ports[current_port] == matching_ports[port].first){
          conn[matching_ports[port].second] = true;
        }
      }
      else{ //pins mismatch
        if(ports[current_port] == matching_ports[port].first){
          conn[matching_ports[port].second] = false;
        }
      }
    }
    if(!current_state){
      digitalWrite(ports[current_port], HIGH);
      current_state = true;
    }
    else{
      if(ports[current_port] != D3 && ports[current_port] != D4){
        pinMode(ports[current_port], INPUT_PULLUP);
      }
      else{
        pinMode(ports[current_port], INPUT);
      }
      current_port = (current_port +1)%9;
      current_state = false;
      pinMode(ports[current_port], OUTPUT);
      digitalWrite(ports[current_port], LOW);
    }
    all_connections_made = conn[0] && conn[1] && conn[2] && conn[3];
    Serial.println(String("all_conn:") + all_connections_made);
    next_check = millis()+1;
  }
}

