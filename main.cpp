/*
 * clogger
 * Copyright (C) 2026 Yusifzade Azer
 * Contact: yusifzadeazer22@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdio>
#include <cerrno>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

/*
 * Keys and buttons taken from <linux/input-event-codes.h>
 * See https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h for details
 */

static const std::unordered_map<unsigned int, std::string> keyNames =
{
    {KEY_A, "A"}, {KEY_B, "B"}, {KEY_C, "C"}, {KEY_D, "D"},
    {KEY_E, "E"}, {KEY_F, "F"}, {KEY_G, "G"}, {KEY_H, "H"},
    {KEY_I, "I"}, {KEY_J, "J"}, {KEY_K, "K"}, {KEY_L, "L"},
    {KEY_M, "M"}, {KEY_N, "N"}, {KEY_O, "O"}, {KEY_P, "P"},
    {KEY_Q, "Q"}, {KEY_R, "R"}, {KEY_S, "S"}, {KEY_T, "T"},
    {KEY_U, "U"}, {KEY_V, "V"}, {KEY_W, "W"}, {KEY_X, "X"},
    {KEY_Y, "Y"}, {KEY_Z, "Z"},

    {KEY_0, "0"}, {KEY_1, "1"}, {KEY_2, "2"}, {KEY_3, "3"},
    {KEY_4, "4"}, {KEY_5, "5"}, {KEY_6, "6"}, {KEY_7, "7"},
    {KEY_8, "8"}, {KEY_9, "9"},

    /*
     * Numpad = KP
     */
    {KEY_KP0, "NUMPAD 0"}, {KEY_KP1, "NUMPAD 1"}, {KEY_KP2, "NUMPAD 2"},
    {KEY_KP3, "NUMPAD 3"}, {KEY_KP4, "NUMPAD 4"}, {KEY_KP5, "NUMPAD 5"},
    {KEY_KP6, "NUMPAD 6"}, {KEY_KP7, "NUMPAD 7"}, {KEY_KP8, "NUMPAD 8"},
    {KEY_KP9, "NUMPAD 9"},
    {KEY_KPDOT, "NUMPAD ."}, {KEY_KPENTER, "NUMPAD ENTER"},
    {KEY_KPPLUS, "NUMPAD +"}, {KEY_KPMINUS, "NUMPAD -"},
    {KEY_KPASTERISK, "NUMPAD *"}, {KEY_KPSLASH, "NUMPAD /"},

    {KEY_MINUS, "-"}, {KEY_EQUAL, "="},
    {KEY_LEFTBRACE, "["}, {KEY_RIGHTBRACE, "]"},
    {KEY_SEMICOLON, ";"}, {KEY_APOSTROPHE, "'"},
    {KEY_GRAVE, "`"}, {KEY_BACKSLASH, "\\"},
    {KEY_COMMA, ","}, {KEY_DOT, "."}, {KEY_SLASH, "/"},

    {KEY_SPACE, "SPACE"}, {KEY_ENTER, "ENTER"}, {KEY_ESC, "ESC"},
    {KEY_BACKSPACE, "BACKSPACE"}, {KEY_TAB, "TAB"},
    {KEY_LEFTSHIFT, "LEFT SHIFT"}, {KEY_RIGHTSHIFT, "RIGHT SHIFT"},
    {KEY_LEFTCTRL, "LEFT CTRL"}, {KEY_RIGHTCTRL, "RIGHT CTRL"},
    {KEY_LEFTALT, "LEFT ALT"}, {KEY_RIGHTALT, "RIGHT ALT"},
    {KEY_CAPSLOCK, "CAPS LOCK"},

    {KEY_INSERT, "INSERT"}, {KEY_DELETE, "DELETE"},
    {KEY_HOME, "HOME"}, {KEY_END, "END"},
    {KEY_PAGEUP, "PAGE UP"}, {KEY_PAGEDOWN, "PAGE DOWN"},

    {KEY_UP, "ARROW UP"}, {KEY_DOWN, "ARROW DOWN"},
    {KEY_LEFT, "ARROW LEFT"}, {KEY_RIGHT, "ARROW RIGHT"},

    {KEY_F1, "F1"}, {KEY_F2, "F2"}, {KEY_F3, "F3"}, {KEY_F4, "F4"},
    {KEY_F5, "F5"}, {KEY_F6, "F6"}, {KEY_F7, "F7"}, {KEY_F8, "F8"},
    {KEY_F9, "F9"}, {KEY_F10, "F10"}, {KEY_F11, "F11"}, {KEY_F12, "F12"},

    /*
     * Extra meta
     */
    {KEY_LEFTMETA, "LEFT META"}, {KEY_RIGHTMETA, "RIGHT META"},

    {BTN_LEFT, "MOUSE LEFT BUTTON"},
    {BTN_RIGHT, "MOUSE RIGHT BUTTON"},
    {BTN_MIDDLE, "MOUSE MIDDLE BUTTON"},
};

std::string timestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm *t = std::localtime(&now);
    char buf[9];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", t);
    return std::string("[") + buf + "] ";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    std::cout << "Logger is active >>> \n";

    int fDescr = open(argv[1], O_RDONLY);

    if (fDescr == -1)
    {
        perror("Error");
        return 1;
    }

    struct input_event iEvent;

    bool shiftHeld = false;
    bool capsOn = false;

    while (true)
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
            std::cout << "Incomplete input event\n";
            break;
        }

        /*
         * Shift 1=pressed, 2=repeat, 0=released
         * Caps is toggled
         */
        if (iEvent.code == KEY_LEFTSHIFT || iEvent.code == KEY_RIGHTSHIFT)
            shiftHeld = (iEvent.value != 0);
        if (iEvent.code == KEY_CAPSLOCK && iEvent.value == 1)
            capsOn = !capsOn;

        if (iEvent.type == EV_KEY && iEvent.value == 1)
        {
            /*
             * Input event from keyNames
             */
            auto it = keyNames.find(iEvent.code);

            if (it != keyNames.end())
            {
                std::cout << timestamp();
                const std::string &name = it->second;

                if (shiftHeld && capsOn)
                    std::cout << "Key >>> (Shift + Caps + " << name << ")\n";
                else if (shiftHeld)
                    std::cout << "Key >>> (Shift + " << name << ")\n";
                else if (capsOn)
                    std::cout << "Key >>> (Caps + " << name << ")\n";
                else
                    std::cout << "Key >>> " << name << "\n";
            }
            else
            {
                std::cout << timestamp();
                std::cout << "UNKNOWN KEY CODE: " << iEvent.code << "\n";
            }
        }
    }

    close(fDescr);
    return 0;
}