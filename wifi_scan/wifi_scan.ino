// Super petit scanner WIFI à base d'ESP32-c3 super mini avec résultat sur DumbDisplay sur le smartphone ;-)
#define zVERSION        "zf240615.1812"
//
// ATTENTION, ce code a été testé sur un esp32-c3 super mini. Pas testé sur les autres boards !
//
// ATTENTION, tant qu'il n'y a pas eu de connexion à DumbDisplay, c'est bloqué en attente de connexion BLE !
//
// Sources:
// https://deepbluembedded.com/esp32-wifi-scanner-example-arduino/
// https://github.com/trevorwslee/Arduino-DumbDisplay
// https://github.com/trevorwslee/Arduino-DumbDisplay/blob/master/samples/esp32c3ddblink/esp32c3ddblink.ino
// https://github.com/trevorwslee/Arduino-DumbDisplay/blob/master/projects/ddgpsmap/ddgpsmap.ino


// General pour AI-C3 DUAL USBC esp32-c3 grand modèle avec LED RGB !
// const int ledPin = 20;    // the number of the LED pin, c'est une LED RGB sur ce modèle
const int ledPin = 8;             // the number of the LED pin
const int buttonPin = 9;  // the number of the pushbutton pin

const int zSonarPulseOn = 50;    // délai pour sonarPulse
const int zSonarPulseOff = 100;    // délai pour sonarPulse
const int zSonarPulseWait = 500;    // délai pour sonarPulse

// // configuration du NeoPixel (LED RGB) pour ce modèle !
// #define RGB_BRIGHTNESS 10     // LED RGB brightness (max 255)
// #define RGB_BUILTIN 8         // Pin de la led RGB !

// configuration de la console 'commune' (ATTENTION au saut de ligne '\' !)
#define CONSOLEFLN(...)                 \
  do                                    \
  {                                     \
    char zString[40];                   \
    sprintf(zString,__VA_ARGS__);       \
    USBSerial.println(zString);         \
    terminal->println(zString);         \
  } while (0)


// WIFI library
#include "WiFi.h"

void setup_WIFI() {
    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}


// DumbDisplay library
#include "esp32bledumbdisplay.h"
// - use ESP32 BLE with name "WIFI_SCANNER"
// - at the same time, enable Serial connection with 115200 baud 
DumbDisplay dumbdisplay(new DDBLESerialIO("WIFI_SCANNER", true, 115200));
TerminalDDLayer* terminal;      // for showing trace of reading data

void setup_DD() {
  USBSerial.println("En attente de connexion à DumbDisplay...");

  terminal = dumbdisplay.createTerminalLayer(1000, 1000);
  terminal->border(5, "blue");
  terminal->padding(5);
  dumbdisplay.configAutoPin(DD_AP_VERT);
  } 


void setup() {
  // Pulse deux fois pour dire que l'on démarre
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); delay(zSonarPulseOn); digitalWrite(ledPin, HIGH); delay(zSonarPulseOff);
  digitalWrite(ledPin, LOW); delay(zSonarPulseOn); digitalWrite(ledPin, HIGH); delay(zSonarPulseOff);
  delay(zSonarPulseWait);

  // Start serial console
  USBSerial.begin(19200);
  USBSerial.setDebugOutput(true);       //pour voir les messages de debug des libs sur la console série !
  delay(3000);                          //le temps de passer sur la Serial Monitor ;-)
  USBSerial.println("\n\n\n\n**************************************\nCa commence !"); USBSerial.println(zVERSION);

  setup_DD();     //configure DumbDisplay

  setup_WIFI();   //configure WIFI

  CONSOLEFLN("C'est parti !");
  CONSOLEFLN("");
  digitalWrite(ledPin, LOW);
  delay(500);
}


void loop() {
    CONSOLEFLN("Scan start");
    digitalWrite(ledPin, HIGH);
    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    CONSOLEFLN("Scan done");
    digitalWrite(ledPin, LOW);

    if (n == 0) {
        CONSOLEFLN("no networks found");
    } else {
        CONSOLEFLN("%d networks found", n);
        CONSOLEFLN("Nr, SSID, RSSI, CH");
        for (int i = 0; i < n; ++i) {
            CONSOLEFLN("%d, %s, %d, %d", i+1, WiFi.SSID(i), WiFi.RSSI(i), WiFi.channel(i));
        }
    }
    CONSOLEFLN("");
    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();
}

