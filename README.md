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

- Arduino Uno
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
     ```arduino
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

---

## 5. Mô tả mã nguồn

### 5.1. Arduino (`TramThoiTiet_DHT22.ino`)

- Khởi tạo:
  - Cảm biến `DHT22` tại chân `A3` (`DHTPIN A3`, `DHTTYPE DHT22`).
  - LCD I2C 16x2 tại địa chỉ `0x27` (có thể đổi `0x3F` nếu module khác).
  - Kênh Serial: `Serial.begin(9600)`.
- Chu kỳ đọc: mỗi **2 giây**:
  - Đọc `t` (nhiệt độ °C) và `h` (độ ẩm %).
  - Nếu đọc lỗi (`isnan(t)` hoặc `isnan(h)`):
    - Hiển thị thông báo lỗi trên LCD.
    - In thông báo lỗi qua Serial.
  - Nếu hợp lệ:
     - Hiển thị lên LCD:
       - Dòng 1: `Nhiet do: t C`
       - Dòng 2: `Do am: h %`
     - Gửi dữ liệu cho MATLAB theo định dạng cố định:
       ```arduino
       T:<t>,H:<h>\n
       ```
       Ví dụ:
       ```arduino
       T:30.5,H:65.2
       ```
       
### 5.2. MATLAB Script (`DocDuLieuArduino.m`)

Script này dùng để **đọc và kiểm tra dữ liệu** từ Arduino qua cổng Serial, đồng thời là nền tảng logic cho ứng dụng GUI.

**Cách sử dụng:**

```matlab
DocDuLieuArduino();              % Chọn cổng COM & Baud Rate từ menu
DocDuLieuArduino("COM6", 9600);  % Chỉ định trực tiếp
```
**Chức năng chính:**
- Nếu không truyền tham số:
  - Liệt kê các cổng COM khả dụng bằng `serialportlist("available")`.
  - Cho người dùng chọn cổng COM.
  - Cho người dùng chọn Baud Rate từ danh sách: `9600`, `19200`, `38400`, `57600`, `115200`.
- Tạo đối tượng Serial:
  - `serialport(port, baud)` - Tạo kết nối Serial tới 'port' (ví dụ "COM6") với tốc độ 'baud' (ví dụ 9600)
  - `configureTerminator(sp, "CR/LF")` - Thiết lập ký tự kết thúc dòng là Carriage Return + Line Feed phải khớp với Serial.println() bên Arduino
  - `flush(sp)` trước khi bắt đầu đọc. - Xóa toàn bộ dữ liệu cũ còn tồn trong buffer trước khi bắt đầu đọc
- Vòng lặp đọc liên tục:
  - `sp.NumBytesAvailable` - Số byte có trong buffer để đọc
  - `readline(sp)` - Đọc 1 dòng dữ liệu đến ký tự terminator ("CR/LF"), ví dụ "T:30.5,H:65.2"
  - `data = sscanf(char(line), 'T:%f,H:%f')` - Đọc chuỗi theo định dạng trên
  - thêm lệnh tạm dừng 0.05s để tránh bị treo CPU - `pause(0.05)`
- Xử lý tài nguyên
  - Đảm bảo đóng cổng Serial khi dừng hoặc lỗi: `c = onCleanup(@() cleanup(sp))`
  - `cleanup(sp)` - Kiểm tra và xóa đối tượng serial để giải phóng cổng COM

**Mục đích:**
- Kiểm tra nhanh Arduino có gửi đúng định dạng T:<float>,H:<float> hay không.
- Logic parse chuỗi này được dùng lại cho MATLAB App Designer và các module khác.

### 5.3. MATLAB App (`TramThoiTietApp.mlapp`)

Ứng dụng được xây dựng bằng **MATLAB App Designer**, cung cấp giao diện đồ họa để giám sát trạm thời tiết theo thời gian thực.

**Thành phần chính:**

- Dropdown chọn **Cổng COM**
- Dropdown chọn **Baud Rate** (mặc định `9600`)
- Các nút:
  - **Làm mới**: quét lại danh sách cổng COM
  - **Kết nối**: mở kết nối Serial đến Arduino
  - **Ngắt kết nối**: đóng kết nối Serial
  - **Bắt đầu**: bắt đầu đọc và hiển thị dữ liệu
  - **Kết thúc**: dừng đọc dữ liệu
- Hai đồ thị:
  - Nhiệt độ (°C) theo thời gian
  - Độ ẩm (%) theo thời gian
- Hai ô hiển thị số:
  - Giá trị nhiệt độ hiện tại
  - Giá trị độ ẩm hiện tại
- Nhãn hiển thị **trạng thái kết nối**

**Logic hoạt động:**

- Khi nhấn **Kết nối**:
  - Ứng dụng tạo đối tượng `serialport` với COM & Baud được chọn
  - Cập nhật trạng thái: `Đã kết nối`
- Khi nhấn **Bắt đầu**:
  - Thiết lập `timer` hoặc callback định kỳ để đọc dữ liệu từ Serial
  - Mỗi lần nhận được chuỗi hợp lệ dạng `T:%f,H:%f`:
    - Cập nhật dãy dữ liệu trên đồ thị
    - Cập nhật giá trị số (nhiệt độ, độ ẩm) trên giao diện
- Khi nhấn **Kết thúc** hoặc **Ngắt kết nối**:
  - Dừng việc đọc dữ liệu
  - Đóng cổng Serial (xóa đối tượng)
  - Cập nhật trạng thái: `Chưa kết nối`

---

### 5.4. Simulink (`TramThoiTiet_DHT22.slx` - nếu sử dụng)

- Xây dựng mô hình Simulink mô phỏng/nhận dữ liệu từ cảm biến DHT22.

**Sử dụng:**

- Khối **IO Device Builder** hoặc khối tùy chỉnh để đọc DHT22.

**Đầu ra mô phỏng:**

- `NhietDo` (°C)
- `DoAm` (%)

**Ứng dụng:**

- Kết nối với Scope để quan sát
- Kết hợp với khối lọc, điều khiển, cảnh báo (ví dụ: điều khiển quạt theo nhiệt độ)

---

## 6. Thiết lập & Sử dụng

### 6.1. Nạp chương trình Arduino

- Mở file Arduino trong thư mục `arduino/` (ví dụ: `TramThoiTiet_DHT22.ino`) bằng Arduino IDE
- Chọn đúng **Board** và **cổng COM**
- Upload chương trình
- Kiểm tra:
  - LCD hiển thị màn hình khởi động
  - Sau đó hiển thị nhiệt độ và độ ẩm

---

### 6.2. Kiểm tra Serial bằng MATLAB Script

- Mở MATLAB
- Thêm thư mục project vào path:

  ```matlab
  addpath(genpath('TramThoiTiet_Arduino_MATLAB'));
  % addpath + genpath: thêm toàn bộ thư mục dự án (và các thư mục con)
  % để MATLAB có thể gọi được các file .m, .mlapp, .slx trong project
  ```
- Chạy:
  ```matlab
  DocDuLieuArduino();
  % Gọi script đọc dữ liệu từ Arduino để kiểm tra kết nối & định dạng
  ```
- Quan sát Command Windows: Giá trị đưa ra đúng định dạng → Kết nối và định dạng dữ liệu Arduino OK.
  
