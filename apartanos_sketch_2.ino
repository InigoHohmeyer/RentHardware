#include <ArduinoBLE.h>

BLEService service("19B10000-E8F2-537E-4F6C-D104768A1214"); // Custom service UUID
BLEByteCharacteristic commandCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); // Custom characteristic UUID

const int bufferSize = 50; // Increase the buffer size

void setup() {
  Serial.begin(9600); // Initialize the serial monitor
  while (!Serial);
  
  // Initialize the BLE module
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }
  
  // Set the advertised local name and service UUID
  BLE.setLocalName("Arduino");
  BLE.setAdvertisedService(service);
  
  // Add the characteristic to the service
  service.addCharacteristic(commandCharacteristic);
  
  // Add the service to the BLE module
  BLE.addService(service);
  
  // Start advertising
  BLE.advertise();
  
  Serial.println("Arduino is ready to receive commands!");
}

void loop() {
  // Check if a central device is connected
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    
    while (central.connected()) {
      // Check if the central has written to the command characteristic
      if (commandCharacteristic.written()) {
        // Read the command sent by the central
        unsigned char command[bufferSize];
        int commandLength = commandCharacteristic.valueLength();
        if (commandLength > bufferSize) {
          commandLength = bufferSize; // Limit to buffer size
        }
        commandCharacteristic.readValue(command, commandLength);
        
        // Print the received command to the serial monitor
        Serial.print("Received command: ");
        for (int i = 0; i < commandLength; i++) {
          Serial.print((char)command[i]);
        }
        Serial.println();
      }
    }
    
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
  
  // Add the rest of your main loop code here
}

