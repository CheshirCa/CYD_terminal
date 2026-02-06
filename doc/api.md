# API Reference

## Terminal API

### Initialization
```cpp
void terminalInit(int baudRateIndex, int mode)
```
Initialize terminal with baud rate and mode.
- `baudRateIndex`: 0-5 (9600, 19200, 38400, 57600, 115200, 230400)
- `mode`: 0=USB, 1=External UART

### Input/Output
```cpp
void terminalUpdate()
```
Process incoming UART data. Call in main loop.

```cpp
void terminalSendText(const char* text)
```
Send UTF-8 text to UART with local echo.

```cpp
void terminalSendChar(char c)
```
Send single character to UART with local echo.

### Screen Control
```cpp
void terminalClear()
```
Clear screen buffer and reset cursor.

```cpp
void terminalRedraw()
```
Redraw visible portion of terminal buffer.

### Scrolling
```cpp
void terminalScroll(int delta)
```
Scroll terminal buffer.
- `delta > 0`: scroll up (back in history)
- `delta < 0`: scroll down (forward)

```cpp
void terminalScrollToBottom()
```
Reset scroll to most recent content.

```cpp
int terminalGetScrollOffset()
```
Get current scroll position (0 = bottom).

```cpp
int terminalGetMaxScroll()
```
Get maximum scroll offset.

```cpp
void terminalScrollForKeyboard(bool visible)
```
Adjust scroll when keyboard is shown/hidden.

---

## Display API

### Initialization
```cpp
void displayInit()
```
Initialize TFT display and touch controller.

### Touch
```cpp
bool getTouch(uint16_t *x, uint16_t *y)
```
Get touch coordinates. Returns `true` if screen is touched.
- `x`, `y`: pointers to store coordinates (0-319, 0-239)

---

## Keyboard API

### Control
```cpp
void showKeyboard()
```
Display on-screen keyboard at bottom of screen.

```cpp
void hideKeyboard()
```
Hide on-screen keyboard.

```cpp
void handleKeyboardTouch()
```
Process touch input for keyboard. Call when keyboard is visible.

---

## Sound API

### Initialization
```cpp
void soundInit()
```
Initialize I2S audio DAC.

### Control
```cpp
void soundSetEnabled(bool enabled)
```
Enable or disable sound output. Saves setting to NVS.

```cpp
bool soundIsEnabled()
```
Check if sound is enabled.

### Playback
```cpp
void soundPlayBell()
```
Play bell tone (0x07 character).

```cpp
void soundPlayClick()
```
Play keyboard click sound.

---

## WiFi API

### Initialization
```cpp
void wifiInit()
```
Initialize WiFi and web server.

```cpp
void wifiStart()
```
Start WiFi in configured mode.

```cpp
void wifiStop()
```
Stop WiFi and web server.

```cpp
void wifiUpdate()
```
Update WiFi status and handle reconnection. Call in main loop.

### Configuration
```cpp
void wifiSetMode(WiFiMode mode)
```
Set WiFi mode.
- `WIFI_MODE_OFF`: WiFi disabled
- `WIFI_MODE_AP`: Access Point mode
- `WIFI_MODE_CLIENT`: Station mode

```cpp
void wifiSetClientCredentials(const char* ssid, const char* password)
```
Set credentials for Client mode.

```cpp
void wifiSaveConfig()
```
Save WiFi configuration to NVS.

```cpp
void wifiLoadConfig()
```
Load WiFi configuration from NVS.

### Status
```cpp
WiFiMode wifiGetMode()
```
Get current WiFi mode.

```cpp
WiFiStatus wifiGetStatus()
```
Get WiFi connection status.
- `WIFI_STATUS_OFF`: WiFi disabled
- `WIFI_STATUS_AP`: AP mode active
- `WIFI_STATUS_CONNECTING`: Connecting to network
- `WIFI_STATUS_CONNECTED`: Connected
- `WIFI_STATUS_ERROR`: Connection error

```cpp
String wifiGetSSID()
```
Get current SSID (AP or connected network).

```cpp
String wifiGetIP()
```
Get IP address.

### WebSocket
```cpp
void wifiSendTerminalLine(const char* line)
```
Send terminal line to all connected WebSocket clients.

---

## UTF-8 API

### Decoder
```cpp
void utf8Init(UTF8Decoder* decoder)
```
Initialize UTF-8 decoder.

```cpp
bool utf8Decode(UTF8Decoder* decoder, uint8_t byte)
```
Decode UTF-8 byte. Returns `true` when character is complete.

```cpp
uint32_t utf8GetCodepoint(UTF8Decoder* decoder)
```
Get decoded Unicode codepoint.

### Character Rendering
```cpp
void drawUnicodeChar(uint32_t codepoint, int x, int y, 
                     uint16_t fgColor, uint16_t bgColor, int scale)
```
Draw Unicode character at screen position.
- `codepoint`: Unicode codepoint (U+0000 to U+FFFF)
- `x`, `y`: screen coordinates
- `fgColor`, `bgColor`: 16-bit RGB565 colors
- `scale`: font scale (1 = 6x8, 2 = 12x16)

### Cyrillic Support
```cpp
bool isCyrillic(uint32_t codepoint)
```
Check if codepoint is in Cyrillic range (U+0400-U+052F).

---

## Configuration

### config.h Defines

#### Hardware Pins
```cpp
#define KEY_PIN 0              // Boot button
#define BAT_ADC_PIN 34         // Battery ADC
#define RED_PIN 22             // RGB LED
#define GREEN_PIN 16
#define BLUE_PIN 17
#define AUDIO_EN 4             // Audio enable
#define I2S_BCLK 26            // I2S bit clock
#define I2S_LRC 25             // I2S L/R clock
#define I2S_DOUT 22            // I2S data out
#define SD_VSPI_SS 5           // SD card CS
#define SD_VSPI_MOSI 23
#define SD_VSPI_MISO 19
#define SD_VSPI_SCK 18
```

#### Terminal Settings
```cpp
#define TERMINAL_COLS 53       // Characters per line
#define TERMINAL_ROWS 27       // Visible rows
#define TERMINAL_BUFFER_ROWS 100  // Total buffer
#define TERMINAL_START_Y 22    // Y position below status bar
```

#### Sound Settings
```cpp
#define BELL_FREQ 1000         // Bell frequency (Hz)
#define BELL_DURATION 100      // Bell duration (ms)
#define CLICK_FREQ 2000        // Click frequency (Hz)
#define CLICK_DURATION 10      // Click duration (ms)
```

#### WiFi Settings
```cpp
#define WIFI_AP_SSID "CYD-Terminal"
#define WIFI_AP_PASS ""        // No password
#define WIFI_AP_IP "192.168.4.1"
#define WEB_SERVER_PORT 80
```

---

## Web Server Endpoints

### GET /
Main page with real-time terminal view via WebSocket.

### GET /logs
List of log files on SD card with download links.

### GET /download?file={filename}
Download specific log file.
- Parameter: `file` - filename from `/logs/` directory
- Response: Plain text file download

### WebSocket /ws
Real-time terminal output stream.
- Protocol: Text frames
- Direction: Server → Client only
- Format: UTF-8 text lines with `\n` terminator

---

## Data Structures

### UTF8Decoder
```cpp
struct UTF8Decoder {
  uint8_t state;
  uint32_t codepoint;
  uint8_t bytesNeeded;
  uint8_t bytesReceived;
};
```

### WiFiMode
```cpp
enum WiFiMode {
  WIFI_MODE_OFF = 0,
  WIFI_MODE_AP = 1,
  WIFI_MODE_CLIENT = 2
};
```

### WiFiStatus
```cpp
enum WiFiStatus {
  WIFI_STATUS_OFF = 0,
  WIFI_STATUS_AP = 1,
  WIFI_STATUS_CONNECTING = 2,
  WIFI_STATUS_CONNECTED = 3,
  WIFI_STATUS_ERROR = 4
};
```

---

## Extending the Code

### Adding New Keyboard Layouts
Edit `keyboard.cpp`:
```cpp
const char* keyboardNEW[] = {
  "layout_row_1",
  "layout_row_2",
  "layout_row_3"
};
```

Add to `KeyboardLayout` enum and `showKeyboard()` function.

### Adding New Escape Sequences
Edit `processEscSequence()` in `terminal.cpp`:
```cpp
case 'X': // Your new command
  // Handle command
  break;
```

### Custom Web Interface
Modify HTML in `wifiInit()` in `wifi_manager.cpp`.
WebSocket connection: `ws://[IP]/ws`

### Additional Fonts
Add font bitmaps to `utf8.cpp`:
```cpp
const uint8_t newFont6x8[][6] PROGMEM = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // char 0
  // ... more characters
};
```

Update `drawUnicodeChar()` to use new font.
