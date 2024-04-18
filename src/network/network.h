#include <WiFi.h>
#include "ArduinoJson.h"
#include <ESPmDNS.h>

bool ConnectToWifi(JsonDocument config);
void SetupNetwork(JsonDocument config, const char *hostname = "esp-server");