K_BACK       =  kVK_Delete, //should be 8 ?
K_BACKSPACE  = K_BACK,

K_TAB        = kVK_Tab,

K_RETURN     = kVK_Return, //24 not 13?,
K_ENTER      = K_RETURN,
//K_ESCAPE     = 27,

K_SPACE      = 32,

//K_DELETE     = XK_Delete + K_DELTA,
//K_DELETE     = NSDeleteCharacter,

K_SHIFT_KEY  = kVK_Shift + K_DELTA,
K_CTRL_KEY   = kVK_Control + K_DELTA,
K_ALT_KEY    = kVK_Option + K_DELTA,
K_CAPSLOCK   = kVK_CapsLock + K_DELTA,
K_ESCAPE     = kVK_Escape + K_DELTA,
//K_PRIOR      = VK_PRIOR + K_DELTA,
K_PAGEUP     = kVK_PageUp + K_DELTA,
//K_NEXT       = VK_NEXT + K_DELTA,
K_PAGEDOWN   = kVK_PageDown + K_DELTA,
K_END        = kVK_End + K_DELTA,
K_HOME       = kVK_Home + K_DELTA,
K_LEFT       = kVK_LeftArrow + K_DELTA,
K_UP         = kVK_UpArrow + K_DELTA,
K_RIGHT      = kVK_RightArrow + K_DELTA,
K_DOWN       = kVK_DownArrow + K_DELTA,
K_INSERT     = 0x72 + K_DELTA, //114      //VK_INSERT + K_DELTA,
K_DELETE     = kVK_ForwardDelete + K_DELTA,  //0x75 117

K_NUMPAD0    = kVK_ANSI_Keypad0 + K_DELTA,
K_NUMPAD1    = kVK_ANSI_Keypad1 + K_DELTA,
K_NUMPAD2    = kVK_ANSI_Keypad2 + K_DELTA,
K_NUMPAD3    = kVK_ANSI_Keypad3 + K_DELTA,
K_NUMPAD4    = kVK_ANSI_Keypad4 + K_DELTA,
K_NUMPAD5    = kVK_ANSI_Keypad5 + K_DELTA,
K_NUMPAD6    = kVK_ANSI_Keypad6 + K_DELTA,
K_NUMPAD7    = kVK_ANSI_Keypad7 + K_DELTA,
K_NUMPAD8    = kVK_ANSI_Keypad8 + K_DELTA,
K_NUMPAD9    = kVK_ANSI_Keypad9 + K_DELTA,
K_MULTIPLY   = kVK_ANSI_KeypadMultiply + K_DELTA,
K_ADD        = kVK_ANSI_KeypadPlus + K_DELTA,
//K_SEPARATOR  = VK_SEPARATOR + K_DELTA,
K_SUBTRACT   = kVK_ANSI_KeypadMinus + K_DELTA,
K_DECIMAL    = kVK_ANSI_KeypadDecimal + K_DELTA,
K_DIVIDE     = kVK_ANSI_KeypadDivide + K_DELTA,
//K_SCROLL     = VK_SCROLL + K_DELTA,

K_F1         = kVK_F1 + K_DELTA,
K_F2         = kVK_F2 + K_DELTA,
K_F3         = kVK_F3 + K_DELTA,
K_F4         = kVK_F4 + K_DELTA,
K_F5         = kVK_F5 + K_DELTA,
K_F6         = kVK_F6 + K_DELTA,
K_F7         = kVK_F7 + K_DELTA,
K_F8         = kVK_F8 + K_DELTA,
K_F9         = kVK_F9 + K_DELTA,
K_F10        = kVK_F10 + K_DELTA,
K_F11        = kVK_F11 + K_DELTA,
K_F12        = kVK_F12 + K_DELTA,

K_A          = 'A' + K_DELTA,
K_B          = 'B' + K_DELTA,
K_C          = 'C' + K_DELTA,
K_D          = 'D' + K_DELTA,
K_E          = 'E' + K_DELTA,
K_F          = 'F' + K_DELTA,
K_G          = 'G' + K_DELTA,
K_H          = 'H' + K_DELTA,
K_I          = 'I' + K_DELTA,
K_J          = 'J' + K_DELTA,
K_K          = 'K' + K_DELTA,
K_L          = 'L' + K_DELTA,
K_M          = 'M' + K_DELTA,
K_N          = 'N' + K_DELTA,
K_O          = 'O' + K_DELTA,
K_P          = 'P' + K_DELTA,
K_Q          = 'Q' + K_DELTA,
K_R          = 'R' + K_DELTA,
K_S          = 'S' + K_DELTA,
K_T          = 'T' + K_DELTA,
K_U          = 'U' + K_DELTA,
K_V          = 'V' + K_DELTA,
K_W          = 'W' + K_DELTA,
K_X          = 'X' + K_DELTA,
K_Y          = 'Y' + K_DELTA,
K_Z          = 'Z' + K_DELTA,
K_0          = '0' + K_DELTA,
K_1          = '1' + K_DELTA,
K_2          = '2' + K_DELTA,
K_3          = '3' + K_DELTA,
K_4          = '4' + K_DELTA,
K_5          = '5' + K_DELTA,
K_6          = '6' + K_DELTA,
K_7          = '7' + K_DELTA,
K_8          = '8' + K_DELTA,
K_9          = '9' + K_DELTA,

K_CTRL_LBRACKET  = K_CTRL|219|K_DELTA,
K_CTRL_RBRACKET  = K_CTRL|221|K_DELTA,
K_CTRL_MINUS     = K_CTRL|0xbd|K_DELTA,
K_CTRL_GRAVE     = K_CTRL|0xc0|K_DELTA,
K_CTRL_SLASH     = K_CTRL|0xbf|K_DELTA,
K_CTRL_BACKSLASH = K_CTRL|0xdc|K_DELTA,
K_CTRL_COMMA     = K_CTRL|0xbc|K_DELTA,
K_CTRL_PERIOD    = K_CTRL|0xbe|K_DELTA,
K_CTRL_SEMICOLON = K_CTRL|0xbe|K_DELTA,
K_CTRL_EQUAL     = K_CTRL|0xbb|K_DELTA,
K_CTRL_APOSTROPHE= K_CTRL|0xde|K_DELTA,
//K_CTRL_LBRACKET  = K_CTRL|XK_bracketleft|K_DELTA,
//K_CTRL_RBRACKET  = K_CTRL|XK_bracketright|K_DELTA,
//K_CTRL_MINUS     = K_CTRL|0x2d|K_DELTA,
//K_CTRL_GRAVE     = K_CTRL|XK_grave|K_DELTA,
//K_CTRL_SLASH     = K_CTRL|0x5f|K_DELTA,
//K_CTRL_BACKSLASH = K_CTRL|0x5c|K_DELTA,
//K_CTRL_COMMA     = K_CTRL|0x2c|K_DELTA,
//K_CTRL_PERIOD    = K_CTRL|XK_period|K_DELTA,
//K_CTRL_SEMICOLON = K_CTRL|XK_semicolon|K_DELTA,
//K_CTRL_EQUAL     = K_CTRL|0x3d|K_DELTA,
//K_CTRL_APOSTROPHE= K_CTRL|0x27|K_DELTA,

//K_BREAK      = VK_Pause|K_DELTA,
