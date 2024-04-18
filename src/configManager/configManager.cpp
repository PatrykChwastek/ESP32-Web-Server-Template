#include "SPIFFS.h"

#include "configManager.h"
JsonDocument ReadConfig()
{
    File configFile = SPIFFS.open("/config.json", "r");
    JsonDocument doc;
    deserializeJson(doc, configFile);
    configFile.close();

    return doc;
}
void SaveToConfig(JsonDocument doc)
{
    JsonDocument fullConfig = ReadConfig();
    for (JsonPair kv : doc.as<JsonObject>())
    {
        fullConfig[kv.key().c_str()] = kv.value();
    }

    File configFile = SPIFFS.open("/config.json", "w");
    serializeJson(fullConfig, configFile);
    configFile.close();
}

void SaveToConfig(String VarName, String VarValue)
{
    JsonDocument doc = ReadConfig();
    doc[VarName] = VarValue;

    SaveToConfig(doc);
}