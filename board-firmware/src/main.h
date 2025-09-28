#ifndef MAIN_H
#define MAIN_H
// ethernet
static bool eth_connected = false;

#define DEVICE_NAME         "meteo-station"

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN -1          // Pin# of the enable signal for the external crystal oscillator (-1 to disable for internal APLL source)

#define ETH_TYPE ETH_PHY_LAN8720  // Type of the Ethernet PHY (LAN8720 or TLK110)
#define ETH_ADDR 0                // I²C-address of Ethernet PHY (0 or 1 for LAN8720, 31 for TLK110)
#define ETH_MDC_PIN 23            // Pin# of the I²C clock signal for the Ethernet PHY
#define ETH_MDIO_PIN 18           // Pin# of the I²C IO signal for the Ethernet PHY
#define NRST 5


#include <Arduino.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ETH.h>
#include <NimBLEEddystoneTLM.h>


// MQTT Broker
// const char *mqtt_broker = "100.88.1.5";
const char *mqtt_broker = "192.168.1.254";
const char *wind_topic = "meteor/wind";
const char *mqtt_username = "admin";
const char *mqtt_password = "password";
const int mqtt_port = 1883;

//Static Networking
IPAddress local_IP(192, 168, 1, 250); // Static IP Address
IPAddress gateway(192, 168, 1, 1); // Gateway
IPAddress subnet(255, 255, 255, 0); // Subnet Mask
IPAddress primaryDNS(8, 8, 8, 8); // Primary DNS
IPAddress secondaryDNS(8, 8, 4, 4); // Secondary DNS

//Static Networking
// IPAddress local_IP(192, 168, 88, 191); // Static IP Address
// IPAddress gateway(192, 168, 88, 42); // Gateway
// IPAddress subnet(255, 255, 255, 0); // Subnet Mask
// IPAddress primaryDNS(8, 8, 8, 8); // Primary DNS
// IPAddress secondaryDNS(8, 8, 4, 4); // Secondary DNS

WiFiClient espClient;
PubSubClient client(espClient);


#endif //MAIN_H
