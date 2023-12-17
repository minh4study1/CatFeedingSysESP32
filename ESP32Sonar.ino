#include <NewPing.h>

#define TRIGGER_PIN 14
#define ECHO_PIN    12
#define OUT_PIN     13 // Chân GPIO kết nối với chân OUT của cảm biến

NewPing sonar(TRIGGER_PIN, ECHO_PIN);
int outPinState;

const int BUZZER_PIN = 16; // Chân GPIO kết nối với còi

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Các thiết lập khác ở đây
}

void loop() {
  // Đọc giá trị từ chân OUT
  outPinState = digitalRead(OUT_PIN);

  // Đọc giá trị khoảng cách từ cảm biến siêu âm
  int distance = sonar.ping_cm();

  // In ra giá trị khoảng cách và trạng thái chân OUT
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, OUT Pin State: ");
  Serial.println(outPinState);

  // Kiểm tra nếu khoảng cách dưới 5cm và trạng thái của chân OUT là HIGH
  if (distance < 5 && outPinState == HIGH) {
    // Thực hiện hành động khi đạt điều kiện

    // Bật còi
    digitalWrite(BUZZER_PIN, HIGH);

    // Hoặc thực hiện các hành động khác theo yêu cầu của bạn
  } else {
    // Tắt còi
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(500); // Đợi 500ms giữa các lần đo
}
