# TRẠM THỜI TIẾT ARDUINO VÀ MATLAB

## 1. Giới thiệu

Đề tài xây dựng **trạm thời tiết mini** sử dụng cảm biến **DHT22** để đo nhiệt độ và độ ẩm, hiển thị trên:

- Màn hình LCD I2C gắn với Arduino
- Ứng dụng **MATLAB App Designer** (GUI)
- Mô hình **Simulink** (IO Device Builder / DHT22)

Dữ liệu được truyền **theo thời gian thực** từ Arduino sang MATLAB qua cổng Serial, phục vụ giám sát và mô phỏng.

---

## 2. Chức năng chính

- Đo và hiển thị **nhiệt độ (°C)**, **độ ẩm (%)** từ cảm biến DHT22.
- Hiển thị giá trị trực tiếp trên **LCD 16x2 I2C**.
- Truyền dữ liệu sang MATLAB qua Serial theo thời gian thực.
- Giao diện MATLAB:
  - Chọn cổng COM và Baud Rate.
  - Kết nối / Ngắt kết nối Arduino.
  - Bắt đầu / Dừng đọc dữ liệu.
  - Vẽ **2 đồ thị thời gian thực** (nhiệt độ, độ ẩm).
  - Hiển thị **giá trị số hiện tại**.
- Mô hình Simulink:
  - Nhận 2 tín hiệu: `NhietDo`, `DoAm`.
  - Có thể dùng để phân tích, lọc, hoặc điều khiển.

---

## 3. Kiến trúc hệ thống

### 3.1. Phần cứng

- Arduino Uno (hoặc tương đương)
- Cảm biến DHT22
- LCD I2C 16x2 (địa chỉ 0x27 hoặc 0x3F)
- Dây nối, nguồn 5V và GND

**Kết nối:**

- DHT22:
  - `DATA` → `A3`
  - `VCC` → `3.3V`
  - `GND → GND`
- LCD I2C:
  - `SCL` → `A5`
  - `SDA` → `A4`
  - `VCC` → `5V`
  - `GND` → `GND`

### 3.2. Phần mềm

1. **Arduino**
   - Đọc cảm biến DHT22 mỗi 2 giây.
   - Hiển thị nhiệt độ và độ ẩm lên LCD.
   - Gửi dữ liệu qua Serial theo định dạng:
     ```text
     T:<nhiet_do>,H:<do_am>\n
     ```
2. **MATLAB Script (`DocDuLieuArduino.m`)**
   - Kết nối tới Arduino qua Serial.
   - Đọc chuỗi dữ liệu, tách giá trị T & H.
   - In kết quả ra Command Window (dùng để test/kiểm tra).

3. **MATLAB App Designer (Giao_Dien_Tram_Du_Bao.mlapp)**
   - Giao diện đồ họa như thiết kế:
     - Dropdown chọn COM.
     - Dropdown chọn Baud Rate.
     - Nút: Làm mới, Kết nối, Ngắt kết nối, Bắt đầu, Kết thúc.
     - Hai đồ thị: Nhiệt độ, Độ ẩm.
     - Hai ô hiển thị giá trị T & H.
     - Nhãn trạng thái kết nối.
   - Logic đọc & xử lý tương tự `DocDuLieuArduino.m` nhưng cập nhật lên GUI.
     
4. **Simulink (Simulink_Hardware.slx)**
   - Khối DHT22/IO Device Builder có 2 đầu ra:
     - `NhietDo`
     - `DoAm`
   - Kết nối tới Scope hoặc các khối xử lý khác.

---

## 4. Cấu trúc thư mục

```text
TramThoiTiet_Arduino_MATLAB/
│
├─ ADD-ONS/
│   ├─ arduino.mlpkginstall
│   └─ arduinoio.mlpkginstall
|
├─ CODE/
│   ├─ APP_DESIGN
|   |   └─ Giao_Dien_Tram_Du_Bao.mlapp   
│   ├─ ARDUINO
|   |   └─ Tram_Thoi_Tiet
|   |       └─ Tram_Thoi_Tiet.ino
│   ├─ MATLAB
|   |   └─ DocDuLieuArduino.m
│   └─ SIMULINK
|       ├─ DHT22
|       └─ Simulink_Hardware.slx
│
├─ DOCS/
│   ├─ Bao cao Tram Thoi tiet Arduino va MATLAB - Nhom 3
│   └─ Phan_chia_cong_viec.xlsx
│
├─ LIBRARY/
│   ├─ FAdafruit_Sensor-1.1.15.zip
│   ├─ DHT-sensor-library-1.4.6.zip
│   └─ LiquidCrystal_I2C-1.1.3.zip
│
└─ README.md
```
