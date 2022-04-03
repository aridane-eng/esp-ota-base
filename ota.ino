/****************************************************************************
 * OVER THE AIR UPDATES (ARDUINO IDE)
 ****************************************************************************/

String getOtaPassword();

void setup_ota() {
  ArduinoOTA.setHostname("esp-ota");
  ArduinoOTA.setPassword(getOtaPassword().c_str());

  ArduinoOTA.onStart([]() {
    Serial.println("OTA start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA end");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]", error);
  });

  ArduinoOTA.begin();
}

void ota_loop() {
  ArduinoOTA.handle();
}