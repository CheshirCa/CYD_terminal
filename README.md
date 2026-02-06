# CYD Terminal

![CYD Terminal](https://github.com/CheshirCa/CYD_terminal/blob/main/doc/1.jpg)
![CYD Terminal](https://github.com/CheshirCa/CYD_terminal/blob/main/doc/2.jpg)
![CYD Terminal](https://github.com/CheshirCa/CYD_terminal/blob/main/doc/3.jpg)

UART terminal with VT100/ANSI escape sequences support for ESP32 CYD (Cheap Yellow Display).

## Features

- **Serial Terminal**: USB or external UART (GPIO3/1) with configurable baud rates (9600-230400)
- **Display**: 320x240 touchscreen with UTF-8 support (English, Russian Cyrillic)
- **On-screen Keyboard**: Multi-language keyboard (EN/RU/Symbols) with shift and layout switching
- **Scrollback Buffer**: 100-line circular buffer with touch scrolling
- **Sound**: I2S DAC audio for bell character (0x07) and keyboard clicks
- **WiFi**: AP or Client mode with web-based terminal viewer
- **WebSocket**: Real-time terminal streaming to web browser
- **Data Logging**: Automatic logging to MicroSD with web download interface
- **VT100/ANSI**: Basic escape sequences (colors, cursor positioning, screen clear)

## Hardware

**Board**: ESP32-2432S028R (CYD FNK0103L_3P2)
- **Display**: ST7789 3.2" 320x240 IPS
- **Touch**: XPT2046 resistive touchscreen
- **Audio**: I2S DAC output (GPIO 4, 26, 25, 22)
- **SD Card**: MicroSD slot (VSPI: GPIO 5, 23, 19, 18)
- **UART**: USB (default) or external (GPIO 3 RX, GPIO 1 TX)
- **LED**: RGB LED for status indication

## Requirements

### Hardware
- ESP32 CYD board (FNK0103L_3P2)
- MicroSD card (optional, for logging)
- USB cable for programming/power

### Software
- Arduino IDE 2.x or PlatformIO
- ESP32 board support (v3.x)

### Libraries
```
TFT_eSPI
ESPAsyncWebServer
AsyncTCP
Preferences (built-in)
SD (built-in)
WiFi (built-in)
```

## Installation

### 1. TFT_eSPI Configuration
Copy `User_Setup_CYD.h` to `Arduino/libraries/TFT_eSPI/User_Setup_CYD.h`

Edit `Arduino/libraries/TFT_eSPI/User_Setup_Select.h`:
```cpp
#include <User_Setup_CYD.h>
```

### 2. Upload Code
1. Open `CYD_Terminal.ino` in Arduino IDE
2. Select board: "ESP32 Dev Module"
3. Select port
4. Upload

### 3. Initial Setup
On first boot:
1. Select UART mode (USB or External)
2. Select baud rate
3. Press START button

## Usage

### Terminal Mode
- **BOOT button**: Toggle on-screen keyboard
- **Touch scroll**: Drag in terminal area to scroll through buffer
- **Keyboard**: Type characters, switch layouts (EN/RU/SYM)

### Status Bar Icons
```
[Baud Rate] [RX] [TX] [WiFi] [Mute] [KB] [Battery]
```

- **WiFi icon**: Tap to open WiFi settings
  - Gray: Off
  - Cyan: AP mode active
  - Yellow: Connecting
  - Green: Connected
  - Red: Error

- **Mute icon**: Tap to toggle sound (bell and keyboard clicks)
  - Green: Sound enabled
  - Red: Sound muted

- **Keyboard icon**: Tap to toggle on-screen keyboard
  - Green: Keyboard visible
  - Gray: Keyboard hidden

### WiFi Settings
1. Tap WiFi icon in status bar
2. Select mode:
   - **AP Mode**: Creates access point `CYD-Terminal` (no password)
   - **Client Mode**: Connect to existing WiFi network
3. For Client mode:
   - Enter SSID
   - Enter password
4. Tap SAVE (device will reboot)

### Web Interface
Connect to device IP address in browser:
- **AP Mode**: http://192.168.4.1
- **Client Mode**: Check serial output for IP

#### Web Features
- `/` - Real-time terminal view (WebSocket)
- `/logs` - Browse and download log files from SD card

### Keyboard Layouts
- **EN**: QWERTY English
- **RU**: ЙЦУКЕН Russian (Cyrillic)
- **SYM**: Numbers and symbols

**Special Keys**:
- SHIFT: Uppercase (single press)
- LANG: Switch between EN/RU
- SYM: Switch to/from symbols
- SPACE, BKSP, ENTER

### Escape Sequences
Supported ANSI/VT100 sequences:
```
ESC[H       - Cursor home
ESC[{y};{x}H - Set cursor position
ESC[2J      - Clear screen
ESC[K       - Clear line from cursor
ESC[{n}A    - Cursor up n lines
ESC[{n}B    - Cursor down n lines
ESC[{n}C    - Cursor forward n columns
ESC[{n}D    - Cursor back n columns
ESC[{n}m    - Set graphics mode (colors 30-37)
\x07        - Bell (plays sound)
```

## Configuration

### Baud Rates
Available: 9600, 19200, 38400, 57600, 115200, 230400

### Terminal Buffer
- **Columns**: 53 characters (6px font)
- **Visible rows**: 27 lines (8px font)
- **Buffer size**: 100 lines (scrollback)

### Audio
Edit `config.h`:
```cpp
#define BELL_FREQ 1000        // Bell frequency (Hz)
#define BELL_DURATION 100     // Bell duration (ms)
#define CLICK_FREQ 2000       // Click frequency (Hz)
#define CLICK_DURATION 10     // Click duration (ms)
```

### WiFi
Settings stored in NVS (non-volatile storage):
- Mode (AP/Client)
- SSID (Client mode)
- Password (Client mode)
- Sound enabled/disabled

## File Structure
```
CYD_Terminal/
├── CYD_Terminal.ino      # Main sketch
├── config.h              # Hardware configuration
├── display.cpp/h         # Display and touch management
├── terminal.cpp/h        # Terminal implementation
├── keyboard.cpp/h        # On-screen keyboard
├── sound.cpp/h           # Audio output
├── wifi_manager.cpp/h    # WiFi and web server
├── utf8.cpp/h            # UTF-8 and Cyrillic font
└── User_Setup_CYD.h      # TFT_eSPI configuration
```

## Logging

Logs are automatically written to SD card:
- Location: `/logs/` directory
- Format: Plain text, UTF-8 encoded
- Access: Via web interface at `/logs`
- Download: Click on filename to download

## Troubleshooting

### Display not working
- Verify TFT_eSPI configuration is applied
- Check backlight pin (GPIO 27)
- Verify SPI pins in `User_Setup_CYD.h`

### Touch not responding
- Check touch calibration in `display.cpp`
- Verify XPT2046 pins in `config.h`

### No sound
- Verify AUDIO_EN pin (GPIO 4)
- Check I2S pins in `config.h`
- Ensure sound is not muted (tap Mute icon)

### WiFi connection fails
- Verify SSID/password
- Check WiFi signal strength
- Try AP mode first to verify WiFi hardware

### Serial data not appearing
- Check baud rate matches connected device
- Verify UART mode (USB vs External)
- Check RX/TX indicators in status bar

## License

MIT License - see LICENSE file

## Credits

- TFT_eSPI library by Bodmer
- ESPAsyncWebServer by me-no-dev
- ESP32 Arduino Core by Espressif
