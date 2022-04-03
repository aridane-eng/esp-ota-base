# ESP32 Arduino Over-The-Air base

This is a starting point for adding OTA support to your ESP32 projects. 

After starting, no configuration is available and the ESP will open up an access point which can be used to configure WIFI.

```shell
curl -X POST -d '{ "ssid": "your SSID here", "password": "your password here" }' http://192.168.1.30/config/wifi
```

Once this is done, reset the board and it will present itself on the WIFI network as an OTA device in Arduino IDE.

Configuration is saved on LittleFS in the file /wificonfig.json.

## Usage

Install the dependencies before using this (See below).

Then, copy the credentials.ino.example file to credentials.ino and edit the fields:
- getOtaPassword should return the password you want to use for OTA updates. You will have to enter this password when using the OTA update functionality in Arduino IDE.
- getSoftApSsid should return the SSID of the WIFI network that will be created when no configuration is present (first boot).
- getSoftApPassword should return the password of the WIFI network that will be created when no configuration is present (first boot).


## Dependencies

Use the [ESP32 board definitions](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)

### ESP32
Install the following libraries before building:

- ArduinoJson
- https://github.com/me-no-dev/AsyncTCP
- https://github.com/me-no-dev/ESPAsyncWebServer

### ESP8266
Install the following libraries before building:

- ArduinoJson
- https://github.com/me-no-dev/ESPAsyncTCP

- https://github.com/me-no-dev/ESPAsyncWebServer
