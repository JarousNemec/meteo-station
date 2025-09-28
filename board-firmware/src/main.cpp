#include <main.h>

// Callback for Ethernet events
void resetEspClient() {
    espClient.stop(); // force close socket
    delay(100);
}

void wiFiEvent(const WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_ETH_START:
            Serial.println("ETH Started");
            ETH.setHostname(DEVICE_NAME);
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Serial.println("ETH Connected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Serial.print("ETH IP: ");
            Serial.println(ETH.localIP());
            Serial.print("Duplex: ");
            Serial.print(ETH.fullDuplex() ? "FULL" : "HALF");
            Serial.print(", ");
            Serial.print(ETH.linkSpeed());
            Serial.println(" Mbps");
            eth_connected = true;
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Serial.println("ETH Disconnected");
            eth_connected = false;
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Serial.println("ETH Stopped");
            eth_connected = false;
            break;
        default:
            break;
    }
}

void publishDiscovery() {
    String configTopic = "homeassistant/sensor/meteo_station_wind/config";
    const char *discoveryPayload = R"rawliteral({
  "name": "Meteo Station Wind",
  "state_topic": "meteor/wind",
  "unit_of_measurement": "m/s",
  "device_class": "wind_speed",
  "unique_id": "meteo_station_wind",
  "device": {
    "identifiers": ["meteo_station_001"],
    "name": "Meteo Station",
    "model": "WT32-ETH01",
    "manufacturer": "DIY"
  }
})rawliteral";

    bool ok = client.publish(configTopic.c_str(), discoveryPayload, true);
    Serial.print("Discovery published? ");
    Serial.println(ok ? "YES" : "NO");
}

bool checkMqttConnectedAndReconnect() {
    if (!client.connected()) {
        resetEspClient();
        client.setClient(espClient);
        client.setServer(mqtt_broker, mqtt_port);
        client.setBufferSize(512);
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());

        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
            delay(100);
            publishDiscovery();
            return true;
        }
        Serial.print("failed with state ");
        Serial.println(client.state());
        delay(2000);
        return false;
    }
    return true;
}

void publishWindSpeed() {
    const float tempC = random(100, 1001) / 100.0;
    // check whether the reading is successful or not
    if (isnan(tempC)) {
        client.publish(wind_topic, "-1");
    } else {
        const String payload = String(tempC, 1);
        client.publish(wind_topic, payload.c_str());
    }
}


void setupNetwork() {
    Serial.println("\nStarting WT32-ETH01 Ethernet...");

    WiFi.onEvent(wiFiEvent);
    // Initialize Ethernet with built-in LAN8720 PHY pins
    // Initialise the Ethernet interface...
    pinMode(NRST, OUTPUT);
    digitalWrite(NRST, 0);
    delay(200);
    digitalWrite(NRST, 1);
    delay(200);
    digitalWrite(NRST, 0);
    delay(200);
    digitalWrite(NRST, 1);
    ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);

    ETH.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
    delay(100);



    Serial.println("Waiting for network...");
}

void setup() {
    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(14, OUTPUT);

    Serial.begin(115200);
    delay(500);

    setupNetwork();
    delay(2000);
}

unsigned long lastMqttPublish = 0;
unsigned long mqttPublishInterval = 5000;

unsigned long lastEmergencyBehave = 0;
unsigned long emergencyBehaveInterval = 5000;

unsigned long lastEthSerialInfo = 0;
unsigned long ethSerialInfoInterval = 1000;

unsigned short failedMqttAttempts = 0;

void emergencyBehavior() {
    Serial.println("Starting emergency procedure...");
    delay(500);
    Serial.println("Finishing emergency procedure");
}

void loop() {
    if (millis() - lastEthSerialInfo > ethSerialInfoInterval) {
        if (eth_connected) {
            Serial.println("Ethernet OK");
        } else {
            Serial.println("Ethernet K.O.");
        }
        lastEthSerialInfo = millis();
    }
    if (eth_connected) {
        if (checkMqttConnectedAndReconnect()) {
            if (millis() - lastMqttPublish > mqttPublishInterval) {
                publishWindSpeed();
                lastMqttPublish = millis();
                Serial.println("Meteo published");
            }

            client.loop();
            failedMqttAttempts = 0;
        } else {
            if (failedMqttAttempts > 5 && millis() - lastEmergencyBehave > emergencyBehaveInterval) {
                emergencyBehavior();
                lastEmergencyBehave = millis();
                Serial.println("Emergency loop");
            }
            failedMqttAttempts++;
        }
    }
}
