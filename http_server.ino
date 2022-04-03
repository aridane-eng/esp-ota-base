/****************************************************************************
 * HTTP SERVER
 ****************************************************************************/

void setup_http_server() {
  http_server.on("/", handle_root);
  http_server.on("/info", handle_info);
  http_server.on("/config/wifi", HTTP_POST, handle_post_wifi);

  http_server.begin();
}

void handle_root(AsyncWebServerRequest *request) {
  request->send(200, "text/html", "<html><body>Nothing to see here</body></html>");
}

void handle_info(AsyncWebServerRequest *request) {
  DynamicJsonDocument doc(1024);

  char info[1024];
  snprintf(info, sizeof(info), "Build %s - %s", __DATE__, __TIME__);
  doc["build"] = String(info);

  String info_json_str;
  serializeJson(doc, info_json_str);

  request->send(200, "application/json", info_json_str);
}

void handle_post_wifi(AsyncWebServerRequest *request) {
  Serial.println("POST /config/wifi");
  
  if (request->hasParam("body", true)) {
    String message = request->getParam("body", true)->value();
    do_config_wifi(message);
    request->send(200, "text/plain", "");
  } else {
    request->send(403, "text/plain", "Invalid wifi config");
  }
}
