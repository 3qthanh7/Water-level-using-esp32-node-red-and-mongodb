# HỆ THỐNG ĐIỀU KHIỂN MỰC NƯỚC QUA SERVER MONGO REALM
# I. Giới thiệu
Đề tài "Hệ thống điều khiển mực nước qua server Mongo Realm" nhằm mục đích thiết kế và xây dựng một hệ thống cơ bản để quản lý mực nước trong bồn nước thông qua vi điều khiển ESP32 và cảm biến siêu âm, kết hợp với việc sử dụng Node-red để thiết kế giao diện điều khiển, cũng như việc áp dụng server Mongo Realm trong việc lưu trữ và xử lý dữ liệu.
Ngoài ra, hệ thống còn sử dụng bộ lọc Kalman cho cảm biến giúp giảm nhiễu gây ảnh hưởng kết quả đo.

<div align="center">
    <video src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/46b7c483-7c6e-49ce-8e28-08a6664b40be" alt="Demo video" height=200/>
</div>
        
<hr>

# II. Sơ đồ phần cứng
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/0a8dd02e-5b08-4e43-bd64-94c5d55d1b1f" alt="Sơ đồ khối tổng quát hệ thống" width="640"/></p>
<p align="center"><strong><em>Hình 1:</em></strong> Sơ đồ khối tổng quát hệ thống</p>

## 1.1.1	Khối động lực nước và nguồn
Chức năng : Lưu trữ và điều tiết lưu lượng nước ra vào hệ thống thông qua khối điều khiển và cấp nguồn cho cả hệ thống.
Linh kiện sử dụng : Nguồn điện cấp cho 2 động cơ và mạch buck cấp nguồn cho mạch điều khiển, 2 máy bơm và bồn chứa nước.

Thông số kĩ thuật :
-	Nguồn điện : Đầu vào 100-240 VAC. Đầu ra 12V 2A.
- Mạch buck : Đầu vào từ 4.5-40 VDC. Đầu ra 5VDC.
- Động cơ (máy bơm) : RS385 6-12VDC 1.5-2L/Min.
- Bồn chứa nước : Hình trụ cao có vạch chia mức nước.
## 1.1.2 Khối điều khiển
Chức năng : Thực thi lệnh điều khiển được gửi từ sever khối điều khiển lưu lượng nước, đo lường và thu thập dữ liệu biến động mực nước của bồn nước.
Linh kiện sử dụng : Vi điều khiển, cảm biến siêu âm.

Thông số kĩ thuật :
-	Vi điều khiển : Mạch thu phát WiFi BLE ESP-32 NodeMCU LuaNode32
-	Cảm biến siêu âm : Cảm biến siêu âm SRF04.
-	Mạch cầu H : Module L298, điện áp hoạt động 5V-35V, dòng điều khiển động cơ 2A, nhiệt độ hoạt động -20 tới +135, công suất đầu ra 25W.
# II. Thiết kế giao diện (Node-red)
Bao gồm 2 phần chính: thông số hệ thống và điều khiển hệ thống.
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/33977959-b638-48c7-8916-d1f0ab0a06ea" alt="Giao diện web app điều khiển và giám sát hệ thống" width="640"/></p>
<p align="center"><strong><em>Hình 2:</em></strong> Giao diện web app điều khiển và giám sát hệ thống</p>

## 2.1 Phần thông số hệ thống:
- Hiển thị mực nước hiện tại: thông tin này được lấy từ cảm biến siêu âm và truyền về cho server thông qua giao thức HTTP. Sau đó, thông tin này được hiển thị lên trang web.
- Biểu đồ thể hiện mực nước theo thời gian: thông tin về mực nước được lưu trữ trên server và được đồng bộ hóa liên tục với trang web để hiển thị dưới dạng biểu đồ. Điều này giúp người dùng có thể dễ dàng theo dõi sự thay đổi của mực nước trong thời gian.
- Hình SVG mô phỏng trạng thái bồn nước: Hình ảnh này được tạo bằng ngôn ngữ SVG và được liên kết đến thông số mực nước hiện tại. Hình ảnh này giúp người dùng có thể xem trực quan trạng thái bồn nước và mức nước hiện tại.
## 2.2 Phần điều khiển hệ thống:
Setpoint là tham số được truyền từ trang web đến server thông qua giao thức HTTP. Tham số này được sử dụng để điều khiển máy bơm vào và máy bơm ra. Khi mực nước thấp hơn setpoint, server sẽ gửi tín hiệu điều khiển máy bơm để bơm nước vào bồn. Ngược lại, khi mực nước vượt quá setpoint, server sẽ gửi tín hiệu điều khiển máy bơm để bơm nước ra khỏi bồn. Điều này giúp duy trì mực nước trong bồn ở mức ổn định.
## 2.3 Code Node-red
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/0f2bfaf9-0bc7-400f-9add-7509b1fa0177" alt="Lưu đồ giải thuật Node-red" width="540"/></p>
<p align="center"><strong><em>Hình 3:</em></strong> Lưu đồ giải thuật Node-red</p>
Trước khi import file JSON cần cài đặt các Palette sau:
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/d07dc896-211b-44e4-9e10-4434e6800b3a" alt="Các Palette cần cài đặt" width="540"/></p>
<p align="center"><strong><em>Hình 4:</em></strong> Các Palette cần cài đặt</p>
Import file  "SVG_Watertank.json" and Deloy
Hình ảnh Node-red sau import:
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/3275d334-be5c-45d5-a606-2978a068da8a" alt="Sơ đồ các khối trên Node-red" width="540"/></p>
<p align="center"><strong><em>Hình 5:</em></strong> Sơ đồ các khối trên Node-red</p>

# III. Lập trình phần cứng
## 3.1	Lưu đồ giải thuật trên ESP32
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/3e1d254a-769b-45ea-82d7-4d74d3f85173" alt="Lưu đồ giải thuật trên ESP32" width="540"/></p>
<p align="center"><strong><em>Hình 6:</em></strong> Lưu đồ giải thuật trên ESP32</p>

  ESP32 được kết nối với một mạng Wi-Fi và truyền nhận dữ liệu từ cơ sở dữ liệu MongoDB. Chương trình sử dụng cảm biến siêu âm HC-SR04 để đo mức nước trong bồn và điều khiển hai bơm nước, một để bơm nước vào bồn và một để bơm nước ra khỏi bồn. Chương trình bao gồm hai hàm chính Post() và Get() để gửi và nhận dữ liệu từ cơ sở dữ liệu MongoDB.

  Đầu tiên khai báo các thư viện:
  ```c
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <cmath>

  ```
  Khai báo các biến sử dụng:
```c
//Khai báo hai chân cảm biến
const int trigPin = 17;
const int echoPin = 16;
//Các biến chương trình
float level_M, level_Kal;
int CT;
// Các biến trạng thái và điều khiển máy bơm
bool PumpOut, PumpIn;
// PumpOut_2
const int EN_A = 12;
const int PumpOutPin1 = 14;
const int PumpOutPin2 = 27;
// PumpIn_1
const int EN_B = 13;
const int PumpInPin1 = 25;
const int PumpInPin2 = 26;
```
  Hàm hc_sr04() xử lí tín hiệu nhận được từ cảm biến:
  ```c
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
```
  
  Hàm Post() tạo một đối tượng JSON để đại diện cho dữ liệu cần gửi và gửi một yêu cầu POST đến URL được chỉ định. Dữ liệu bao gồm mức nước, trạng thái của máy bơm bơm nước vào và trạng thái của máy bơm bơm nước ra khỏi bồn. Hàm sử dụng thư viện HTTPClient để xử lý các yêu cầu và phản hồi HTTP.
```c
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
```
  
  Hàm Get() gửi một yêu cầu GET đến một URL khác để nhận tín hiệu điều khiển từ cơ sở dữ liệu. Hàm đặt giá trị của biến CT dựa trên phản hồi nhận được. Biến CT được sử dụng để điều khiển các máy bơm.
```c
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
```
  
  Hàm setup() khởi tạo giao tiếp serial, thiết lập các chân cho cảm biến siêu âm và các bơm và kết nối ESP32 với mạng Wi-Fi:
  ```c
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
  WiFi.begin("Xperia 1", "37200137");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Kết nối đến Wi-Fi...");
  }

  Serial.println("Đã kết nối đến Wi-Fi");
}
```
  
  Hàm loop() liên tục đo mức nước sử dụng cảm biến siêu âm và gửi dữ liệu đến cơ sở dữ liệu bằng cách sử dụng hàm Post(). Nó cũng nhận tín hiệu điều khiển từ cơ sở dữ liệu bằng cách sử dụng hàm Get() và điều khiển các bơm dựa trên giá trị của biến CT.
```c
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
```
# IV. Database - Sever
## 4.1 Lưu đồ giải thuật trên sever (Mongo Realm)
Gồm hai hàm chức năng : GET và POST

Chương trình hàm POST:
```js
exports = async function({ query, headers, body }, response) {
  const now = new Date();
  const timezoneOffset = 7;
  const vietnamTime = new Date(now.getTime() + (timezoneOffset * 60 * 60 * 1000));
  const data = JSON.parse(body.text());
  data.time = vietnamTime.toISOString();
  const level_m = data.Level_M;
  const get_level_kal = await context.services
.get("mongodb-atlas")
.db("MyData")
.collection("WaterTank_Data").find().sort({ _id: -1 }).skip(1).limit(1).toArray();
  if (get_level_kal.length > 0) {
    previous_level_kal = get_level_kal[0].Level_Kal;
  }
  else{
    previous_level_kal = 0;
  }
  var level_k = previous_level_kal + 0.5 * (level_m - previous_level_kal);
  data.Level_Kal = level_k;
  const result = context.services
.get("mongodb-atlas")
.db("MyData")
.collection("WaterTank_Data").insertOne(data);
  return result;
};

```
-	Hàm POST : có chức năng lưu một đối tượng mới lên server. Khi được gọi, hàm này sẽ nhận đầu vào là một đối tượng dưới dạng JSON, sau đó thực hiện việc lưu trữ đối tượng này vào cơ sở dữ liệu MongoDB. Trong quá trình lưu trữ, hàm này cũng sẽ kiểm tra tính hợp lệ của dữ liệu và thông báo kết quả về việc lưu trữ cho người dùng. Nếu lưu trữ thành công, hàm sẽ trả về một thông báo thành công cùng với đối tượng đã được lưu trữ. Nếu lưu trữ không thành công, hàm sẽ trả về một thông báo lỗi cùng với các thông tin về lỗi đã xảy ra. Sử dụng hàm POST, trước tiên phải truy cập vào MongoDB Realm bằng cách sử dụng đối tượng context.services và truy cập vào cơ sở dữ liệu "MyData" và bảng "WaterTank_Data" bằng phương thức .db() và .collection(). Sau đó, đối tượng JSON được phân tích từ chuỗi dữ liệu được truyền vào body.text(), và được chèn vào cơ sở dữ liệu bằng phương thức .insertOne(). Cuối cùng, kết quả trả về từ chức năng này là một đối tượng esp được chèn vào cơ sở dữ liệu, và nó sẽ được gửi trả về như một phản hồi (response) cho yêu cầu gốc. Trong hàm Post cũng tính toán quy đổi thời gian thực theo múi giờ GMT +7 để thuận tiện cho việc vẽ biểu đồ. Quá trình tính toán Kalman cũng được thực hiện tại đây, chương trình sẽ lấy giá trị Kalman đã tính được từ dữ liệu được lọc từ database trước đó nếu chưa có sẽ cho nó bằng 0 và giá trị đo được hiện tại.
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/786a54cc-fbb4-4cf9-94c7-d521926998a8" alt="Sơ đồ thuật toán hàm Post" width="540"/></p>
<p align="center"><strong><em>Hình 7:</em></strong> Sơ đồ thuật toán hàm Post</p>

Chương trình hàm GET:
```js
exports = async function({ query, headers, body }, response) {
  const GetSP = await context.services.get("mongodb-atlas").db("MyData").collection("WaterTank_SP").find().sort({ _id: -1 }).limit(1).toArray();
  const SetPoint = GetSP[0].SetPoint;
  const GetLevel = await context.services.get("mongodb-atlas").db("MyData").collection("WaterTank_Data").find().sort({ _id: -1 }).limit(1).toArray();
  const LevelValue = GetLevel[0].Level;
  var ER = SetPoint - LevelValue;
  const Offset = 0;
  var control;
  if (ER > Offset)
  { control = 1; // Bơm vào }
  else if (ER < Offset)
  {
    control = 2; // Bơm ra
  }
  else
  {
    control = 0; // Tắt
  }
  return control;
};
```

-	Hàm GET: được sử dụng để xử lý request được gửi đến địa chỉ endpoint. Function này nhận một đối tượng request, bao gồm các thông tin như query parameters, headers, body, và response object được sử dụng để trả về kết quả xử lý cho client. Trong function, đầu tiên nó truy vấn tới collection "WaterTank_SP" trong database "MyData" trên MongoDB Atlas để lấy giá trị SetPoint mới nhất được lưu trữ. Sau đó, function tiếp tục truy vấn đến collection "WaterTank_Data" để lấy giá trị Level mới nhất được lưu trữ. Tiếp theo, function tính toán độ chênh lệch giữa giá trị SetPoint và Level, lưu vào biến ER. Sau đó, nó kiểm tra xem độ chênh lệch có lớn hơn Offset hay không. Nếu lớn hơn, biến control được gán giá trị 1 để điều khiển bơm đưa nước vào.  Ngược lại, nếu nhỏ hơn, biến control được gán giá trị 2 để điều khiển bơm đưa nước ra. Trong trường hợp độ chênh lệch bằng 0, biến control được gán giá trị 0 để tắt bơm. Cuối cùng, function trả về giá trị control để sử dụng cho mục đích điều khiển bơm nước.
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/5d4fe13d-c4a2-4fbe-8493-2025131bbea4" alt="Sơ đồ thuật toán hàm Get" width="540"/></p>
<p align="center"><strong><em>Hình 8:</em></strong> Sơ đồ thuật toán hàm Get</p>

### 4.2 Database
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/993b74a9-0054-4261-9d7a-49a215d97bb1" alt="Database trên Monggo Atlas" width="540"/></p>
<p align="center"><strong><em>Hình 9:</em></strong> Database trên Monggo Atlas</p>
<p align="center"><img src="https://github.com/3qthanh7/Water-level-using-esp32-node-red-and-mongodb/assets/102430232/2d6555ff-6afb-401e-9cd5-b973bc54f21c" alt="Biểu đồ từ dữ liệu trên database" width="540"/></p>
<p align="center"><strong><em>Hình 10:</em></strong> Biểu đồ từ dữ liệu trên database</p>
