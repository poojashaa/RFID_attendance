#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// WiFi parameters
#define WLAN_SSID       "helix24"
#define WLAN_PASS       "19651965"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "Sriharry"
#define AIO_KEY         "aio_HBir15qorfDKRKlgbRj25YHF6b3q"

// RFID
#define RST_PIN         D3
#define SS_PIN          D4
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 columns and 2 rows

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish Attendance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/attendance");

// Create a structure to hold RFID tag IDs and names
struct TagIDName {
  const char* tagID;
  const char* name;
};

// Define your RFID tag IDs and associated names
TagIDName tagIDNames[] = {
  {"83 08 09 FF", "bambaraka"},
  {"d32855fe", "bro"},{"5ada9f81","rakshith"},{"0a77d681","harshith"},{"830809ff","pooj"},{"fadb4a80","harry"}

};

// Function to get the name associated with an RFID tag ID
const char* getNameFromTagID(const char* tagID) {
  for (size_t i = 0; i < sizeof(tagIDNames) / sizeof(tagIDNames[0]); ++i) {
    if (strcmp(tagID, tagIDNames[i].tagID) == 0) {
      return tagIDNames[i].name;
    }
  }
  return "Unknown";
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Failed to connect to WiFi"));
    while (1) {
      delay(1000);
    }
  }

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // Increase the delay before connecting to Adafruit IO
  delay(5000);

  // Connect to Adafruit IO
  connect();

  // Initialize RFID reader
  SPI.begin();
  mfrc522.PCD_Init();

  // Initialize LCD display
  lcd.init();
  // turn on the backlight
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.begin(16, 2);
  lcd.print("RFID Attendance");
}

// Connect to Adafruit IO via MQTT
void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavailable")); break;
      case 4: Serial.println(F("wrong password")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);
  }
  Serial.println(F("Adafrui Connected!"));
}

void loop() {
  // Ping Adafruit IO a few times to make sure we remain connected
  if (!mqtt.ping(3)) {
    // Reconnect to Adafruit IO
    if (!mqtt.connected())
      connect();
  }

  // Check for RFID card
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String cardID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      cardID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    Serial.print(F("RFID Card ID: "));
    Serial.println(cardID);

    const char* cardName = getNameFromTagID(cardID.c_str());

    // Publish RFID card name to Adafruit IO
    if (!Attendance.publish(cardName)) {
      Serial.println(F("Failed to publish RFID data"));
    } else {
      Serial.println(F("RFID Data Sent!"));
    }

    // Display "Attendance Taken" on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Attendance Taken");
    delay(2000); // Display for 2 seconds

    // Display "Place Tag" on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place Tag");

    // Halt until the card is removed to avoid continuous readings
    delay(3000);

    Serial.println(F("Waiting for the next RFID card..."));
  }
}
