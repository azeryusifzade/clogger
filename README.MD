# Input Event Logger

A small C program for learning how Linux exposes keyboard input through `/dev/input/eventX`. It reads raw `input_event` records from a device file and prints the key that was pressed, along with Shift/Caps state and a timestamp.

**For educational purposes only.** Only run this on devices you own or have explicit permission to monitor.

## Build

```bash
gcc -Wall -Wextra -g main.c -o logger
```

## Usage

```bash
sudo ./logger /dev/input/eventX
```

Replace `eventX` with your keyboard's device number. You can find it by checking:

```bash
cat /proc/bus/input/devices
```

Press Ctrl+C to stop the logger.

## Example output

```
Logger is active >>> 
[14:32:07] Key >>> A
[14:32:08] Key >>> (Shift + B)
[14:32:09] Key >>> (Caps + C)
```

## Note
Requires manually specifying the device path