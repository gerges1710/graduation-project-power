const int offset = 20;

const int relayPins[] = {5, 2, 3, 4};  // [Main, Battery1, Battery2, Emergency]
const int voltagePins[] = {A2, A0, A1}; // [Main, Battery1, Battery2]
const float thresholds[] = {11.5, 11.5, 11.5}; // نفس العتبة لكل مصدر
const int currentSensor1 = A3;

const float vcc = 5.0;
const int adcMax = 1023;
const float sens1 = 0.0635;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) pinMode(relayPins[i], OUTPUT), digitalWrite(relayPins[i], HIGH);
}

void loop() {
  float voltages[3];
  for (int i = 0; i < 3; i++) {
    int raw = analogRead(voltagePins[i]);
    voltages[i] = (map(raw, 0, 1023, 0, 2500) + offset) / 100.0;
  }

  float current1 = readCurrent(currentSensor1);
  float current2 = digitalRead(relayPins[1]) == LOW ? 0.06 : 0.0;

  // طباعة الجهود
  const char* labels[] = {"Main", "Battery 1", "Battery 2"};
  for (int i = 0; i < 3; i++) {
    Serial.print(labels[i]);
    Serial.print(" Voltage = ");
    Serial.print(voltages[i]);
    Serial.println(" V");
  }

  // طباعة التيارات
  Serial.print("Current Sensor 1 = "); Serial.print(current1, 3); Serial.println(" A");
  Serial.print("Current Sensor 2 = "); Serial.print(current2, 3); Serial.println(" A");

  // منطق التشغيل حسب الأولوية
  int selected = 3; // Emergency mode
  for (int i = 0; i < 3; i++) {
    if (voltages[i] >= thresholds[i]) {
      selected = i;
      break;
    }
  }

  for (int i = 0; i < 4; i++) {
    digitalWrite(relayPins[i], i == selected ? LOW : HIGH);
  }

  Serial.print("Using: ");
  Serial.println(selected == 0 ? "Main Source (Relay 5)" :
                 selected == 1 ? "Battery 1 (Relay 2)" :
                 selected == 2 ? "Battery 2 (Relay 3)" :
                                 "Emergency mode ON (Relay 4)");

  Serial.println("-----------------------------");
  delay(1000);
}

float readCurrent(int pin) {
  float total = 0;
  for (int i = 0; i < 1000; i++) {
    total += analogRead(pin);
    delayMicroseconds(100);
  }
  float avg = total / 1000.0;
  float voltage = (avg / adcMax) * vcc;
  float current = (voltage - (vcc / 2)) / sens1;
  return abs(current) < 0.1 ? 0.0 : current;
}
