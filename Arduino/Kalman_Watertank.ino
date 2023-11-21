#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <cmath>

//float x = 0;
const int trigPin = 17;
const int echoPin = 16;
float level_M, level_Kal;
int CT;
bool PumpOut, PumpIn;
// PumpOut_2
const int EN_A = 12;
const int PumpOutPin1 = 14;
const int PumpOutPin2 = 27;
// PumpIn_1
const int EN_B = 13;
const int PumpInPin1 = 25;
const int PumpInPin2 = 26;
/*
#define SENSOR  23
#define LIGHT_VALVE_1 22
#define LIGHT_VALVE_2 21
#define BUZZER 33
*/
float hc_sr04()
{
  unsigned long t ;// bien do tg
  float  R; // bien luu khoan cach

  digitalWrite(trigPin,0);// tat chan phat 
  delayMicroseconds(2);// delay 2 m1cro giay
  digitalWrite(trigPin,1);// bat chan phat
  delayMicroseconds(5);// delay 5 m1cro giay
  digitalWrite(trigPin,0);// tat chan phat 
  t = pulseIn(echoPin, HIGH); // pulsein : hàm đo độ rộng xung

  R = (t/2/29.412);

  return R;
}
void Post() {
  // Tạo object JSON
  DynamicJsonDocument doc(1024); // Kích thước JSON
  doc["Level_M"] = level_M;
  doc["PumpIn"] = PumpIn;
  doc["PumpOut"] = PumpOut;
  // Chuyển đổi object JSON thành chuỗi JSON
  String json;
  serializeJson(doc, json);
  Serial.println(json);
  HTTPClient http;
  // POST URL
  http.begin("https://asia-south1.gcp.data.mongodb-api.com/app/watertank_1-hnafs/endpoint/WT_Post_Data_Kal");
  http.addHeader("Content-Type", "application/json");
  // POST request
  int httpResponseCode = http.POST(json);
  // Nếu phản hồi thành công in Serial Monitor
  if (httpResponseCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.println("Lỗi trong quá trình thực hiện POST request");
  }
  // Đóng kết nối HTTP sau khi hoàn thành yêu cầu
  http.end();
}
void Get() {
  HTTPClient http;
  // Thực hiện GET request đến URL
  http.begin("https://asia-south1.gcp.data.mongodb-api.com/app/watertank_1-hnafs/endpoint/WT_Get_Control");
  // Gửi yêu cầu GET và đợi phản hồi từ máy chủ
  int httpResponseCode = http.GET();
  // Nếu phản hồi thành công in ra Serial Monitor
  if (httpResponseCode == HTTP_CODE_OK) {
    CT = http.getString().toInt();
    Serial.println("Điều khiển: ");
    Serial.println(CT);
  }
  else {
    Serial.println("Lỗi trong quá trình thực hiện GET request");
  }
  // Đóng kết nối HTTP sau khi hoàn thành yêu cầu
  http.end();
}
void setup()
{
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(EN_A,OUTPUT);
  pinMode(EN_B,OUTPUT);
  pinMode(PumpOutPin1, OUTPUT);
  pinMode(PumpOutPin2, OUTPUT);
  pinMode(PumpInPin1, OUTPUT);
  pinMode(PumpInPin2, OUTPUT);


  // Kết nối ESP32 với mạng Wi-Fi
  //WiFi.begin("D903@TDTU", "D903@TDTU");
  WiFi.begin("Xperia 1", "37200137");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Kết nối đến Wi-Fi...");
  }

  Serial.println("Đã kết nối đến Wi-Fi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    level_M = 34.5 - hc_sr04();
    Serial.printf("Mực nước đo được: ");
    Serial.println(level_M);
    Post();
    Get();
    // Xử lí điều khiển
  switch(CT) {
  case 2:
    PumpOut= 1;
    PumpIn= 0;
    Serial.println("Bơm ra");
    digitalWrite(PumpOutPin1, HIGH);
	  digitalWrite(PumpOutPin2, LOW);
    analogWrite(EN_A,210);
    digitalWrite(PumpInPin1, LOW);
	  digitalWrite(PumpInPin2, LOW);
    delay(1000);
    break;
  case 1:
    PumpOut= 0;
    PumpIn= 1;    
    Serial.println("Bơm vào");
    digitalWrite(PumpInPin1, HIGH);
	  digitalWrite(PumpInPin2, LOW);
    analogWrite(EN_B,240);
    digitalWrite(PumpOutPin1, LOW);
	  digitalWrite(PumpOutPin2, LOW);
    break;
  case 0:
    PumpOut= 0;
    PumpIn= 0;
    Serial.println("Tắt máy bơm");
    digitalWrite(PumpOutPin1, LOW);
	  digitalWrite(PumpOutPin2, LOW);
    digitalWrite(PumpInPin1, LOW);
	  digitalWrite(PumpInPin2, LOW);
    break;
  default:
    Serial.println("Lỗi tín hiệu");
    break;
}
  }
  // Chờ 2 giây trước khi thực hiện lại
  delay(2000);
}