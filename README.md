# GNSS Logger

A C program that logs GNSS NMEA data from a dynamically selected serial port on a Raspberry Pi or Linux system. The user can choose from available serial ports, baud rates (up to **921600**), and a test duration before logging the data to a timestamped file.

## 🚀 Features
- **Automatic Serial Port Detection**: Scans `/dev/` for available serial ports (`ttyUSB`, `ttyS`, `serial`).
- **Configurable Baud Rate**: Supports baud rates from **9600** to **921600**.
- **Customizable Logging Duration**: Users can set a test duration (1-86400 seconds).
- **Logs GNSS Data**: Captures and saves GNSS messages to a structured log file.
- **Timestamped Log Files**: Stores logs in `logs/` with automatic filenames.

## 📌 Prerequisites
- Raspberry Pi or Linux-based system.
- GNSS module connected via **UART** (USB or GPIO serial interface).
- `gcc` compiler installed.

## 🔧 Installation & Compilation
Clone the repository and compile the program:
```sh
git clone https://github.com/adedolapoadegboye/GNSS-Logger-Serial-C
make
```

## 📌 Usage
Run the program:
```sh
sudo ./basicgnsslogger
```

The user will be prompted to:
1. **Select a serial port** from a dynamically detected list.
2. **Select a baud rate** from available options.
3. **Set the logging duration** (1-300 seconds).

The GNSS data will be logged and saved automatically.

## 📜 Example Output
```
Example of available serial ports:
1: /dev/ttyUSB0
2: /dev/ttyUSB1
Select a serial port (1-2): 1

Available Baud Rates:
1: 9600
2: 19200
3: 38400
4: 57600
5: 115200
6: 230400
7: 460800
8: 921600
Select a baud rate (1-8): 5

Enter test duration (seconds, min: 1, max: 86400): 10
Logging GNSS data to logs/gnss_log_20240309_153000.txt for 10 seconds...
```

## 📂 Log File Format
- The log file is stored in the `logs/` directory.
- The filename is automatically generated based on the timestamp:
  ```
  logs/gnss_log_YYYYMMDD_HHMMSS.txt
  ```
- Data is written in raw NMEA format.

## ⚡ Troubleshooting
| Issue                                   | Solution                                                                                       |
| --------------------------------------- | ---------------------------------------------------------------------------------------------- |
| **No serial ports found**               | Ensure the device is connected and run `ls /dev/ttyUSB*` or `ls /dev/serial*`.                 |
| **Permission denied on `/dev/serial0`** | Run with `sudo` or add the user to the `dialout` group: `sudo usermod -a -G dialout $(whoami)` |
| **No GNSS data received**               | Verify module connections and baud rate settings.                                              |

## 🛠 Future Improvements
- Add error handling for corrupted NMEA messages.
- Implement logging formats such as **CSV or JSON**.
- Support real-time GNSS message filtering.

## 📜 License
MIT License

