#ifndef BTKEYCODES_H
#define BTKEYCODES_H

//modifier codes
#define BTMC_NONE 0x00
#define BTMC_CTRL_LEFT 0x01
#define BTMC_SHIFT_LEFT 0x02
#define BTMC_ALT_LEFT 0x04
#define BTMC_GUI_LEFT 0x08
#define BTMC_CTRL_RIGHT 0x10
#define BTMC_SHIFT_RIGHT 0x20
#define BTMC_ALT_RIGHT 0x40
#define BTMC_GUI_RIGHT 0x80

//consumer codes
#define BTCC_NONE 0x0000
#define BTCC_HOME 0x0001
#define BTCC_EMAIL 0x0002
#define BTCC_SEARCH 0x0004
#define BTCC_KEYBOARD_LAYOUT 0x0008
#define BTCC_VOLUME_UP 0x0010
#define BTCC_VOLUME_DOWN 0x0020
#define BTCC_MUTE 0x0040
#define BTCC_PLAY_PAUSE 0x0080
#define BTCC_NEXT_TRACK 0x0100
#define BTCC_PREVIOUS_TRACK 0x0200
#define BTCC_STOP 0x0400
#define BTCC_EJECT 0x0800
#define BTCC_FAST_FORWARD 0x1000
#define BTCC_REWIND 0x2000
#define BTCC_STOP_EJECT 0x4000
#define BTCC_BROWSER 0x8000

//keycodes (MODIFIED FOR MAGIC MUSIC KEYBOARD)
#define BTKC_NONE 0
#define BTKC_1 59
#define BTKC_Q 46
#define BTKC_A 34
#define BTKC_Z 24
#define BTKC_2 60
#define BTKC_W 47
#define BTKC_S 35
#define BTKC_X 25
#define BTKC_3 61
#define BTKC_E 48
#define BTKC_D 36
#define BTKC_C 26
#define BTKC_4 62
#define BTKC_R 49
#define BTKC_F 37
#define BTKC_V 27
#define BTKC_5 63
#define BTKC_T 50
#define BTKC_G 38
#define BTKC_B 28
#define BTKC_6 64
#define BTKC_Y 51
#define BTKC_H 39
#define BTKC_N 29
#define BTKC_7 65
#define BTKC_U 52
#define BTKC_J 40
#define BTKC_M 30
#define BTKC_8 66
#define BTKC_I 53
#define BTKC_K 41
#define BTKC_9 67
#define BTKC_O 54
#define BTKC_L 42
#define BTKC_0 68
#define BTKC_P 55
#define BTKC_BACKSLASH 70
#define BTKC_BACKSLASH_ 71
#define BTKC_RETURN 45
#define BTKC_ESCAPE 10
#define BTKC_BACKSPACE 72
#define BTKC_TAB 48
#define BTKC_SPACE 100
#define BTKC_MINUS 69
#define BTKC_EQUAL 0
#define BTKC_BRACKET_LEFT 0
#define BTKC_BRACKET_RIGHT 0
#define BTKC_EUROPE_1 0
#define BTKC_SEMICOLON 0
#define BTKC_APOSTROPHE 0
#define BTKC_GRAVE 0
#define BTKC_COMMA 31
#define BTKC_PERIOD 32 // Ю
#define BTKC_SLASH 33
#define BTKC_CAPS_LOCK 97
#define BTKC_F1 73
#define BTKC_F2 74
#define BTKC_F3 75
#define BTKC_F4 76
#define BTKC_F5 77
#define BTKC_F6 78
#define BTKC_F7 79
#define BTKC_F8 80
#define BTKC_F9 81
#define BTKC_F10 82
#define BTKC_F11 83
#define BTKC_F12 84
#define BTKC_PRINT_SCREEN 0
#define BTKC_SCROLL_LOCK 101
#define BTKC_PAUSE 102
#define BTKC_INSERT 92
#define BTKC_HOME 93
#define BTKC_PAGE_UP 94
#define BTKC_DELETE 89
#define BTKC_END 90
#define BTKC_PAGE_DOWN 91
#define BTKC_ARROW_RIGHT 87
#define BTKC_ARROW_LEFT 85
#define BTKC_ARROW_DOWN 86
#define BTKC_ARROW_UP 88
#define BTKC_NUM_LOCK 9
#define BTKC_KEYPAD_DIVIDE 160
#define BTKC_KEYPAD_MULTIPLY 104
#define BTKC_KEYPAD_SUBTRACT 105
#define BTKC_KEYPAD_ADD 106
#define BTKC_KEYPAD_ENTER 132
#define BTKC_KEYPAD_1 151
#define BTKC_KEYPAD_2 152
#define BTKC_KEYPAD_3 153
#define BTKC_KEYPAD_4 154
#define BTKC_KEYPAD_5 155
#define BTKC_KEYPAD_6 156
#define BTKC_KEYPAD_7 157
#define BTKC_KEYPAD_8 158
#define BTKC_KEYPAD_9 159
#define BTKC_KEYPAD_0 150
#define BTKC_KEYPAD_DECIMAL 121
#define BTKC_EUROPE_2 0
#define BTKC_APPLICATION 0
#define BTKC_POWER 0
#define BTKC_KEYPAD_EQUAL 0
#define BTKC_F13 0
#define BTKC_F14 0
#define BTKC_F15 0
#define BTKC_PLUS 69
#define BTKC_BAR 58 // ё
#define BTKC_LCTRL 130
#define BTKC_LSHIFT 0
#define BTKC_LESS 44
#define BTKC_CAPS 97
#define BTKC_ALT 98
#define BTKC_ALTGR 99
#define BTKC_HYPHEN 33  // / ? near right shift
#define BTKC_OSLASH 43 // Ж
#define BTKC_ARING 56  // Х
#define BTKC_RCTRL 134
#define BTKC_RSHIFT 133
#define BTKC_UMLAUT 57 // Ъ
#define BTKC_AE 44 // Э

//highest bit masked with 1 is a virtual keycode
//ie, 0x80 = 1000000b

//0x80 to 0x87 are modifer keys
// the lower 7 bits defines how much 0x1 is shifted left to get the modifier code
// ie) BTKC_ALT_LEFT = 0x82 (10000010b)
//       lower 7 bits = 0x02 (0000010b)
//       0x1 << 2 = 100b = 0x04 = BTMC_ALT_LEFT
#define BTKC_CTRL_LEFT 0x80
#define BTKC_SHIFT_LEFT 0x81
#define BTKC_ALT_LEFT 0x82
#define BTKC_GUI_LEFT 0x83
#define BTKC_CTRL_RIGHT 0x84
#define BTKC_SHIFT_RIGHT 0x85
#define BTKC_ALT_RIGHT 0x86
#define BTKC_GUI_RIGHT 0x87

//0xC0 to 0xD0 are consumer keys
// the lower 6 bits defines how much 0x1 is shifted left to get the 2 byte consumer code
// ie) BTKC_VOLUME_UP = 0xC4 (11000100b)
//       lower 6 bits = 0x04 (000100b)
//       0x1 << 4 = 10000b = 0x0010 = BTCC_VOLUME_UP
//consumer codes
#define BTKC_HOMEPAGE 0xC0
#define BTKC_EMAIL 0xC1
#define BTKC_SEARCH 0xC2
#define BTKC_KEYBOARD_LAYOUT 0xC3
#define BTKC_VOLUME_UP 0xC4
#define BTKC_VOLUME_DOWN 0xC5
#define BTKC_MUTE 0xC6
#define BTKC_PLAY_PAUSE 0xC7
#define BTKC_NEXT_TRACK 0xC8
#define BTKC_PREVIOUS_TRACK 0xC9
#define BTKC_STOP 0xCA
#define BTKC_EJECT 0xCB
#define BTKC_FAST_FORWARD 0xCD
#define BTKC_REWIND 0xCE
#define BTKC_STOP_EJECT 0xCF
#define BTKC_BROWSER 0xD0

//0xE0 to 0xE2 are system keys
#define BTKC_SYSTEM_POWER 0xE0 //0x81
#define BTKC_SYSTEM_SLEEP 0xE1 //0x82
#define BTKC_SYSTEM_WAKE 0xE2 //0x83


#endif //BTKEYCODES_H
