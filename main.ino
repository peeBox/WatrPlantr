#include <ESP8266WiFi.h>

// replace ### with your own info

const char* ssid = "###";
const char* password = "###";

const char* apiKey = "###";
const char* eventName = "button_status";
const char* host = "maker.ifttt.com";
const int port = 80;

const int dry = 733;
const int wet = 317;
const int pump = D8;
bool x = false;
char* buttonState = "Pressed";

void setup()
{
  Serial.begin(115200);
  pinMode(pump, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to network");
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    if (++counter > 100)
      ESP.restart();
    Serial.print( "." );
  }
  Serial.println("\nWiFi connected");
  Serial.println("All configured. Now waiting...");
}
void loop()
{
  WiFiClient client;

  int sensorVal = analogRead(A0);
  int Humi = map(sensorVal, wet, dry, 100, 0);
  digitalWrite(pump, LOW);

  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  if (Humi < 50)
  {
    Serial.print("Pompe en marche");
    digitalWrite(pump, HIGH);
    delay(400);
    digitalWrite(pump, LOW);
    delay(200);
    Serial.print(Humi);
    Serial.println("%");
    if (x == false) {
      String url = "/trigger/";
      url += eventName;
      url += "/with/key/";
      url += apiKey;

      Serial.println("Sending trigger to server");
      Serial.println(url);

      Serial.print(String("POST ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Content-Type: application/x-www-form-urlencoded\r\n" +
                   "Content-Length: 15\r\n\r\n" +
                   "value1=" + buttonState + "\r\n");
      delay(1000)
    }
    x = true;
  } else {
    x = false;
    Serial.print(Humi);
    Serial.println("%");
    delay(1000);
  }
}
