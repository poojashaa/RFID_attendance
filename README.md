# RFID Attendance System with Adafruit IO

## Overview

This Arduino project implements an RFID attendance system using an ESP8266 microcontroller, Adafruit IO for MQTT communication, and an RFID reader (MFRC522). The system connects to a Wi-Fi network, reads RFID card IDs, and publishes attendance data to Adafruit IO.

## Components Used

- **ESP8266 (NodeMCU):** Microcontroller for IoT projects.
- **MFRC522 RFID Reader:** Used for reading RFID card IDs.
- **I2C LCD Display:** Provides visual feedback for attendance events.
- **Adafruit IO Account:** Utilized for MQTT communication.

## Dependencies

- [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
- [Adafruit_MQTT](https://github.com/adafruit/Adafruit_MQTT_Library)
- [Wire](https://www.arduino.cc/en/reference/wire)
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
- [SPI](https://www.arduino.cc/en/reference/SPI)
- [MFRC522](https://github.com/miguelbalboa/rfid)

## Configuration

1. Set up your Wi-Fi credentials in the code:

    ```cpp
    #define WLAN_SSID       "YourWiFiSSID"
    #define WLAN_PASS       "YourWiFiPassword"
    ```

2. Configure Adafruit IO credentials:

    ```cpp
    #define AIO_SERVER      "io.adafruit.com"
    #define AIO_SERVERPORT  1883
    #define AIO_USERNAME    "YourAdafruitIOUsername"
    #define AIO_KEY         "YourAdafruitIOKey"
    ```

3. Define RFID tag IDs and associated names:

    ```cpp
    TagIDName tagIDNames[] = {
      {"83 08 09 FF", "bambaraka"},
      {"d32855fe", "bro"},
      {"5ada9f81", "rakshith"},
      {"0a77d681", "harshith"},
      {"830809ff", "pooj"},
      {"fadb4a80", "harry"}
    };
    ```

## Hardware Setup

1. Connect the RFID reader to the specified pins (RST_PIN, SS_PIN).
2. Connect the I2C LCD to the specified address (0x27).

## Usage

1. Upload the code to your ESP8266 using the Arduino IDE.
2. Open the Serial Monitor to view debugging information.
3. Ensure your Adafruit IO dashboard is set up with a feed named "attendance."

## Acknowledgments

- This project uses the [Adafruit IO](https://io.adafruit.com/) platform for MQTT communication.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
