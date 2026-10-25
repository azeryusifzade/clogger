#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>

/*
 * Keys and buttons taken from <linux/input-event-codes.h>
 * See https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h for details
 */

static const char *key_names[KEY_MAX + 1] = 
{
    [KEY_A] = "A", [KEY_B] = "B", [KEY_C] = "C", [KEY_D] = "D",
    [KEY_E] = "E", [KEY_F] = "F", [KEY_G] = "G", [KEY_H] = "H",
    [KEY_I] = "I", [KEY_J] = "J", [KEY_K] = "K", [KEY_L] = "L",
    [KEY_M] = "M", [KEY_N] = "N", [KEY_O] = "O", [KEY_P] = "P",
    [KEY_Q] = "Q", [KEY_R] = "R", [KEY_S] = "S", [KEY_T] = "T",
    [KEY_U] = "U", [KEY_V] = "V", [KEY_W] = "W", [KEY_X] = "X",
    [KEY_Y] = "Y", [KEY_Z] = "Z",

    [KEY_0] = "0", [KEY_1] = "1", [KEY_2] = "2", [KEY_3] = "3",
    [KEY_4] = "4", [KEY_5] = "5", [KEY_6] = "6", [KEY_7] = "7",
    [KEY_8] = "8", [KEY_9] = "9",
    /*
    * Numpad = KP
    */
    [KEY_KP0] = "NUMPAD 0", [KEY_KP1] = "NUMPAD 1", [KEY_KP2] = "NUMPAD 2",
    [KEY_KP3] = "NUMPAD 3", [KEY_KP4] = "NUMPAD 4", [KEY_KP5] = "NUMPAD 5",
    [KEY_KP6] = "NUMPAD 6", [KEY_KP7] = "NUMPAD 7", [KEY_KP8] = "NUMPAD 8",
    [KEY_KP9] = "NUMPAD 9",
    [KEY_KPDOT] = "NUMPAD .", [KEY_KPENTER] = "NUMPAD ENTER",
    [KEY_KPPLUS] = "NUMPAD +", [KEY_KPMINUS] = "NUMPAD -",
    [KEY_KPASTERISK] = "NUMPAD *", [KEY_KPSLASH] = "NUMPAD /",

    [KEY_MINUS] = "-", [KEY_EQUAL] = "=",
    [KEY_LEFTBRACE] = "[", [KEY_RIGHTBRACE] = "]",
    [KEY_SEMICOLON] = ";", [KEY_APOSTROPHE] = "'",
    [KEY_GRAVE] = "`", [KEY_BACKSLASH] = "\\",
    [KEY_COMMA] = ",", [KEY_DOT] = ".", [KEY_SLASH] = "/",

    [KEY_SPACE] = "SPACE", [KEY_ENTER] = "ENTER", [KEY_ESC] = "ESC",
    [KEY_BACKSPACE] = "BACKSPACE", [KEY_TAB] = "TAB",
    [KEY_LEFTSHIFT] = "LEFT SHIFT", [KEY_RIGHTSHIFT] = "RIGHT SHIFT",
    [KEY_LEFTCTRL] = "LEFT CTRL", [KEY_RIGHTCTRL] = "RIGHT CTRL",
    [KEY_LEFTALT] = "LEFT ALT", [KEY_RIGHTALT] = "RIGHT ALT",
    [KEY_CAPSLOCK] = "CAPS LOCK",

    [KEY_INSERT] = "INSERT", [KEY_DELETE] = "DELETE",
    [KEY_HOME] = "HOME", [KEY_END] = "END",
    [KEY_PAGEUP] = "PAGE UP", [KEY_PAGEDOWN] = "PAGE DOWN",

    [KEY_UP] = "ARROW UP", [KEY_DOWN] = "ARROW DOWN",
    [KEY_LEFT] = "ARROW LEFT", [KEY_RIGHT] = "ARROW RIGHT",

    [KEY_F1] = "F1", [KEY_F2] = "F2", [KEY_F3] = "F3", [KEY_F4] = "F4",
    [KEY_F5] = "F5", [KEY_F6] = "F6", [KEY_F7] = "F7", [KEY_F8] = "F8",
    [KEY_F9] = "F9", [KEY_F10] = "F10", [KEY_F11] = "F11", [KEY_F12] = "F12",
    /*
    * Extra meta
    */
    [KEY_LEFTMETA] = "LEFT META", [KEY_RIGHTMETA] = "RIGHT META",

    [BTN_LEFT] = "MOUSE LEFT BUTTON",
    [BTN_RIGHT] = "MOUSE RIGHT BUTTON",
    [BTN_MIDDLE] = "MOUSE MIDDLE BUTTON",
};

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
            if (errno == EINTR) continue;

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
         * Getting input event from key_names
         */
            
            if (iEvent.code <= KEY_MAX && key_names[iEvent.code]) printf("Key >>> %s\n", key_names[iEvent.code]);
            else printf("UNKNOWN KEY CODE: %u\n", iEvent.code);

        }
    }

    close(fDescr);
    return 0;
}
