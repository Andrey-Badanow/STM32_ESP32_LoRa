# STM32_ESP32_LoRa   

Железо
ESP32: TTGO LoRa32 V2.1 (WiFi + LoRa SX1278 868MHz)
STM32: STM32F411CEU6 Black Pill
LoRa модуль: E22-900M22S (SX1278 868MHz)
USB-UART: CH340E

Суть
ESP32 берет нужные данные из интернета (в моём случае погоду) → отправляет по LoRa → STM32 принимает и 
обрабатывает в соответствии с некой своей логикой → передает на компьютер через UART (CH340)

ПОДКЛЮЧЕНИЕ:
  STM32F411CEU6 к CH340E:
      A9(USART1_TX) -> RXD(CH340E)
      A10(USART1_RX) -> TXD(CH340E)
      GND->GND
      3,3V - 3,3 (Если нужно)
