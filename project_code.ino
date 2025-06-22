int offset = 20;

const int relayMainPin = 5;  // ريليه المصدر الرئيسي
const int relay1Pin = 2;     // ريليه البطارية 1
const int relay2Pin = 3;     // ريليه البطارية 2
const int relay3Pin = 4;     // ريليه الطوارئ

const float mainThreshold = 12.0;     // أقل جهد مقبول للمصدر الرئيسي
const float batteryThreshold = 11.5;  // أقل جهد مقبول للبطاريات

void setup() {
  Serial.begin(9600);

  pinMode(relayMainPin, OUTPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);

  // إطفاء كل الريليهات في البداية
  digitalWrite(relayMainPin, HIGH);
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(relay3Pin, HIGH);
}

void loop() {
  // قراءة جهد المصدر الرئيسي (A2)
  int rawMain = analogRead(A2);
  double voltageMain = map(rawMain, 0, 1023, 0, 2500) + offset;
  voltageMain /= 100.0;

  // قراءة جهد البطارية الأولى (A0)
  int raw1 = analogRead(A0);
  double voltage1 = map(raw1, 0, 1023, 0, 2500) + offset;
  voltage1 /= 100.0;

  // قراءة جهد البطارية الثانية (A1)
  int raw2 = analogRead(A1);
  double voltage2 = map(raw2, 0, 1023, 0, 2500) + offset;
  voltage2 /= 100.0;

  // طباعة الجهود
  Serial.print("Main Source Voltage = ");
  Serial.print(voltageMain);
  Serial.println(" V");

  Serial.print("Battery 1 Voltage = ");
  Serial.print(voltage1);
  Serial.println(" V");

  Serial.print("Battery 2 Voltage = ");
  Serial.print(voltage2);
  Serial.println(" V");

  // منطق التشغيل حسب الأولوية
  if (voltageMain >= mainThreshold) {
    digitalWrite(relayMainPin, LOW);   // ON
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, HIGH);
    digitalWrite(relay3Pin, HIGH);
    Serial.println("Using Main Source (Relay 5)");
  } else if (voltage1 >= batteryThreshold) {
    digitalWrite(relayMainPin, HIGH);
    digitalWrite(relay1Pin, LOW);      // ON
    digitalWrite(relay2Pin, HIGH);
    digitalWrite(relay3Pin, HIGH);
    Serial.println("Using Battery 1 (Relay 2)");
  } else if (voltage2 >= batteryThreshold) {
    digitalWrite(relayMainPin, HIGH);
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, LOW);      // ON
    digitalWrite(relay3Pin, HIGH);
    Serial.println("Using Battery 2 (Relay 3)");
  } else {
    digitalWrite(relayMainPin, HIGH);
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, HIGH);
    digitalWrite(relay3Pin, LOW);      // ON
    Serial.println("No battery available! Emergency mode ON (Relay 4)");
  }

  delay(1000);
}
