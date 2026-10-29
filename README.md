#  Finger Gesture Recognition

Hệ thống nhận diện cử chỉ ngón tay được phát triển trên vi điều khiển STM32F103C8T6 . Sử dụng cảm biến uốn cong Flex Sensor kết hợp với màn hình OLED SSD1306 để nhận diện và hiển thị các cử chỉ ngón tay theo thời gian thực.

- Bare-metal Register Programming: Tự xây dựng các module driver cho **RCC/Clock**, **GPIO**, **ADC1**, **TIMER2**, **I2C1** và **OLED SSD1306**.

#### Bảng tóm tắt ngoại vi và thanh ghi trọng tâm

| Ngoại vi | Thanh ghi chính | Vai trò & Chức năng trong hệ thống |
| :--- | :--- | :--- |
| **RCC** (Clock) | `RCC_CR`, `RCC_CFGR`, `RCC_APB1ENR`, `RCC_APB2ENR` | Cấu hình xung nhịp hệ thống (HSE 8MHz -> PLL 72MHz), bật clock cho GPIOA, GPIOB, ADC1, TIMER2, I2C1. |
| **GPIO** | `GPIOx_CRL`, `GPIOx_CRH`, `GPIOx_IDR`, `GPIOx_ODR` | Cấu hình chế độ chân (Analog, Out PP, AF OD) và thao tác ghi/đọc mức logic. |
| **ADC1** | `ADC_CR1`, `ADC_CR2`, `ADC_SMPR2`, `ADC_SQR3`, `ADC_DR`, `ADC_SR` | Đọc tín hiệu Analog từ Flex Sensor trên kênh 0 (PA0), hiệu chuẩn (Calibration) và thực hiện chuyển đổi liên tục. |
| **TIMER2** | `TIMx_CR1`, `TIMx_PSC`, `TIMx_ARR`, `TIMx_DIER`, `TIMx_SR` | Tạo định thời cơ sở (1µs/tick), phát ngắt Update Interrupt định kỳ để đồng bộ trạng thái hệ thống. |
| **I2C1** (Hardware) | `I2C_CR1`, `I2C_CR2`, `I2C_CCR`, `I2C_TRISE`, `I2C_DR`, `I2C_SR1`, `I2C_SR2` | Giao tiếp phần cứng với màn hình OLED SSD1306 ở chế độ Fast Mode (400kHz). |
| **NVIC** | `ISER`, `IP` | Quản lý và cho phép ngắt định thời TIMER2 (`TIM2_IRQn`). |

---


####  Build & Flash

#### Yêu cầu phần mềm & phần cứng:
- **Phần mềm**: Keil uVision 
- **Mạch nạp**: ST-Link V2
- **Phần cứng**: Mạch STM32F103C8T6 Blue Pill.

#### Các bước thực hiện:

1. **Mở Project**:
   - Khởi động Keil uVision.
   - Chọn `File` -> `Open...` và mở file project `Final_term.uvprojx`.

2. **Cấu hình Target & Compiler**:
   - Đảm bảo Target được chọn là `Target 1` với vi điều khiển `STM32F103C8`.
   - Vào `Options for Target...` (Phím tắt `Alt + F7`):
     - Mục **Device**: Chọn `STM32F103C8`.
     - Mục **Target**: Xung nhịp `8.0 MHz`.
     - Mục **Debug**: Chọn `ST-Link Debugger` (hoặc `J-LINK / J-TRACE`).

3. **Biên dịch Dự án (Build)**:
   - Nhấn phím `F7` hoặc chọn biểu tượng **Build** trên thanh công cụ.
   - Kiểm tra kết quả biên dịch ở cửa sổ *Build Output* (đảm bảo `0 Error(s), 0 Warning(s)`).

4. **Nạp Code vào STM32 (Flash/Download)**:
   - Kết nối ST-Link với STM32F103C8T6 (`3V3`, `GND`, `SWDIO`, `SWCLK`).
   - Nhấn phím `F8` hoặc chọn biểu tượng **LOAD** (Download) để nạp file `.hex`/`.axf` xuống vi điều khiển.
   - Nhấn nút **RESET** trên board STM32 để khởi chạy chương trình.
