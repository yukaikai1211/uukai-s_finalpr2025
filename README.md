# 🔐 Smart Door Lock with Telegram Notification (ESP32)

本專案為一套基於 ESP32 的智慧門鎖系統，結合 4x4 鍵盤、I2C LCD、伺服馬達與 Telegram Bot，可透過密碼解鎖並即時通知使用者。已整合 Wokwi 模擬環境，方便雲端模擬與測試。
恩，大一通識課報告用...

---

## 📦 功能特色

- ✅ 4 位數 PIN 密碼輸入解鎖
- ✅ 按 `*` 可重新上鎖
- ✅ 按 `C` 可刪除輸入
- ✅ 正確 / 錯誤 PIN 將即時推播至 Telegram
- ✅ 開機上線時自動通知
- ✅ 使用 ESP32 與常見元件可即時部署

---

## 🛠 使用元件

| 元件名稱         | 規格與功能             |
|------------------|------------------------|
| ESP32 DevKit     | 控制核心               |
| 4x4 Keypad       | 密碼輸入               |
| 16x2 I2C LCD     | 顯示狀態（SDA: 21, SCL: 22） |
| Servo Motor      | 控制門鎖機構（PWM: D14）   |
| WiFi             | 連線傳送 Telegram 訊息 |

---

## 🚀 操作方式

1. 啟動後 LCD 顯示「Door Locked」
2. 輸入 PIN（預設為 `1234`），按 `#` 解鎖
3. LCD 顯示「Door Unlocked」，Telegram 推播
4. 按 `*` 可重新上鎖
5. 輸入錯誤 PIN 將顯示錯誤並傳送警告訊息至 Telegram
6. 按 `C` 可清除一位輸入

---

## 🌐 Telegram 設定

請修改 `sketch.ino` 中以下變數：
```cpp
const char* BOT_TOKEN = "Your_Bot_Token";
const char* USER_ID = "Your_Chat_ID";
