#include <M5Stack.h>
#include "const.h"
#include <AquesTalkTTS.h>
#include <tasks/LipSync.h>
#include <Avatar.h>
#include <string.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

using namespace m5avatar;

const char* host = "api.justyo.co";
WiFiClient client;
const int httpsPort = 80;

Avatar *avatar;
int count = 0;
float f = 0;
float last = 0;
const Expression expressions[] = {
  Expression::Angry,
  Expression::Sleepy,
  Expression::Happy,
  Expression::Sad
};
const int expressionsSize = sizeof(expressions) / sizeof(Expression);
int idx = 0;
unsigned long previousYo = 0;

boolean configWifi()
{
  if (WiFi.begin(SSID.c_str(), PASSWORD.c_str()))
  {
    delay(10);
    unsigned long timeout = 10000;
    unsigned long previousMillis = millis();
    while (true)
    {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > timeout)
        break;
      if (WiFi.status() == WL_CONNECTED)
        return true;
      delay(1000);
    }
    return false;
  }
}

void setup()
{
  while (true)
  {
    if (configWifi())
    {
      Serial.println("success");
      break;
    }
    else
    {
      Serial.println("fail");
      Serial.println("fail. retry within 5 seconds...");
      delay(5000);
    }
  }
  int iret;
  M5.begin();
  M5.Lcd.setBrightness(30);
  M5.Lcd.clear();
  iret = TTS.create(AQUESTALK_KEY);
  avatar = new Avatar();
  avatar->init();
  avatar->addTask(lipSync, "lipSync");
}

void yo()
{
  previousYo = millis();
  Serial.println("host connection");
  if (!client.connect(host, httpsPort)) {
    Serial.println("host connection failed");
    return;
  }
  Serial.println("host connection success");
  String url = "/yo/";
  String params = "api_token=" + YO_TOKEN + "&username=mgntn2\r\n\r\n";
  int length = params.length();

  String request = "POST " + url + " HTTP/1.1\r\n"
  + "Host: " + host + "\r\n"
  + "Content-type: application/x-www-form-urlencoded\r\n"
  + "Content-length: " + String(length) + "\r\n"
  + "Connection: close\r\n"
  + "\r\n" + params + "\r\n\r\n";

  Serial.println("making request: " + request);
  client.print(request);

while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r")
    {
      break;
    }
  }
}

void loop()
{
  M5.update();
  if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed())
  {
    TTS.play("yo'-", 90);
    yo();
  }
}
