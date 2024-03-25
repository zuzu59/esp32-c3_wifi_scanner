// Super petit scanner WIFI à base d'ESP32-c3 avec résultat sur DumbDisplay sur le smartphone ;-)
// zf240325.1412
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



// DumbDisplay library
#include "esp32bledumbdisplay.h"
// - use ESP32 BLE with name "ESP32C3"
// - at the same time, enable Serial connection with 115200 baud 
DumbDisplay dumbdisplay(new DDBLESerialIO("WIFI_SCANNER", true, 115200));
TerminalDDLayer* terminal;      // for showing trace of reading data

void setup_DD() {
  terminal = dumbdisplay.createTerminalLayer(1500, 1000);
  terminal->border(5, "blue");
  terminal->padding(5);
  dumbdisplay.configAutoPin(DD_AP_VERT);
  } 






void setup() {
    USBSerial.begin(19200);
    delay(3000);  //le temps de passer sur la Serial Monitor ;-)
    USBSerial.println("\n\n\n\nCa commence !\n");
    
    setup_DD();   //configure DumbDisplay

    setup_WIFI();

    USBSerial.println("C'est parti !");
    terminal->println("C'est parti !");
}


void loop() {
    USBSerial.println("Scan start");
    terminal->println("Scan start");

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    USBSerial.println("Scan done");
    terminal->println("Scan done");
    if (n == 0) {
        USBSerial.println("no networks found");
        terminal->println("no networks found");
    } else {
        USBSerial.print(n);
        terminal->print(n);
        USBSerial.println(" networks found");
        terminal->println(" networks found");
        USBSerial.println("Nr, SSID, RSSI, CH");
        terminal->println("Nr, SSID, RSSI, CH");
        for (int i = 0; i < n; ++i) {
            String zString = String(i+1) + ", " + WiFi.SSID(i) + ", " + WiFi.RSSI(i) + ", " + WiFi.channel(i);
            USBSerial.println(zString);
            terminal->println(zString);
        }
    }
    USBSerial.println("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    // Wait a bit before scanning again.
    delay(500);

    // zString = "NO2:" + String(NO2) + "  EtOH:" + String(C2H5OH) + "    "    ;
    // lcd->setCursor(0, 0);
    // lcd->print(zString);
    // zString = "VOC:" + String(VOC) + "  CO:" + String(CO) + "    "          ;
    // lcd->setCursor(0, 1);
    // lcd->print(zString);

    // delay(100);
}

