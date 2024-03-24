// Super petit scanner WIFI à base d'ESP32-c3 avec résultat sur DumbDisplay sur le smartphone ;-)
// zf240324.1917
//
// Sources:
// https://deepbluembedded.com/esp32-wifi-scanner-example-arduino/
// https://github.com/trevorwslee/Arduino-DumbDisplay/blob/master/samples/esp32c3ddblink/esp32c3ddblink.ino
// https://github.com/trevorwslee/Arduino-DumbDisplay/blob/master/projects/servo/servo.ino
// https://github.com/trevorwslee/Arduino-DumbDisplay
// https://github.com/trevorwslee/Arduino-DumbDisplay/blob/master/projects/ddgpsmap/ddgpsmap.ino


// WIFI library
#include "WiFi.h"


void setup_WIFI() {
    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}


// Redirection de la console
#define CONSOLE(...)                                                                                                   \
  do                                                                                                                   \
  {                                                                                                                    \
    USBSerial.print(__VA_ARGS__);                                                                                         \
    terminal->print(__VA_ARGS__);                                                                                         \
  } while (0)

#define CONSOLELN(...)                                                                                                 \
  do                                                                                                                   \
  {                                                                                                                    \
    USBSerial.println(__VA_ARGS__);                                                                                       \
    terminal->println(__VA_ARGS__);                                                                                       \
  } while (0)

#define CONSOLEF(...)                                                                                                 \
  do                                                                                                                   \
  {                                                                                                                    \
    USBSerial.printf(__VA_ARGS__);                                                                                       \
  } while (0)





// DumbDisplay library
#include "esp32bledumbdisplay.h"
// - use ESP32 BLE with name "ESP32C3"
// - at the same time, enable Serial connection with 115200 baud 
DumbDisplay dumbdisplay(new DDBLESerialIO("WIFI_SCANNER", true, 115200));
TerminalDDLayer* terminal;      // for showing trace of reading data

// PlotterDDLayer *pPlotter;
// LcdDDLayer *lcd;


void setup_DD() {

  terminal = dumbdisplay.createTerminalLayer(1000, 1000);
  terminal->border(5, "blue");
  terminal->padding(5);


  // // create a plotter layer that shows the angle, and for more fun, sin and cos of the angle
  // pPlotter = dumbdisplay.createPlotterLayer(1000, 1000);
  // pPlotter->padding(10);
  // pPlotter->opacity(100);
  // // pPlotter->opacity(25);
  // pPlotter->noBackgroundColor();
  // // pPlotter->backgroundColor("red");
  // pPlotter->label("Sensor multichannel");

  // // create a LCD layers with x rows of y characters
  // lcd = dumbdisplay.createLcdLayer(20, 2);
  //   // set LCD colors and print out something
  // lcd->pixelColor("black");
  // lcd->bgPixelColor("lightgray");
  // lcd->backgroundColor("lightgray");




  // "auto pin" the layers vertically
  dumbdisplay.configAutoPin(DD_AP_VERT);
  } 






void setup() {
    USBSerial.begin(19200);
    delay(3000);  //le temps de passer sur la Serial Monitor ;-)
    CONSOLELN("\n\n\n\nCa commence !\n");
    
    setup_DD();   //configure DumbDisplay

    setup_WIFI();

    CONSOLELN("C'est parti !\n");
}


void loop() {
    CONSOLELN("Scan start");

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    CONSOLELN("Scan done");
    if (n == 0) {
        CONSOLELN("no networks found");
    } else {
        CONSOLE(n);
        CONSOLELN(" networks found");
        CONSOLELN("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            CONSOLEF("%2d",i + 1);
            CONSOLE(" | ");
            CONSOLEF("%-32.32s", WiFi.SSID(i).c_str());
            CONSOLE(" | ");
            CONSOLEF("%4d", WiFi.RSSI(i));
            CONSOLE(" | ");
            CONSOLEF("%2d", WiFi.channel(i));
            CONSOLE(" | ");
            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                CONSOLE("open");
                break;
            case WIFI_AUTH_WEP:
                CONSOLE("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                CONSOLE("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                CONSOLE("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                CONSOLE("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                CONSOLE("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                CONSOLE("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                CONSOLE("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                CONSOLE("WAPI");
                break;
            default:
                CONSOLE("unknown");
            }
            CONSOLELN();
            delay(10);
        }
    }
    CONSOLELN("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    // Wait a bit before scanning again.
    delay(5000);

    // zString = "NO2:" + String(NO2) + "  EtOH:" + String(C2H5OH) + "    "    ;
    // lcd->setCursor(0, 0);
    // lcd->print(zString);
    // zString = "VOC:" + String(VOC) + "  CO:" + String(CO) + "    "          ;
    // lcd->setCursor(0, 1);
    // lcd->print(zString);

    // delay(100);
}

