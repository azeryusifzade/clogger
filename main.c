#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    printf("Logger is active >>> \n");

    int fDescr = open(argv[1], O_RDONLY);

    if (fDescr == -1)
    {
        perror("Error");
        exit(1);
    }

    struct input_event iEvent;

    while (1)
    {
        ssize_t bytesRead = read(fDescr, &iEvent, sizeof(iEvent));
        /*
         * Checking read()
         */
        if (bytesRead == -1)
        {
            perror("read");
            break;
        }
        if (bytesRead == 0) break;
        if (bytesRead != sizeof(iEvent))
        {
            printf("Incomplete input event\n");
            break;
        }

        if (iEvent.type == EV_KEY && iEvent.value == 1)
        {
        /*
         * Keys and buttons taken from <linux/input-event-codes.h>
         */
            if (iEvent.code == KEY_A) printf("A\n");
            else if (iEvent.code == KEY_B) printf("B\n");
            else if (iEvent.code == KEY_C) printf("C\n");
            else if (iEvent.code == KEY_D) printf("D\n");
            else if (iEvent.code == KEY_E) printf("E\n");
            else if (iEvent.code == KEY_F) printf("F\n");
            else if (iEvent.code == KEY_G) printf("G\n");
            else if (iEvent.code == KEY_H) printf("H\n");
            else if (iEvent.code == KEY_I) printf("I\n");
            else if (iEvent.code == KEY_J) printf("J\n");
            else if (iEvent.code == KEY_K) printf("K\n");
            else if (iEvent.code == KEY_L) printf("L\n");
            else if (iEvent.code == KEY_M) printf("M\n");
            else if (iEvent.code == KEY_N) printf("N\n");
            else if (iEvent.code == KEY_O) printf("O\n");
            else if (iEvent.code == KEY_P) printf("P\n");
            else if (iEvent.code == KEY_Q) printf("Q\n");
            else if (iEvent.code == KEY_R) printf("R\n");
            else if (iEvent.code == KEY_S) printf("S\n");
            else if (iEvent.code == KEY_T) printf("T\n");
            else if (iEvent.code == KEY_U) printf("U\n");
            else if (iEvent.code == KEY_V) printf("V\n");
            else if (iEvent.code == KEY_W) printf("W\n");
            else if (iEvent.code == KEY_X) printf("X\n");
            else if (iEvent.code == KEY_Y) printf("Y\n");
            else if (iEvent.code == KEY_Z) printf("Z\n");

            else if (iEvent.code == KEY_0) printf("0\n");
            else if (iEvent.code == KEY_1) printf("1\n");
            else if (iEvent.code == KEY_2) printf("2\n");
            else if (iEvent.code == KEY_3) printf("3\n");
            else if (iEvent.code == KEY_4) printf("4\n");
            else if (iEvent.code == KEY_5) printf("5\n");
            else if (iEvent.code == KEY_6) printf("6\n");
            else if (iEvent.code == KEY_7) printf("7\n");
            else if (iEvent.code == KEY_8) printf("8\n");
            else if (iEvent.code == KEY_9) printf("9\n");

            else if (iEvent.code == KEY_SPACE) printf("SPACE\n");
            else if (iEvent.code == KEY_ENTER) printf("ENTER\n");
            else if (iEvent.code == KEY_ESC) printf("ESC\n");
            else if (iEvent.code == KEY_BACKSPACE) printf("BACKSPACE\n");
            else if (iEvent.code == KEY_TAB) printf("TAB\n");
            else if (iEvent.code == KEY_LEFTSHIFT) printf("LEFT SHIFT\n");
            else if (iEvent.code == KEY_RIGHTSHIFT) printf("RIGHT SHIFT\n");
            else if (iEvent.code == KEY_LEFTCTRL) printf("LEFT CTRL\n");
            else if (iEvent.code == KEY_RIGHTCTRL) printf("RIGHT CTRL\n");
            else if (iEvent.code == KEY_LEFTALT) printf("LEFT ALT\n");
            else if (iEvent.code == KEY_RIGHTALT) printf("RIGHT ALT\n");
            else if (iEvent.code == KEY_CAPSLOCK) printf("CAPS LOCK\n");

            else if (iEvent.code == KEY_UP) printf("ARROW UP\n");
            else if (iEvent.code == KEY_DOWN) printf("ARROW DOWN\n");
            else if (iEvent.code == KEY_LEFT) printf("ARROW LEFT\n");
            else if (iEvent.code == KEY_RIGHT) printf("ARROW RIGHT\n");

            else if (iEvent.code == KEY_F1) printf("F1\n");
            else if (iEvent.code == KEY_F2) printf("F2\n");
            else if (iEvent.code == KEY_F3) printf("F3\n");
            else if (iEvent.code == KEY_F4) printf("F4\n");
            else if (iEvent.code == KEY_F5) printf("F5\n");
            else if (iEvent.code == KEY_F6) printf("F6\n");
            else if (iEvent.code == KEY_F7) printf("F7\n");
            else if (iEvent.code == KEY_F8) printf("F8\n");
            else if (iEvent.code == KEY_F9) printf("F9\n");
            else if (iEvent.code == KEY_F10) printf("F10\n");
            else if (iEvent.code == KEY_F11) printf("F11\n");
            else if (iEvent.code == KEY_F12) printf("F12\n");

        /*
         * For mouse buttons try the different event file
         */
            else if (iEvent.code == BTN_LEFT) printf("MOUSE LEFT BUTTON\n");
            else if (iEvent.code == BTN_RIGHT) printf("MOUSE RIGHT BUTTON\n");
            else if (iEvent.code == BTN_MIDDLE) printf("MOUSE MIDDLE BUTTON\n");

            else printf("UNKNOWN KEY CODE: %u\n", iEvent.code);

        }
    }

    close(fDescr);
    return 0;
}
