#include "ArduinoJson.h"
#include "SPIFFS.h"

#include "network/network.h"
#include "server/server.h"
#include "configManager/configManager.h"

const int BTN_PIN = 0;
const int LED_PIN = 2;

bool BTN_STATE = false;

bool handleButtonState() {
  static bool prevBtnState = false;
  bool currBtnState = digitalRead(BTN_PIN) == LOW;

  if (currBtnState && !prevBtnState) {
    BTN_STATE = !BTN_STATE;

    JsonDocument doc;
    doc["STATE"] = BTN_STATE;

    MessageToAll(doc);
  }

  prevBtnState = currBtnState;
  return BTN_STATE;
}

void ledStatusMessage(bool enabled) {
  static bool prevLedState = false;

  if (enabled) {
    bool ledState = !digitalRead(LED_PIN);

    if (ledState != prevLedState) {
      JsonDocument doc;
      doc["type"] = "ledStatus";
      doc["status"] = ledState;

      MessageToAll(doc);

      prevLedState = ledState;
    }
  }
}

void ledBlink(bool enabled) {
  static uint32_t previousMillis;
  const long interval = 1000;

  if (enabled) {
    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      bool ledState = !digitalRead(LED_PIN);
      digitalWrite(LED_PIN, ledState);
    }
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }

  ledStatusMessage(enabled);
}

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();

  SPIFFS.begin(true);

  JsonDocument config = ReadConfig();
  SetupNetwork(config);

  setupServer();
}

void loop()
{
  ledBlink(BTN_STATE);
  handleButtonState();
}
