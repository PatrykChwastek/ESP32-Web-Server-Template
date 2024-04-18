## ESP32 Web Server Template
This is a starting point for ESP32 projects in PlatformIO.
Tested on ` ESP32 Wroom DevKit v1`

### Features
- Async Web Server
- WebSockets for two ways communication
- WiFi and AP mode
- mDNS default: `http://esp-server.local`
- JSON support
- Save configuration JSON file
- Examples for on board LED and button

### Dependencies
- ArduinoJson
- AsyncTCP
- ESPAsyncWebServer
- Wifi
- ESPmDNS
- SPIFFS

### Usage
- (optional) Set your WiFI credentials in `data/config.json` or to it on network to in ap mode `http://esp-server.local/network`
- Build and upload filesystem image
- Build and upload program to ESP