#include <TelegramKeyboard.h>
#include <TelegramBot.h>

#include <WiFi101.h> 
#include <SPI.h>  
#include <TelegramBot.h>  
 
// Initialize Wifi connection to the router  
char ssid[] = "cupita_wifi";             // your network SSID (name)  
char pass[] = "dianetica";            // your network key 
// Initialize Telegram BOT  
const char* BotToken = "1189296909:AAGBCswxMgC2vu_ZSAwVmGs6-9uy_goNLu4";    // your Bot Teken  
WiFiSSLClient client;  
TelegramBot bot(BotToken,client); 

 
const int ledPin = 6;  // the number of the LED pin  
void setup() 
{  
 Serial.begin(115200);  
 while (!Serial) {}  //Start running when the serial is open 
 delay(3000);  
 // attempt to connect to Wifi network:  
 Serial.print("Connecting Wifi: ");  
 Serial.println(ssid);  
 while (WiFi.begin(ssid, pass) != WL_CONNECTED) 
       {  
   Serial.print(".");  
   delay(500);  
 }  
 Serial.println("");  
 Serial.println("WiFi connected");  
 bot.begin();  
 pinMode(ledPin, OUTPUT);  
TelegramKeyboard keyboard_one; 
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
 

}  


void loop() 
{  
 message m = bot.getUpdates(); // Read new messages

 
 if (m.text.equals("On")) 
       {  
   digitalWrite(ledPin, HIGH);  
   Serial.println("message received");  
   bot.sendMessage(m.chat_id, "The Led is now ON");  
 }  
 else if (m.text.equals("Off")) 
       {  
   digitalWrite(ledPin, LOW);  
   Serial.println("message received");  
   bot.sendMessage(m.chat_id, "The Led is now OFF");  
 }  
 
}  
