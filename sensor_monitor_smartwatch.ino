#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Grove sensor pins
const int tempPin  = A0;
const int soundPin = A1;
const int lightPin = A2;

// Grove button pin
const int buttonPin = 2;
const int BUTTON_PRESSED = HIGH;  // Change to LOW if button works opposite

// Alert thresholds
const float TEMP_HIGH = 30.0;
const float TEMP_LOW  = 15.0;

const int SOUND_HIGH = 70;
const int LIGHT_HIGH = 80;
const int LIGHT_LOW  = 10;

// Timing filter
const unsigned long ALERT_HOLD_TIME = 3000;   // must stay bad for 3 seconds
const unsigned long NORMAL_RESET_TIME = 1500; // must stay normal for 1.5 seconds

bool alertShowing = false;
bool alertAcknowledged = false;

String currentAlert = "";
String pendingAlert = "";
String acknowledgedAlert = "";

unsigned long alertStartTime = 0;
unsigned long normalStartTime = 0;

bool lastButtonState = LOW;
unsigned long lastButtonTime = 0;
const unsigned long debounceDelay = 80;

void setup() {
  u8g2.begin();
  pinMode(buttonPin, INPUT);
}

void loop() {
  int tempRaw  = analogRead(tempPin);
  int soundRaw = analogRead(soundPin);
  int lightRaw = analogRead(lightPin);

  float voltage = tempRaw * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0;

  int soundPercent = map(soundRaw, 0, 1023, 0, 100);
  int lightPercent = map(lightRaw, 0, 1023, 0, 100);

  soundPercent = constrain(soundPercent, 0, 100);
  lightPercent = constrain(lightPercent, 0, 100);

  String detectedAlert = getAlertMessage(temperatureC, soundPercent, lightPercent);

  checkButton();
  updateAlertState(detectedAlert);

  if (currentAlert == "") {
    drawSensorMonitor(temperatureC, soundPercent, lightPercent);
  } 
  else if (alertAcknowledged && currentAlert == acknowledgedAlert) {
    drawSensorMonitor(temperatureC, soundPercent, lightPercent);
  } 
  else {
    alertShowing = true;
    drawAlert(currentAlert);
  }

  delay(100);
}

String getAlertMessage(float tempC, int soundPercent, int lightPercent) {
  if (tempC >= TEMP_HIGH) {
    return "TEMP TOO HIGH";
  }

  if (tempC <= TEMP_LOW) {
    return "TEMP TOO LOW";
  }

  if (soundPercent >= SOUND_HIGH) {
    return "SOUND TOO LOUD";
  }

  if (lightPercent >= LIGHT_HIGH) {
    return "LIGHT TOO BRIGHT";
  }

  if (lightPercent <= LIGHT_LOW) {
    return "LIGHT TOO DARK";
  }

  return "";
}

void updateAlertState(String detectedAlert) {
  unsigned long now = millis();

  if (detectedAlert == "") {
    if (normalStartTime == 0) {
      normalStartTime = now;
    }

    if (now - normalStartTime >= NORMAL_RESET_TIME) {
      currentAlert = "";
      pendingAlert = "";
      alertShowing = false;
      alertAcknowledged = false;
      acknowledgedAlert = "";
      alertStartTime = 0;
    }

    return;
  }

  normalStartTime = 0;

  if (detectedAlert != pendingAlert) {
    pendingAlert = detectedAlert;
    alertStartTime = now;
    return;
  }

  if (now - alertStartTime >= ALERT_HOLD_TIME) {
    currentAlert = detectedAlert;

    if (currentAlert != acknowledgedAlert) {
      alertAcknowledged = false;
    }
  }
}

void checkButton() {
  bool buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    lastButtonTime = millis();
  }

  if ((millis() - lastButtonTime) > debounceDelay) {
    if (buttonState == BUTTON_PRESSED && lastButtonState != BUTTON_PRESSED) {
      if (currentAlert != "") {
        alertShowing = false;
        alertAcknowledged = true;
        acknowledgedAlert = currentAlert;
      }
    }
  }

  lastButtonState = buttonState;
}

void drawSensorMonitor(float tempC, int soundPercent, int lightPercent) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);

  u8g2.drawStr(0, 12, "Sensor Monitor");
  u8g2.drawHLine(0, 14, 128);

  char line1[32];
  char line2[32];
  char line3[32];

  snprintf(line1, sizeof(line1), "Temp:  %.1f C", tempC);
  snprintf(line2, sizeof(line2), "Sound: %3d %%", soundPercent);
  snprintf(line3, sizeof(line3), "Light: %3d %%", lightPercent);

  u8g2.drawStr(0, 30, line1);
  u8g2.drawStr(0, 45, line2);
  u8g2.drawStr(0, 60, line3);

  u8g2.sendBuffer();
}

void drawAlert(String message) {
  u8g2.clearBuffer();

  // White alert box
  u8g2.setDrawColor(1);
  u8g2.drawBox(10, 18, 108, 34);

  // Black text inside white box
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_7x14_tf);

  u8g2.drawStr(18, 34, "ALERT!");
  u8g2.drawStr(18, 48, message.c_str());

  u8g2.setDrawColor(1);
  u8g2.sendBuffer();
}
