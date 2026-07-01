# Bluetooth-Controlled Smart Extension Board using Arduino

A DIY home-automation project that retrofits a standard AC mains extension switchboard with an Arduino Uno, an HC-05 Bluetooth module, and an electromagnet relay to safely toggle high-voltage appliances wirelessly via a smartphone terminal app.

---

## 🛠️ Hardware Requirements
* **Microcontroller:** Arduino Uno R3
* **Bluetooth Module:** HC-05 (Slave configuration, default baud rate 9600)
* **Relay Module:** 2-Channel 5V Active-Low Relay Board (Robodo / Equivalent)
* **Power Source:** 5V USB (via Laptop or Power Bank)
* **Enclosure:** Standard multi-socket AC extension box
* **Wiring:** 
  * Low-Voltage: Female-to-Male (F-to-M) DuPont jumper wires
  * High-Voltage: 1.0 / 1.5 sq mm flexible insulated copper AC wire (salvaged)

---

## ⚡ Circuit & Wiring Architecture

### 1. Low-Voltage DC Control Side
Both the Bluetooth module and the relay control board share the Arduino's 5V and GND power tracks via a breadboard parallel rail circuit.
* **HC-05 Bluetooth:** `VCC` ➔ 5V | `GND` ➔ GND | `TXD` ➔ Pin 0 (RX) | `RXD` ➔ Pin 1 (TX)
* **Relay Board (4-Pin Row):** `VCC` ➔ 5V | `GND` ➔ GND | `IN1` ➔ Pin 2
* **Relay Power Jumper:** Ensure the logic isolation cap bridging `JD-VCC` and `VCC` remains intact.

### 2. High-Voltage AC Mains Intercept
The relay is configured on the **Live (Phase)** wire to act as an active gatekeeper while retaining the original manual physical switch as a master safety override.
* **Switch Board Output:** Heavy-duty wire runs from the bottom of the manual rocker switch into the **Middle Hole (COM)** terminal of the relay (`K1`).
* **Socket Input:** Output heavy-duty wire runs from the **Left Hole (NO - Normally Open)** terminal of the relay into the right-hand socket terminal.
* **Normally Closed (NC):** The rightmost hole remains completely disconnected to ensure the appliance defaults to safe-OFF if the microcontroller loses power.

---

## 🚀 Installation & Deployment Instructions

### 1. Uploading the Software
> ⚠️ **CRITICAL:** You must physically disconnect the wires from **Pin 0 (RX)** and **Pin 1 (TX)** on the Arduino prior to compiling and uploading the sketch. The onboard USB-to-Serial converter shares these hardware UART lines; failing to unplug them will create a data collision and throw an upload error (`stk500_getsync()`).

1. Open `src/smart_extension.ino` in the Arduino IDE.
2. Select your board (`Arduino Uno`) and the active communication port (`Tools -> Port`).
3. Flash the code to the microcontroller.
4. Once completed, securely re-plug the HC-05 `TXD` and `RXD` back into Pins 0 and 1.

### 2. Smartphone Pairing & Execution
1. Download any standard serial application (e.g., *Serial Bluetooth Terminal* on Android or *BLE Terminal* on iOS).
2. Scan and pair your smartphone with the `HC-05` module using the system PIN `1234`.
3. Open the application, connect to the channel, and pass character codes down the serial stream:
   * Send `1` ➔ Closes the relay loop ➔ Turns the outlet **ON**
   * Send `0` ➔ Snaps the circuit open ➔ Turns the outlet **OFF**

---

## 🔒 Safety Systems
* **Isolation Mapping:** High-voltage AC tracks are physically separated from the breadboard's low-voltage DC signals.
* **Fail-Safe State:** The wire configuration utilizes the **Normally Open (NO)** channel, forcing the high-voltage relay to drop dead into an open, isolated state if the system undergoes a sudden crash, brownout, or total power loss.

---

## 🔍 Troubleshooting & Edge Cases Conquered

During the fabrication and initial testing phases, several distinct hardware and communication bottlenecks were encountered. Below is the debugging matrix and structural edge cases to look out for:

### 1. The Shared Serial Highway Conflict (Upload Failures)
* **The Symptom:** Attempting to flash updated logic to the microcontroller outputs a compilation failure or connection timeout error: `avrdude: stk500_getsync(): not in sync: resp=0x00`.
* **The Root Cause:** The Arduino Uno shares its hardware UART lines (Digital Pin 0 and Pin 1) directly with the onboard USB-to-Serial processing chip connected to your PC. Leaving the HC-05 `TXD`/`RXD` lines connected during compilation induces data collision, jamming the programming interface.
* **The Resolution:** Physically decouple the jumpers from Pins 0 and 1 before hitting the "Upload" command inside the IDE environment. Safely reconnect them only after the status bar returns `Done Uploading`.

### 2. Serial Communication Corruption (The Garbage Buffer Loop)
* **The Symptom:** Connecting via the terminal application initiates an infinite loop printing unreadable characters or gibberish fragments like `@^` or `⸮⸮⸮`.
* **The Root Cause:** This is typically a symptom of either crossed data lines or a Baud Rate mismatch between the transceiver arrays. 
* **The Resolution:** * Verify cross-over wiring topology: Ensure HC-05 `TXD` maps to Arduino `RX (Pin 0)` and HC-05 `RXD` maps to Arduino `TX (Pin 1)`. 
  * Match system transmission speeds: Ensure the software serial monitoring utility or application layout is configured precisely to **9600 Baud**, matching the initialization value specified in `Serial.begin(9600)`.

### 3. Missing Mechanical Switch-Over (LED Activates, No AC Output)
* **The Symptom:** Sending the character code `1` illuminates the onboard tracking LED on the relay board, but the module fails to pipe AC power to the secondary wall outlet.
* **The Root Cause:** The internal mechanical coil demands a sudden peak current draw to throw its electromagnetic rocker. If the logic isolation jumper block (the physical plastic shunt capping `JD-VCC` and `VCC`) is missing or loose, the coil lines drop completely dead, leaving the high-voltage relay stranded in an open circuit.
* **The Resolution:** Ensure the physical copper/plastic logic shunt remains seated snugly over the `JD-VCC` and `VCC` pin array to correctly tap power from the Arduino’s main 5V rail.

### 4. Direct Terminal Crossover Misalignment (The Left-to-Right Mixup)
* **The Symptom:** The code flashes cleanly, the Bluetooth handshake succeeds, and the relay physically clicks, yet the appliance connected down the loop completely ignores commands.
* **The Root Cause:** Passing the structural AC line through the wrong output terminal hole (e.g., screwing your load wire into the `NC` block instead of the `NO` block). On specific production module variants (such as the Robodo series), explicit text labels like `COM` and `NO` are omitted for raw channel markers (`K1` / `K2`).
* **The Resolution:** Keeping the terminal block array facing towards you, precisely conform to the high-voltage index: Left Hole = `NO` (Connect to Socket Input), Middle Hole = `COM` (Connect to Switch Line), Right Hole = `NC` (Leave open).
