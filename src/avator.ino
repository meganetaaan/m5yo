#include <M5Stack.h>
#include "avator.h"
#include "const.h"
#include <AquesTalkTTS.h>
#include <string.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

const char* host = "api.justyo.co";
WiFiClient client;
const int httpsPort = 80;

Avator *avator;
int count = 0;
float f = 0;
float last = 0;
const Expression expressions[] = {
  Angry,
  Sleepy,
  Happy,
  Sad
};
const int expressionsSize = sizeof(expressions) / sizeof(Expression);
int idx = 0;
unsigned long previousYo = 0;

void breath(void *args)
{
  int c = 0;
  for(;;)
  {
    c = c + 1 % 100;
    float f = sin(c * 2 * PI / 100.0);
    avator->setBreath(f);
    delay(33);
  }
}
void drawLoop(void *args)
{
  for(;;)
  {
    unsigned long currentMillis = millis();
    bool yo = currentMillis - previousYo < 3000;
    int level = TTS.getLevel();
    float f = level / 12000.0;
    float open = _min(1.0, last + f / 2.0);
    // count += 3;
    // float f0 = ((count % 360) / 180.0) * PI;
    // float open = (sin(f0) + 1.0) / 2.0;
    last = f;
    avator->setMouthOpen(open);
    avator->draw(yo);
    delay(33);
  }
}

void saccade(void *args)
{
  for(;;)
  {
    float vertical = (float)rand()/(float)(RAND_MAX / 2) - 1;
    float horizontal = (float)rand()/(float)(RAND_MAX / 2) - 1;
    avator->setGaze(vertical, horizontal);
    delay(500 + 100 * random(20));
  }
}

void blink(void *args)
{
  for(;;)
  {
    avator->setEyeOpen(1);
    delay(2500 + 100 * random(20));
    avator->setEyeOpen(0);
    delay(300 + 10 * random(20));
  }
}

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
  avator = new Avator();
  xTaskCreatePinnedToCore(
      drawLoop,   /* Function to implement the task */
      "drawLoop", /* Name of the task */
      4096,       /* Stack size in words */
      NULL,       /* Task input parameter */
      1,          /* Priority of the task */
      NULL,       /* Task handle. */
      1);         /* Core where the task should run */
  xTaskCreatePinnedToCore(
      saccade,   /* Function to implement the task */
      "saccade", /* Name of the task */
      4096,      /* Stack size in words */
      NULL,      /* Task input parameter */
      3,         /* Priority of the task */
      NULL,      /* Task handle. */
      1);        /* Core where the task should run */
  xTaskCreatePinnedToCore(
      breath,   /* Function to implement the task */
      "breath", /* Name of the task */
      4096,     /* Stack size in words */
      NULL,     /* Task input parameter */
      2,        /* Priority of the task */
      NULL,     /* Task handle. */
      1);       /* Core where the task should run */
  xTaskCreatePinnedToCore(
      blink,   /* Function to implement the task */
      "blink", /* Name of the task */
      4096,    /* Stack size in words */
      NULL,    /* Task input parameter */
      2,       /* Priority of the task */
      NULL,    /* Task handle. */
      1);      /* Core where the task should run */
}

void yo()
{
  Serial.println("yo");
  HTTPClient http;
  http.begin(host, 80, "/yoall/"); //HTTP
  int httpCode = http.POST("api_token=" + YO_TOKEN);

  if (httpCode > 0)
  {
    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
  else
  {
    Serial.println(httpCode);
  }
}

void yo2()
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
    yo2();
  }
}
