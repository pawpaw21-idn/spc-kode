#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Inisialisasi LCD I2C: address 0x27, 20 kolom, 4 baris
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo myservo;

// Pin sensor
#define ir_enter 2
#define ir_exit 4
#define ir_car1 A0
#define ir_car2 A1
#define ir_car3 A2
#define ir_car4 A3
#define ir_car5 A4
#define ir_car6 A5

int S1 = 0, S2 = 0, S3 = 0, S4 = 0, S5 = 0, S6 = 0;
int flag1 = 0, flag2 = 0;
int slot = 6;

void setup() {
  Serial.begin(9600);

  pinMode(ir_enter, INPUT);
  pinMode(ir_exit, INPUT);
  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);
  pinMode(ir_car5, INPUT);
  pinMode(ir_car6, INPUT);

  myservo.attach(3);
  myservo.write(0); // posisi awal tertutup

  lcd.begin(A4, A5);
  lcd.backlight(); // hidupkan backlight
  lcd.setCursor(2, 0);
  lcd.print("Car Parking");
  lcd.setCursor(2, 1);
  lcd.print("System OK");
  delay(2000);
  lcd.clear();

  readSensors();
  int total = S1 + S2 + S3 + S4 + S5 + S6;
  slot = 6 - total;
}

void loop() {
  readSensors();

  lcd.setCursor(0, 0);
  lcd.print("Slot Tersedia: ");
  lcd.print(slot);
  lcd.print("   "); // hapus sisa karakter

  // Tampilkan status sensor di Serial Monitor
  Serial.print("S1:"); Serial.print(S1);
  Serial.print(" S2:"); Serial.print(S2);
  Serial.print(" S3:"); Serial.print(S3);
  Serial.print(" S4:"); Serial.print(S4);
  Serial.print(" S5:"); Serial.print(S5);
  Serial.print(" S6:"); Serial.println(S6);

  // Mobil masuk
  if (digitalRead(ir_enter) == LOW && flag1 == 0) {
    if (slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(90); // buka
        slot = slot - 1;
      }
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Parkir Penuh        ");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print("                    ");
    }
  }

  // Mobil keluar
  if (digitalRead(ir_exit) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(90); // buka keluar
      slot = slot + 1;
    }
  }

  // Reset posisi servo
  if (flag1 == 1 || flag2 == 1) {
    delay(1500);
    myservo.write(0); // tutup kembali
    flag1 = 0;
    flag2 = 0;
  }

  delay(2000);
}

void readSensors() {
  S1 = digitalRead(ir_car1) == LOW ? 1 : 0;
  S2 = digitalRead(ir_car2) == LOW ? 1 : 0;
  S3 = digitalRead(ir_car3) == LOW ? 1 : 0;
  S4 = digitalRead(ir_car4) == LOW ? 1 : 0;
  S5 = digitalRead(ir_car5) == LOW ? 1 : 0;
  S6 = digitalRead(ir_car6) == LOW ? 1 : 0;
}
