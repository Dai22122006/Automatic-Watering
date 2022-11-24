// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLQksnicz4"
#define BLYNK_DEVICE_NAME "led2"

#define BLYNK_FIRMWARE_VERSION  "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

#define led1 14 //D5



int relay =  D5; // Chân relay số 5
int cb = A0;   // Chân cảm biến ở chân Analog: A0
int doc_cb, TBcb, doAm, phanTramAo, phanTramThuc;


BLYNK_WRITE(V0){
    int value = param.asInt(); // đọc độ ẩm cần thiết từ app về mạch
    doAm = value; 
}


BLYNK_WRITE(V2){
    int value = param.asInt(); // đọc bat tat từ app về mạch
    if (value == 1) value = 0;
    else value = 1;
    digitalWrite(relay, value);
    delay(5000);
}


void setup()
{
  Serial.begin(115200);
  delay(100);
  pinMode(led1, OUTPUT);
  pinMode(cb, OUTPUT);
  digitalWrite(relay, LOW);
  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
   for (int i = 0; i <= 9; i++) /*Chúng ta sẽ tạo một hàm for để đọc 10 lần giá trị cảm biến,
                                 sau đó lấy giá trị trung bình để được giá trị chính xác nhất.*/
    {
        doc_cb += analogRead(cb);
    }
    TBcb = doc_cb / 10; // Tính giá trị trung bình

    Serial.println(TBcb);
    phanTramAo = map(TBcb, 0, 1023, 0, 100); // Chuyển giá trị Analog thành giá trị %
    phanTramThuc = 100 - phanTramAo;         // Tính giá trị phần trăm thực, chuyển điện thế khô thành ẩm


    Serial.print("analog: ");
    Serial.print(TBcb);
    Serial.print(" ~ ");
    Serial.print(phanTramThuc);
    Serial.println("%");
    delay(500);
    doc_cb = 0;


    Blynk.virtualWrite(V1, phanTramThuc); // gửi phần trăm thực lên app
    if (phanTramThuc >= doAm) // Nếu phần trăm thực >= độ ẩm
    {
        digitalWrite(relay, 1); // Thì relay OFF, không bơm
    }
    else // Ngược lại, phần trăm thực < độ ẩm
    {
        digitalWrite(relay, 0); // Thì relay ON, Bơm chạy
        delay(5000);
    }
}

