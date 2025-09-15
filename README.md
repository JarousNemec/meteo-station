# Meteo Station

The **Meteo Station** project is a DIY weather monitoring system based on the **LilyGO T-Internet-POE** development board.  
It collects local environmental data (wind speed and temperature) and publishes it to an **MQTT broker** (e.g., Mosquitto), which can then be integrated with **Home Assistant** for visualization on a dashboard.

---

## Features

- **ESP32-based board**: LilyGO T-Internet-POE with PoE support (ESP32-WROOM + LAN8720 Ethernet PHY).
- **Wind speed measurement**: Anemometer with a conversion factor of **0.3 m/s per revolution**.
- **Temperature sensing**: External temperature sensor (model can be configured as needed).
- **MQTT communication**: Device connects to Mosquitto broker and publishes sensor readings.
- **Home Assistant integration**: Subscribes to the MQTT topics and displays the values on a dashboard.

---

## Hardware

- LilyGO **T-Internet-POE** (ESP32 + LAN8720 + PoE power).
- **Anemometer** (digital pulse output).
- **Temperature sensor** (e.g., DS18B20, DHT22, or similar).
- Ethernet connection with optional Power over Ethernet.

---

## Software

- **PlatformIO** (ESP32 Dev Module board definition recommended).
- **Arduino framework** (or ESP-IDF, depending on preference).
- **Libraries** (examples):
    - `PubSubClient` or `AsyncMqttClient` for MQTT.
    - `OneWire` / `DallasTemperature` for DS18B20 (if used).
    - `DHT` library for DHT sensors (if used).
    - ESP32 Ethernet (LAN8720) support.
- Code calculates wind speed using this equation:
$$
  v = n \times 0.3 \,\text{m/s}
$$

---

## MQTT Topics

The device publishes data to configurable MQTT topics. Suggested defaults:

- `meteo/wind_speed` → wind speed in meters per second (m/s).
- `meteo/temperature` → ambient temperature in degrees Celsius (°C).

Example payloads:
```json
{
  "wind_speed": 2.4,
  "temperature": 21.7
}
```

## Home Assistant
Example configuration:
```yaml
mqtt:
  sensor:
    - name: "Meteo Station Wind Speed"
      state_topic: "meteo/wind_speed"
      unit_of_measurement: "m/s"
      device_class: wind_speed

    - name: "Meteo Station Temperature"
      state_topic: "meteo/temperature"
      unit_of_measurement: "°C"
      device_class: temperature

```




