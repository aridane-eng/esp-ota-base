/****************************************************************************
 * DEFINES
 ****************************************************************************/
#define WIFI_CONFIG_FILE "/wificonfig.json"
#define WIFI_CONFIG_FILE_BACKUP "/wificonfig.json.old"

/****************************************************************************
 * CONFIGURATION HELPERS
 ****************************************************************************/
void init_filesystem() {
  int ret = LittleFS.begin();
  if (!ret) {
    Serial.println("Failed to open filesystem");
    if (!LittleFS.format()) {
      Serial.println("Failed to format filesystem");
    } else {
      ret = LittleFS.begin();
      if (!ret) {
        Serial.println("Failed to open filesystem after succesful format");
      }
    }
  }
}

String load_wifi_config() {
  Serial.println("Loading configuration");
  File f = LittleFS.open(WIFI_CONFIG_FILE, "r");
  if (!f) {
    Serial.println("config file open failed, trying backup");
    f = LittleFS.open(WIFI_CONFIG_FILE_BACKUP, "r");
    if (!f) {
      Serial.println("backup config file open failed as well.");
      return "";
    }
  }
  return f.readString();
}

void save_wifi_config(String s) {
  LittleFS.rename(WIFI_CONFIG_FILE, WIFI_CONFIG_FILE_BACKUP);
  File f = LittleFS.open(WIFI_CONFIG_FILE, "w");
  f.print(s);
  f.close();
  if (LittleFS.exists(WIFI_CONFIG_FILE)) {
    Serial.println("Save succesful");
  } else {
    Serial.println("Save failed!");
  }
}

void save_default_wifi_config() {
  Serial.println("No config found, storing default config");

  DynamicJsonDocument doc(1024);

  doc["ssid"] = "";
  doc["password"] = "";

  String config_json_str;
  serializeJson(doc, config_json_str);
  save_wifi_config(config_json_str);
}

void dump_wifi_config() {
  Serial.print("WIFI SSID       : ");
  Serial.println(wifi_ssid);
  Serial.println();
}

void save_mem_wifi_config_to_disk() {
  DynamicJsonDocument config_json(1024);

  config_json["ssid"] = wifi_ssid;
  config_json["password"] = wifi_password;
  
  String config_json_str;
  serializeJson(config_json, config_json_str);
  save_wifi_config(config_json_str);
}

void load_wifi_config_into_mem() {
  Serial.println("Loading config...");

  String prefs = load_wifi_config();
  if (prefs.length() == 0) {
    Serial.println("No config, saving default config");
    save_default_wifi_config();

    prefs = load_wifi_config();
  }

  // Parse definition (json)
  DynamicJsonDocument wifi_config(1024);
  deserializeJson(wifi_config, prefs);

  wifi_ssid = wifi_config["ssid"].as<String>();
  wifi_password = wifi_config["password"].as<String>();

  Serial.print("Configured WIFI SSID ");
  Serial.println(wifi_ssid);
  dump_wifi_config();
}

void do_config_wifi(String &msg) {
  // Parse definition (json)
  DynamicJsonDocument wifi_config(1024);
  deserializeJson(wifi_config, msg);

  wifi_ssid = wifi_config["ssid"].as<String>();
  wifi_password = wifi_config["password"].as<String>();

  dump_wifi_config();

  save_mem_wifi_config_to_disk();
}
