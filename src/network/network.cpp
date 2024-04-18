#include "network.h"

bool ConnectToWifi(JsonDocument config) {
    WiFi.begin(config["wifiSsid"].as<String>(), config["wifiPassword"].as<String>());

    int result = WiFi.waitForConnectResult();
    if (result == WL_CONNECTED) {
        Serial.println("WiFi connected, IP address: ");
        Serial.println(WiFi.localIP());
        WiFi.setAutoReconnect(true);
        return true;
    }
    Serial.println("WiFi failed");
    return false;
}

void SetupNetwork(JsonDocument config, const char* hostname) {
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.setHostname(hostname);
    WiFi.softAP(config["apSsid"].as<String>(), config["apPassword"].as<String>());
    ConnectToWifi(config);

    MDNS.begin(hostname); // http://esp-server.local/
}
