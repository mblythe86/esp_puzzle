const char phase6HTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Phase 6</h1>"
  "<p>This is it...you've reached the end.  I couldn't think of anything additional to do (or, at least anything that I could reasonably hint at).</p>"
  "<p>Jenna thought I should reward you with a poem, so here goes (with apologies to Poe):</p>"
  "<p>Once upon a midnight dreary, while I pondered, weak and weary,<br>"
  "Over kind of a quaint and curious circuit out of days of yore --<br>"
  "While I debugged, nearly smashing, suddenly there came a flashing,<br>"
  "As of electrons quickly splashing, splashing out the wire to the fore.<br>"
  "\"'Tis just some arcing,\" I muttered; when saw blue smoke, then I swore --<br>"
  "This shit will work never more.<br>"
  "</p>"
  "<p>Hope you have a Happy Holiday!</p>"
  "</body></html>";

void handle_phase6(){
  webServer.send_P(200, "text/html", phase6HTML);
}

void phase6_setup(){
  webServer.on("/phase6", handle_phase6);
}

