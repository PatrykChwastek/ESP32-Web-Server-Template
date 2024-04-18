#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <map>

#include "server.h"
#include "configManager/configManager.h"

std::map<String, std::function<void(JsonDocument&)>> messageCallbacks;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void registerWsCallback(String type, std::function<void(JsonDocument&)> cb) {
    messageCallbacks[type] = cb;
}

void MessageToAll(JsonDocument& doc) {
    String message;
    serializeJson(doc, message);
    ws.textAll(message);
}

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
    case WS_EVT_DATA: {
        std::unique_ptr<AwsFrameInfo> info(static_cast<AwsFrameInfo*>(arg));
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            std::string message(reinterpret_cast<char*>(data), len);
            JsonDocument doc;
            deserializeJson(doc, message);
            String type = doc["type"];

            if (messageCallbacks.count(type))
                messageCallbacks[type](doc);
        }
    }
    case WS_EVT_CONNECT:
        break;
    case WS_EVT_DISCONNECT:
        break;
    case WS_EVT_PONG:
        break;
    case WS_EVT_ERROR:
        break;
    }
}

void notFound(AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Not found");
}

void onConfigMessage(JsonDocument& doc) {
    SaveToConfig(doc);
    MessageToAll(doc);
}

void setupServer() {
    registerWsCallback("config", onConfigMessage);

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request)
        { request->send(SPIFFS, "/index.html", "text/html", false); });

    server.on("/network", HTTP_GET, [](AsyncWebServerRequest* request)
        { request->send(SPIFFS, "/pages/network.html", "text/html", false); });

    server.on("/network", HTTP_POST, [](AsyncWebServerRequest* request) {
        String ssid = request->getParam("ssid")->value();
        String password = request->getParam("password", true)->value();
        JsonDocument jsonConfig;

        jsonConfig["wifiSsid"] = ssid;
        jsonConfig["wifiPassword"] = password;

        SaveToConfig(jsonConfig);
        ESP.restart();
        });

    server.on("/network.js", HTTP_GET, [](AsyncWebServerRequest* request)
        { request->send(SPIFFS, "/pages/network.js", "text/javascript"); });

    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest* request)
        { request->send(SPIFFS, "/index.css", "text/css"); });

    server.on("/scripts/socket.js", HTTP_GET, [](AsyncWebServerRequest* request)
        { request->send(SPIFFS, "/scripts/socket.js", "text/javascript"); });

    server.on("/scripts/main.js", HTTP_GET, [](AsyncWebServerRequest* request)
        { request->send(SPIFFS, "/scripts/main.js", "text/javascript"); });

    server.onNotFound(notFound);
    server.begin();
}