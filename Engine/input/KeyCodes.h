#pragma once

// We use the GLFW keycode mappings for 2 reasons:
//     - Ease of API translation
//     - They are ASCII compliant and can be easily converted to text

/* The unknown key */
#define KEYCODE_UNKNOWN            -1

/* Printable keys */
#define KEYCODE_SPACE              32
#define KEYCODE_APOSTROPHE         39  /* ' */
#define KEYCODE_COMMA              44  /* , */
#define KEYCODE_MINUS              45  /* - */
#define KEYCODE_PERIOD             46  /* . */
#define KEYCODE_SLASH              47  /* / */
#define KEYCODE_0                  48
#define KEYCODE_1                  49
#define KEYCODE_2                  50
#define KEYCODE_3                  51
#define KEYCODE_4                  52
#define KEYCODE_5                  53
#define KEYCODE_6                  54
#define KEYCODE_7                  55
#define KEYCODE_8                  56
#define KEYCODE_9                  57
#define KEYCODE_SEMICOLON          59  /* ; */
#define KEYCODE_EQUAL              61  /* = */
#define KEYCODE_A                  65
#define KEYCODE_B                  66
#define KEYCODE_C                  67
#define KEYCODE_D                  68
#define KEYCODE_E                  69
#define KEYCODE_F                  70
#define KEYCODE_G                  71
#define KEYCODE_H                  72
#define KEYCODE_I                  73
#define KEYCODE_J                  74
#define KEYCODE_K                  75
#define KEYCODE_L                  76
#define KEYCODE_M                  77
#define KEYCODE_N                  78
#define KEYCODE_O                  79
#define KEYCODE_P                  80
#define KEYCODE_Q                  81
#define KEYCODE_R                  82
#define KEYCODE_S                  83
#define KEYCODE_T                  84
#define KEYCODE_U                  85
#define KEYCODE_V                  86
#define KEYCODE_W                  87
#define KEYCODE_X                  88
#define KEYCODE_Y                  89
#define KEYCODE_Z                  90
#define KEYCODE_LEFT_BRACKET       91  /* [ */
#define KEYCODE_BACKSLASH          92  /* \ */
#define KEYCODE_RIGHT_BRACKET      93  /* ] */
#define KEYCODE_GRAVE_ACCENT       96  /* ` */
#define KEYCODE_WORLD_1            161 /* non-US #1 */
#define KEYCODE_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KEYCODE_ESCAPE             256
#define KEYCODE_ENTER              257
#define KEYCODE_TAB                258
#define KEYCODE_BACKSPACE          259
#define KEYCODE_INSERT             260
#define KEYCODE_DELETE             261
#define KEYCODE_RIGHT              262
#define KEYCODE_LEFT               263
#define KEYCODE_DOWN               264
#define KEYCODE_UP                 265
#define KEYCODE_PAGE_UP            266
#define KEYCODE_PAGE_DOWN          267
#define KEYCODE_HOME               268
#define KEYCODE_END                269
#define KEYCODE_CAPS_LOCK          280
#define KEYCODE_SCROLL_LOCK        281
#define KEYCODE_NUM_LOCK           282
#define KEYCODE_PRINT_SCREEN       283
#define KEYCODE_PAUSE              284
#define KEYCODE_F1                 290
#define KEYCODE_F2                 291
#define KEYCODE_F3                 292
#define KEYCODE_F4                 293
#define KEYCODE_F5                 294
#define KEYCODE_F6                 295
#define KEYCODE_F7                 296
#define KEYCODE_F8                 297
#define KEYCODE_F9                 298
#define KEYCODE_F10                299
#define KEYCODE_F11                300
#define KEYCODE_F12                301
#define KEYCODE_F13                302
#define KEYCODE_F14                303
#define KEYCODE_F15                304
#define KEYCODE_F16                305
#define KEYCODE_F17                306
#define KEYCODE_F18                307
#define KEYCODE_F19                308
#define KEYCODE_F20                309
#define KEYCODE_F21                310
#define KEYCODE_F22                311
#define KEYCODE_F23                312
#define KEYCODE_F24                313
#define KEYCODE_F25                314
#define KEYCODE_KP_0               320
#define KEYCODE_KP_1               321
#define KEYCODE_KP_2               322
#define KEYCODE_KP_3               323
#define KEYCODE_KP_4               324
#define KEYCODE_KP_5               325
#define KEYCODE_KP_6               326
#define KEYCODE_KP_7               327
#define KEYCODE_KP_8               328
#define KEYCODE_KP_9               329
#define KEYCODE_KP_DECIMAL         330
#define KEYCODE_KP_DIVIDE          331
#define KEYCODE_KP_MULTIPLY        332
#define KEYCODE_KP_SUBTRACT        333
#define KEYCODE_KP_ADD             334
#define KEYCODE_KP_ENTER           335
#define KEYCODE_KP_EQUAL           336
#define KEYCODE_LEFT_SHIFT         340
#define KEYCODE_LEFT_CONTROL       341
#define KEYCODE_LEFT_ALT           342
#define KEYCODE_LEFT_SUPER         343
#define KEYCODE_RIGHT_SHIFT        344
#define KEYCODE_RIGHT_CONTROL      345
#define KEYCODE_RIGHT_ALT          346
#define KEYCODE_RIGHT_SUPER        347
#define KEYCODE_MENU               348

#define KEYCODE_LAST               KEYCODE_MENU