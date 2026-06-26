const int OUTLET_1 = 2; // Relay Channel 1 connected to Digital Pin 2
const int OUTLET_2 = 3; // Relay Channel 2 connected to Digital Pin 3 (Optional extra outlet)

void setup() {
  // Initialize hardware serial communication at 9600 baud rate for HC-05
  Serial.begin(9600); 
  
  // Configure the relay control pins as outputs
  pinMode(OUTLET_1, OUTPUT);
  pinMode(OUTLET_2, OUTPUT);
  
  // SAFETY RESET: Robodo relays are Active-Low. 
  // Setting pins to HIGH ensures outlets default to completely OFF when booting up.
  digitalWrite(OUTLET_1, HIGH); 
  digitalWrite(OUTLET_2, HIGH);
}

void loop() {
  // Check if there is incoming data from the phone's Bluetooth app
  if (Serial.available() > 0) {
    char bluetoothCmd = Serial.read(); // Read the incoming character
    
    // --- OUTLET 1 CONTROL (Your Laptop Charger) ---
    if (bluetoothCmd == '1') {
      digitalWrite(OUTLET_1, LOW);  // Pulling LOW completes the AC circuit (Turns ON)
      Serial.println("Outlet 1: ON (Charging Started)");
    } 
    else if (bluetoothCmd == '0') {
      digitalWrite(OUTLET_1, HIGH); // Pulling HIGH breaks the AC circuit (Turns OFF)
      Serial.println("Outlet 1: OFF (Charging Stopped)");
    }
    
    // --- OUTLET 2 CONTROL (Optional Auxiliary Appliance) ---
    else if (bluetoothCmd == '2') {
      digitalWrite(OUTLET_2, LOW);  // Turns ON Channel 2
      Serial.println("Outlet 2: ON");
    } 
    else if (bluetoothCmd == '9') {
      digitalWrite(OUTLET_2, HIGH); // Turns OFF Channel 2
      Serial.println("Outlet 2: OFF");
    }
  }
}