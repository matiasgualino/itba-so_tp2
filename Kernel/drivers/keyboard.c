#include <stdint.h>
#include <video.h>
#include <keyboard.h>
#include <lib.h>

#define FIRST_BITE_ON(c) (0x80 | c)

extern int video_current_column;
extern int video_current_row;

extern bool screensaverActive;

char buffer[KEYBOARD_BUFFER_SIZE] = {0};

int dequeuePos = 0;
int enqueuePos = 0;

static bool EOF_READ = FALSE;

static specialKeysStatus specialStatus = { FALSE, FALSE, FALSE }; // CAPS, CTRL, ALT

#define NOTHING (char)0

static bool screensaverWillActive = FALSE;

// scancode, ascii, caps active
keyboardCode keyboardCodes[256] = {
    {0x00 , NOTHING, NOTHING}, 
    {0x01 , NOTHING, NOTHING}, // ESCAPE
    {0x02 , '1', '!'},
    {0x03 , '2', '@'},
    {0x04 , '3', '#'},
    {0x05 , '4', '$'},
    {0x06 , '5', '%'},
    {0x07 , '6', '^'},
    {0x08 , '7', '&'},
    {0x09 , '8', '*'},
    {0x0A , '9', '('},
    {0x0B , '0', '"'},
    {0x0C , '-', '_'},
    {0x0D , '=', '+'},
    {0x0E , NOTHING, NOTHING}, // BACKSPACE
    {0x0F , NOTHING, NOTHING}, // TAB
    {0x10 , 'q', 'Q'},
    {0x11, 'w', 'W'},
    {0x12, 'e', 'E'},
    {0x13, 'r', 'R'},
    {0x14, 't', 'T'},
    {0x15, 'y', 'Y'},
    {0x16, 'u', 'U'},
    {0x17, 'i', 'I'},
    {0x18, 'o', 'O'},
    {0x19, 'p', 'P'},
    {0x1a, '[', '{'},
    {0x1b, ']', '}'},
    {0x1c, NOTHING, NOTHING}, // ENTER
    {0x1d, NOTHING, NOTHING}, // CTRL IZQ
    {0x1e, 'a', 'A'},
    {0x1f, 's', 'S'},
    {0x20, 'd', 'D'},
    {0x21, 'f', 'F'},
    {0x22, 'g', 'G'},
    {0x23, 'h', 'H'},
    {0x24, 'j', 'J'},
    {0x25, 'k', 'K'},
    {0x26, 'l', 'L'},
    {0x27, ';', ':'},
    {0x28, '\'', '"'},
    {0x29, '`', '~'},
    {0x2a, NOTHING, NOTHING}, // SHIFT IZQ
    {0x2b, '\\', '|'},
    {0x2c, 'z', 'Z'},
    {0x2d, 'x', 'X'},
    {0x2e, 'c', 'C'},
    {0x2f, 'v', 'V'},
    {0x30, 'b', 'B'},
    {0x31, 'n', 'N'},
    {0x32, 'm', 'M'},
    {0x33, ',', '<'},
    {0x34, '.', '>'},
    {0x35, '/', '?'},
    {0x36, NOTHING, NOTHING}, // SHIFT DER
    {0x37, '*', NOTHING}, // * KEYPAD
    {0x38, NOTHING, NOTHING}, // ALT IZQ
    {0x39, ' ', ' '},
    {0x3a, NOTHING, NOTHING}, // CAPSLOCK
    {0x3b, NOTHING, NOTHING}, // F1
    {0x3c, NOTHING, NOTHING}, // F2
    {0x3d, NOTHING, NOTHING}, // F3
    {0x3e, NOTHING, NOTHING}, // F4
    {0x3f, NOTHING, NOTHING}, // F5
    {0x40, NOTHING, NOTHING}, // F6
    {0x41, NOTHING, NOTHING}, // F7
    {0x42, NOTHING, NOTHING}, // F8
    {0x43, NOTHING, NOTHING}, // F9
    {0x44, NOTHING, NOTHING}, // F10
    {0x45, NOTHING, NOTHING}, // NUMPAD
    {0x46, NOTHING, NOTHING}, // SCROLL
    {0x47, NOTHING, NOTHING}, // UP ARROW
    {0x48, NOTHING, NOTHING}, // 
    {0x49, NOTHING, NOTHING}, // 
    {0x4a, NOTHING, NOTHING}, // 
    {0x4b, NOTHING, NOTHING}, // LEFT ARROW
    {0x4c, NOTHING, NOTHING}, // 
    {0x4d, NOTHING, NOTHING}, // RIGHT ARROW
    {0x4e, NOTHING, NOTHING}, // 
    {0x4f, NOTHING, NOTHING}, // 
    {0x50, NOTHING, NOTHING}, // DOWN ARROW
    {0x51, NOTHING, NOTHING}, // 
    {0x52, NOTHING, NOTHING}, // 
    {0x53, NOTHING, NOTHING}, // DELETE
    {0x55, '*', NOTHING},  
    {0x56, '*', NOTHING}, 
    {0x57, NOTHING, NOTHING}, // F11
    {0x58, NOTHING, NOTHING}// F12
};

static bool writeBuffer(char c) {
    int pos = (dequeuePos + enqueuePos) % KEYBOARD_BUFFER_SIZE;
    if (pos == dequeuePos && enqueuePos != 0) {
        return FALSE;
    }
    buffer[pos] = c;
    enqueuePos++;
    return TRUE;
}

static void deleteBuffer() {
    if (enqueuePos == 0) {
        return;
    }

    if (video_current_column == 0) {
        video_current_column = SCREEN_WIDTH - 1;
        video_current_row--;
    } else {
        video_current_column--;
    }

    enqueuePos--;

    video_print_char_at(' ', video_current_row, video_current_column);
    video_update_cursor();
}

void replaceLastWritten(char* string) {
    enqueuePos = 0;
    while (*string != 0) {
        writeBuffer(*string);
        string++;
    }
}

int waitBuffer(int length) {
    enqueuePos = 0;
    EOF_READ = FALSE;
    while (enqueuePos < length && !EOF_READ) ;
    return enqueuePos;
}

char getCharFromBuffer() {
    char ret = buffer[dequeuePos];
    dequeuePos++;
    dequeuePos = dequeuePos % KEYBOARD_BUFFER_SIZE;
    return ret;
}

static void writeChar(char c) {
    if (writeBuffer(c)) {
        video_printc(c);
    }
}

void keyboardHandler(uint64_t string) {
    if (!screensaverWillActive && screensaverResetTimer()) {
        screensaverWillActive = FALSE;
        return;
    }
    
    keyboardCode t = keyboardCodes[string];

    if (t.ascii == NOTHING) {

        switch (string) {
            case 0x1c: // ENTER

	            EOF_READ = TRUE;
	            screensaverWillActive = TRUE;
	            break;
            case 0x0E: // BACKSPACE
                deleteBuffer();
                break;

            case 0x3a: // CAPSLOCK
                specialStatus.capsON = !specialStatus.capsON;
                break;

            case FIRST_BITE_ON(0x1c):
                if (screensaverWillActive) {
                    screensaverWillActive = FALSE;
                    return;
                }
                break;

            case 0x2a:
            case 0x36: // SHIFT
                specialStatus.capsON = !specialStatus.capsON;
                break;

            case FIRST_BITE_ON(0x2a):
            case FIRST_BITE_ON(0x36): //SHIFT
                specialStatus.capsON = !specialStatus.capsON;
                break;
        }
    } else {
        if (specialStatus.capsON) {
            writeChar(t.specialCharacter);
        } else {
            writeChar(t.ascii);
        }

        video_update_cursor();
    }
}
