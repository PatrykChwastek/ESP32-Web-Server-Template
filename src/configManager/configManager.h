#include <ArduinoJson.h>

JsonDocument ReadConfig();
void SaveToConfig(JsonDocument doc);
void SaveToConfig(String key, String value);