
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// CalibraSense Pro - ESP32-WROOM
// Verify the LCD I2C address (0x27 or 0x3F) before use.

const int POT_PIN    = 34;
const int LED_PIN    = 33;
const int BUZZER_PIN = 25;  // Move buzzer signal wire from GPIO34
const int LCD_SDA    = 23;
const int LCD_SCL    = 21;

const int ADC_MAX = 4095;
const int SAMPLE_COUNT = 20;
const unsigned long LOG_INTERVAL_MS = 500;

// Candidate thresholds derived from observed data.
// Validate them with controlled potentiometer tests.
const float LOWER_THRESHOLD = 2450.0;
const float UPPER_THRESHOLD = 2565.0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

enum SystemState { NORMAL, HIGH_STATE };
SystemState state = NORMAL;

unsigned long lastLogTime = 0;
unsigned long stateStartTime = 0;
unsigned long eventCount = 0;

float filteredADC = 0.0;
bool filterInitialized = false;

const char* stateName() {
  return state == HIGH_STATE ? "HIGH" : "NORMAL";
}

void updateOutputs() {
  digitalWrite(LED_PIN, state == HIGH_STATE ? HIGH : LOW);
  digitalWrite(BUZZER_PIN, state == HIGH_STATE ? HIGH : LOW);
}

void updateLCD(float rawADC) {
  lcd.setCursor(0, 0);
  lcd.print("ADC:");
  lcd.print((int)rawADC);
  lcd.print(" F:");
  lcd.print((int)filteredADC);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("State: ");
  lcd.print(stateName());
  lcd.print("        ");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(115200);
  analogReadResolution(12);

  // SDA = GPIO23, SCL = GPIO21
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CalibraSense");
  lcd.setCursor(0, 1);
  lcd.print("Initializing");

  stateStartTime = millis();
  delay(1000);
  lcd.clear();

  // CSV header: use Serial Monitor at 115200 baud.
  Serial.println(
    "Time_ms,Raw_ADC,Filtered_ADC,State,LED,Buzzer,"
    "EventCount,Duration_ms,EventMarker"
  );

  lastLogTime = millis();
}

void loop() {
  unsigned long now = millis();

  if (now - lastLogTime < LOG_INTERVAL_MS) {
    return;
  }

  lastLogTime = now;

  // Read a batch of samples.
  long sum = 0;
  int rawADC = 0;

  for (int i = 0; i < SAMPLE_COUNT; i++) {
    rawADC = analogRead(POT_PIN);
    sum += rawADC;
    delay(2);
  }

  // Average of 20 ADC samples.
  float averageADC = (float)sum / SAMPLE_COUNT;

  // Use the batch average as the filtered measurement.
  filteredADC = averageADC;
  filterInitialized = true;

  // Hysteresis state machine.
  SystemState previousState = state;

  if (state == NORMAL && filteredADC >= UPPER_THRESHOLD) {
    state = HIGH_STATE;
  } else if (state == HIGH_STATE &&
             filteredADC <= LOWER_THRESHOLD) {
    state = NORMAL;
  }

  String eventMarker = "";

  if (state != previousState) {
    eventCount++;
    unsigned long duration = now - stateStartTime;

    eventMarker = (state == HIGH_STATE) ? "START" : "END";
    stateStartTime = now;
  }

  updateOutputs();
  updateLCD(rawADC);

  unsigned long duration = now - stateStartTime;

  // CSV row. Open Serial Monitor at 115200 baud.
  Serial.print(now);
  Serial.print(",");
  Serial.print(rawADC);
  Serial.print(",");
  Serial.print(filteredADC, 1);
  Serial.print(",");
  Serial.print(stateName());
  Serial.print(",");
  Serial.print(state == HIGH_STATE ? "ON" : "OFF");
  Serial.print(",");
  Serial.print(state == HIGH_STATE ? "ON" : "OFF");
  Serial.print(",");
  Serial.print(eventCount);
  Serial.print(",");
  Serial.print(duration);
  Serial.print(",");
  Serial.println(eventMarker);
}
