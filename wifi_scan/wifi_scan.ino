// Super petit scanner WIFI à base d'ESP32-c3 avec résultat sur DumbDisplay sur le smartphone ;-)
// zf240325.1204
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
  terminal = dumbdisplay.createTerminalLayer(1000, 1000);
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

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    USBSerial.println("Scan done");
    if (n == 0) {
        USBSerial.println("no networks found");
    } else {
        USBSerial.print(n);
        USBSerial.println(" networks found");
        USBSerial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            USBSerial.printf("%2d",i + 1);
            USBSerial.print(" | ");
            USBSerial.printf("%-32.32s", WiFi.SSID(i).c_str());
            USBSerial.print(" | ");
            USBSerial.printf("%4d", WiFi.RSSI(i));
            USBSerial.print(" | ");
            USBSerial.printf("%2d", WiFi.channel(i));
            USBSerial.print(" | ");
            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                USBSerial.print("open");
                break;
            case WIFI_AUTH_WEP:
                USBSerial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                USBSerial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                USBSerial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                USBSerial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                USBSerial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                USBSerial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                USBSerial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                USBSerial.print("WAPI");
                break;
            default:
                USBSerial.print("unknown");
            }
            USBSerial.println();
            delay(10);
        }
    }
    USBSerial.println("");

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

