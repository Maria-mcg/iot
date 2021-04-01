// ArduinoJson - Version: 5.13.4
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>


#include <WiFi101.h>
#include <SPI.h>
#include <TelegramBot.h>
#include <Servo.h>
#include "pitches.h"


const char* ssid = "cupita_wifi";    //  your network SSID (name)
const char* password = "dianetica";  // your network password
const char BotToken[] = "1189296909:AAGBCswxMgC2vu_ZSAwVmGs6-9uy_goNLu4";

WiFiSSLClient client;
TelegramBot bot (BotToken, client);
TelegramKeyboard keyboard_one;

int PortionAmount = 1; // Set default amount of food to 1 portion

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

int buzzerPin = 6; // Pin attached to the buzzer
bool startDetecting = false;
unsigned long timer = 0;
String OldChatId = "";

void setup() {

  Serial.begin(115200);
  delay(3000);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // choose the emoji you like using UNICODE
  // here's the list https://unicode.org/emoji/charts/full-emoji-list.html

  const char*  MusicAndFood = "\U0001F3B6   +   \U0001F36A"; // Note + Cookie
  const char*  MusicNoFood = "\U0001F3B6   NO   \U0001F36A"; // Note NO Cookie
  const char*  OnePortion = "\U0001F408"; // CAT
  const char*  TwoPortion = "\U0001F408 \U0001F408"; // 2 CATS
  const char*  ThreePortion = "\U0001F408 \U0001F408 \U0001F408"; // 3 CATS

  // define your row's
  const char* row_one[] = {MusicAndFood, MusicNoFood};
  const char* row_two[] = {OnePortion, TwoPortion, ThreePortion};

  keyboard_one.addRow(row_one, 2); // assing a row to one or more keyboards
  keyboard_one.addRow(row_two, 3); // second argument is the length of the row
  bot.begin();
  myservo.attach(7);  // attaches the servo on pin 7 to the servo object

  pinMode(buzzerPin, OUTPUT);
}

void loop() {


  message m = bot.getUpdates(); // Read new messages
  if ( m.chat_id != 0 ) { // Checks if there are some updates
    OldChatId = m.chat_id;
    Serial.println(m.text);

    if (m.text == "ud83cudfb6   +   ud83cudf6a") { // if Melody AND food
    
      bot.sendMessage(m.chat_id, "Dispensing "+String(PortionAmount)+" portion of food right now", keyboard_one);  // Reply to the same chat with a text and a custom keyboard
      playMelody(melodyOne, noteDurationsOne, 8);
      moveServo();
      startDetecting = true;
      timer = millis();
    }
    else if (m.text == "ud83cudfb6   NO   ud83cudf6a") { // if Melody and NO food
      bot.sendMessage(m.chat_id, "At your command", keyboard_one);
      playMelody(melodyTwo, noteDurationsTwo, 11);
      startDetecting = true;
      timer = millis();
    }
    else if (m.text == "ud83dudc08") {
      PortionAmount = 1;
      bot.sendMessage(m.chat_id, "Food portion changed to 1", keyboard_one);
    }
    else if (m.text == "ud83dudc08 ud83dudc08") {
      PortionAmount = 2;
      bot.sendMessage(m.chat_id, "Food portion changed to 2", keyboard_one);
    }
    else if (m.text == "ud83dudc08 ud83dudc08 ud83dudc08") {
      PortionAmount = 3;
      bot.sendMessage(m.chat_id, "Food portion changed to 3", keyboard_one);
    }
    else bot.sendMessage(m.chat_id, "Hello !", keyboard_one);
  }

  if (startDetecting) {
    int value = analogRead(A0);
    delay(2000);
    float volt = value * ( 5/1024);
    //Serial.println(volt);
    Serial.println(value);
   // Serial.println(" Value from photosensor==" + value + analogRead(A0) );
    if (value < 200) {
      String TimeValue = String((millis() - timer) / 1000);
      bot.sendMessage(OldChatId, "Cat detected! \nTime to reach the feeder: " + TimeValue + " seconds", keyboard_one);
      startDetecting = false;
    }
    else if (millis() - timer > 120000) {
      bot.sendMessage(OldChatId, "No cat detected in the past two minutes", keyboard_one);
      startDetecting = false;
    }
  }
  //client.stop();
  delay(1000);
}

void moveServo() {
  Serial.println("moving servo");
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                     // waits 15ms for the servo to reach the position

  }
  delay(PortionAmount * 300);  // keep the box open for a time interval based on the amount of food you want to deliver
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                     // waits 15ms for the servo to reach the position
  }
}

void playMelody(int melody[], int noteDurations[], int numberOfNotes ) {
  Serial.println("Playing melody");

  for (int thisNote = 0; thisNote < numberOfNotes; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzerPin);
  }

}
