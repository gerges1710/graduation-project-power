const int offset = 20; // Voltage sensor calibration offset

const int relayPins[] = {5, 2, 3, 4};  // Relay pins: Main, Battery1, Battery2, Emergency
const int voltagePins[] = {A2, A0, A1}; // Voltage sensor pins: Main, Battery1, Battery2
const float thresholds[] = {11.5, 11.5, 11.5}; // Voltage threshold for each source
const int currentSensor1 = A3; // Analog pin for current sensor 1

const float vcc = 5.0;    // Arduino operating voltage (V)
const int adcMax = 1023;  // ADC resolution
const float sens1 = 0.0635; // Sensitivity of ACS current sensor (V/A)

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud rate

  // Initialize relay pins as outputs and turn them OFF (HIGH state)
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT); // Set relay pin as output
    digitalWrite(relayPins[i], HIGH); // Turn relay OFF
  }
}

void loop() {
  float voltages[3]; // Array to store voltage readings

  // Read voltage from each sensor
  for (int i = 0; i < 3; i++) {
    int raw = analogRead(voltagePins[i]); // Read analog value
    voltages[i] = (map(raw, 0, 1023, 0, 2500) + offset) / 100.0; // Convert to voltage (V)
  }

  // Read current from sensor 1
  float current1 = readCurrent(currentSensor1);

  // Simulate current for Battery 1: 0.06A if relay is ON, else 0.0A
  float current2 = digitalRead(relayPins[1]) == LOW ? 0.06 : 0.0;

  // Labels for each voltage source
  const char* labels[] = {"Main", "Battery 1", "Battery 2"};

  // Print voltage readings
  for (int i = 0; i < 3; i++) {
    Serial.print(labels[i]);
    Serial.print(" Voltage = ");
    Serial.print(voltages[i]);
    Serial.println(" V");
  }

  // Print current readings
  Serial.print("Current Sensor 1 = ");
  Serial.print(current1, 3);
  Serial.println(" A");

  Serial.print("Current Sensor 2 = ");
  Serial.print(current2, 3);
  Serial.println(" A");

  // Select default source as Emergency mode
  int selected = 3;

  // Check each source voltage and select the first with voltage >= threshold
  for (int i = 0; i < 3; i++) {
    if (voltages[i] >= thresholds[i]) {
      selected = i;
      break;
    }
  }

  // Activate relays based on the selected source
  for (int i = 0; i < 4; i++) {
    if (selected == 0 && (i == 0 || i == 1)) {
      // If Main source is selected → turn ON relay for Main (Pin 5) and Battery 1 (Pin 2)
      digitalWrite(relayPins[i], LOW);
    } else {
      // For other cases → turn ON only the selected relay, others OFF
      digitalWrite(relayPins[i], i == selected ? LOW : HIGH);
    }
  }

  // Print the currently used source
  Serial.print("Using: ");
  Serial.println(selected == 0 ? "Main Source (Relay 5 + Relay 2)" :
                 selected == 1 ? "Battery 1 (Relay 2)" :
                 selected == 2 ? "Battery 2 (Relay 3)" :
                                 "Emergency mode ON (Relay 4)");

  Serial.println("-----------------------------");

  delay(1000); // Wait 1 second before next loop
}

// Function to read current from ACS sensor
float readCurrent(int pin) {
  float total = 0;

  // Take 1000 samples for accuracy
  for (int i = 0; i < 1000; i++) {
    total += analogRead(pin); // Read analog value
    delayMicroseconds(100); // Small delay between samples
  }

  float avg = total / 1000.0; // Average reading
  float voltage = (avg / adcMax) * vcc; // Convert ADC reading to voltage
  float current = (voltage - (vcc / 2)) / sens1; // Convert voltage to current (A)

  return abs(current) < 0.1 ? 0.0 : current; // Ignore noise below 0.1A
}