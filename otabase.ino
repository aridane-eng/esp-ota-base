#ifdef ESP8266
#include <ESP8266WiFi.h>            // ESP Board Support Package
#else
#include <WiFi.h>                   // ESP Board Support Package
#endif
#include <LittleFS.h>               // ESP Board Support Package
#include <ArduinoJson.h>            // ArduinoJson by Benoit Blachon: 6.19.3
#ifdef ESP8266
#include <ESPAsyncTCP.h>            // https://github.com/me-no-dev/ESPAsyncTCP
#else
#include <AsyncTCP.h>               // https://github.com/me-no-dev/AsyncTCP
#endif
#include <ESPAsyncWebServer.h>      // https://github.com/me-no-dev/ESPAsyncWebServer
#include <ArduinoOTA.h>             // ESP Board Support Package

/****************************************************************************
 * FORWARD DECLARATIONS
 ****************************************************************************/
String getSoftApSsid();
String getSoftApPassword();

/****************************************************************************
 * GLOBAL VARIABLES
 ****************************************************************************/
boolean soft_ap_mode_active = false;

String wifi_ssid;
String wifi_password;

WiFiClient wifiClient;
AsyncWebServer http_server(80);

IPAddress local_IP(192,168,1,30);
IPAddress gateway(192,168,1,30);
IPAddress subnet(255,255,255,0);

void setup_wifi() {
  Serial.print("ESP Board MAC Address: ");
  String macAddress = WiFi.macAddress();
  Serial.println(macAddress);
  Serial.println();

  boolean wifi_ready = false;
  boolean wifi_setup_failed = false;
  do {
    if (wifi_ssid.length() == 0 || wifi_setup_failed) {
      Serial.print("Setting up access point with SSID ");
      Serial.println(getSoftApSsid());
  
      Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "AP config set" : "AP config failed!");
      Serial.println(WiFi.softAP(getSoftApSsid().c_str(), getSoftApPassword().c_str()) ? "AP started" : "AP failed to start");
      Serial.println("SoftAP IP Address is 192.168.1.30");
      soft_ap_mode_active = true;
      wifi_setup_failed = false;
    } else {
      // We start by connecting to a WiFi network
      Serial.printf("Connecting to %s\n", wifi_ssid);
  
      WiFi.mode(WIFI_STA);
      WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  
      int try_count = 0;
      while (WiFi.status() != WL_CONNECTED && try_count < 120) {
        delay(1000);
        Serial.print(".");
      }
  
      if (try_count == 120) {
        wifi_setup_failed = true;
      } else {
        wifi_setup_failed = false;
      }
  
      soft_ap_mode_active = false;
    }
  } while (wifi_setup_failed);

  Serial.printf("\nWifi connected.\nIP Address: %s\n", WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  init_filesystem();
  load_wifi_config_into_mem();
  
  setup_wifi();
  setup_ota();
  setup_http_server();

  Serial.println("OTA ready.");
}

void loop() {
  ota_loop();
}
