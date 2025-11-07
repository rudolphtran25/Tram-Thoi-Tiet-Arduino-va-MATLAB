/*
 * Code Arduino cho Trạm thời tiết ARDUINO và MATLAB
 * Kết nối:
 * - DHT22 DATA pin -> Arduino Pin A3
 * - I2C LCD SCL -> Arduino Pin A5
 * - I2C LCD SDA -> Arduino Pin A4
 * - Cấp nguồn 5V, 3.3V và GND cho các module.
 */

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Cấu hình ---
#define DHTPIN A3      // Pin kết nối với chân DATA của DHT22 (đã đổi sang A3)
#define DHTTYPE DHT22  // Loại cảm biến là DHT22
#define LCD_ADD 0x27   // Địa chỉ của màn hình LCD I2C: 0x27 hoặc 0x3F

// --- Khởi tạo thiết bị ---
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(LCD_ADD, 16, 2); // (Địa chỉ I2C, Số cột, Số hàng)

// --- Thiết lập thuộc tính ---
const unsigned long interval = 2000; // Đọc cảm biến mỗi 2 giây (DHT22 cần ít nhất 2s)
unsigned long lastRead = 0;

void setup() {
  // Khởi động giao tiếp Serial ở baud rate 9600
  Serial.begin(9600);
  
  // --- Khởi động LCD ---
  lcd.init();
  lcd.backlight();

  // --- Thiết lập màn hình khởi động ---
  lcd.setCursor(5, 0);
  lcd.print("Nhom 3");
  lcd.setCursor(0, 1);
  lcd.print("Khoi dong du an.");

  delay(3000);
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print("Tram Thoi Tiet");
  lcd.setCursor(0, 1);
  lcd.print("Dang cap nhat...");

  // Khởi động cảm biến DHT
  dht.begin();
  
  delay(3000); // Chờ khởi động
  lcd.clear();
}

void loop() {
  // Sử dụng millis() để đọc không bị trễ (non-blocking)
  unsigned long currentRead = millis();

  if (currentRead - lastRead >= interval) {
    lastRead = currentRead;

    // Đọc độ ẩm (Humidity)
    float h = dht.readHumidity();
    // Đọc nhiệt độ (Temperature)
    float t = dht.readTemperature(); // Mặc định là độ C

    // Kiểm tra nếu đọc lỗi (ví dụ: rút cảm biến)
    if (isnan(h) || isnan(t)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Loi doc cam bien");
      Serial.println("Loi: Khong the doc du lieu tu cam bien DHT!");
      return;
    }

    // --- 1. Hiển thị lên LCD ---
    lcd.clear();
    
    // Hàng 1: Nhiệt độ
    lcd.setCursor(0, 0);
    lcd.print("Nhiet do: ");
    lcd.print(t, 1); // In 1 số sau dấu phẩy
    lcd.print(" C");

    // Hàng 2: Độ ẩm
    lcd.setCursor(0, 1);
    lcd.print("Do am:    ");
    lcd.print(h, 1); // In 1 số sau dấu phẩy
    lcd.print(" %");

    // --- 2. Gửi dữ liệu qua Serial cho MATLAB ---
    // Gửi theo định dạng "T:xx.x,H:yy.y" để dễ dàng xử lý
    Serial.print("T:");
    Serial.print(t, 1);
    Serial.print(",H:");
    Serial.print(h, 1);
    Serial.println(); // Ký tự xuống dòng để MATLAB biết kết thúc 1 gói tin
  }
}