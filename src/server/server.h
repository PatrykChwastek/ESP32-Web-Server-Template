#include "ArduinoJson.h"

void MessageToAll(JsonDocument &doc);

void registerWsCallback(String type, std::function<void(JsonDocument &)> cb);

void setupServer();