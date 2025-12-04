#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

// Wi-Fi credentials
const char* ssid = "Nothing Phone (3a)";
const char* password = "12345678sh";

// I2C OLED display (128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins
#define SDA_PIN 6
#define SCL_PIN 5
#define BUZZER_PIN 10

// Alarm time
int alarmHour = 7;
int alarmMinute = 0;
bool alarmTriggered = false;

WebServer server(80);

// HTML form page for setting the alarm
const char* htmlForm = R"rawliteral(
<!DOCTYPE html>
<html>
  <head><title>ESP32 Alarm Clock</title></head>
  <body>
    <h2>Set Alarm Time (12-Hour Format)</h2>
    <form action="/set">
      Hour (1-12): <input type="number" name="hour" min="1" max="12" required><br>
      Minute: <input type="number" name="minute" min="0" max="59" required><br>
      AM/PM: 
      <select name="ampm">
        <option value="AM">AM</option>
        <option value="PM">PM</option>
      </select><br><br>
      <input type="submit" value="Set Alarm">
    </form>
  </body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(BUZZER_PIN, OUTPUT);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  // Wi-Fi setup
  WiFi.begin(ssid, password);
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 20) {
    delay(500);
    Serial.print(".");
    tries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi connected!");
    display.println(WiFi.localIP());
    display.display();
  } else {
    display.println("WiFi Failed!");
    display.display();
    return;
  }

  // NTP time
  configTzTime("IST-5:30", "pool.ntp.org", "time.nist.gov");

  // Web server setup
  server.on("/", []() {
    server.send(200, "text/html", htmlForm);
  });

  server.on("/set", []() {
  if (server.hasArg("hour") && server.hasArg("minute") && server.hasArg("ampm")) {
    int hour = server.arg("hour").toInt();
    int minute = server.arg("minute").toInt();
    String ampm = server.arg("ampm");

    // Convert to 24-hour format
    if (ampm == "PM" && hour != 12) hour += 12;
    if (ampm == "AM" && hour == 12) hour = 0;

    alarmHour = hour;
    alarmMinute = minute;
    alarmTriggered = false;

    Serial.printf("Alarm set to %02d:%02d (%s)\n", hour, minute, ampm.c_str());
    server.send(200, "text/html", "<h2>Alarm Set!</h2><a href='/'>Back</a>");
  } else {
    server.send(400, "text/html", "<h2>Invalid input</h2><a href='/'>Back</a>");
  }
});

  server.begin();
  Serial.println("Server started!");
}

void loop() {
  server.handleClient();

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    showTime(timeinfo);

    if (timeinfo.tm_hour == alarmHour && timeinfo.tm_min == alarmMinute && !alarmTriggered) {
      triggerAlarm();
      alarmTriggered = true;
    }

    if (timeinfo.tm_min != alarmMinute) {
      alarmTriggered = false;  // reset for next day
    }
  }

  delay(1000);
}

void showTime(struct tm timeinfo) {
  int hour12 = timeinfo.tm_hour % 12;
  if (hour12 == 0) hour12 = 12;
  const char* ampm = (timeinfo.tm_hour >= 12) ? "PM" : "AM";

  char timeStr[9];  // HH:MM
  snprintf(timeStr, sizeof(timeStr), "%2d:%02d", hour12, timeinfo.tm_min);

  display.clearDisplay();

  // Make digits larger
  display.setTextSize(3);              // BIG text
  display.setTextColor(SSD1306_WHITE);

  // Center the time
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
  int centerX = (SCREEN_WIDTH - w) / 2;
  display.setCursor(centerX, 10);
  display.print(timeStr);

  // Draw AM/PM below
  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 24), 50);
  display.print(ampm);

  display.display();
}

void triggerAlarm() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("ALARM!");
  display.display();

  for (int i = 0; i < 10; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    delay(300);
  }
}