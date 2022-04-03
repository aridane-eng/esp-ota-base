#include <WiFi.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>

/****************************************************************************
 * DEFINES
 ****************************************************************************/

/****************************************************************************
 * GLOBAL VARIABLES
 ****************************************************************************/
String getSoftApSsid();
String getSoftApPassword();


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
      Serial.print("Connecting to ");
      Serial.println(wifi_ssid);
  
      WiFi.mode(WIFI_STA);
      //WiFi.config(ip, dns, gateway, subnet);
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

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void setup() {
  Serial.begin(115200);

  init_filesystem();
  load_wifi_config_into_mem();
  
  setup_wifi();
  setup_ota();
  setup_http_server();
}

void loop() {
  ota_loop();
}
