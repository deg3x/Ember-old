// We use the GLFW mouse button mappings

#define MOUSE_BTN_1         0
#define MOUSE_BTN_2         1
#define MOUSE_BTN_3         2
#define MOUSE_BTN_4         3
#define MOUSE_BTN_5         4
#define MOUSE_BTN_6         5
#define MOUSE_BTN_7         6
#define MOUSE_BTN_8         7
#define MOUSE_BTN_LAST      MOUSE_BTN_8
#define MOUSE_BTN_LEFT      MOUSE_BTN_1
#define MOUSE_BTN_RIGHT     MOUSE_BTN_2
#define MOUSE_BTN_MIDDLE    MOUSE_BTN_3

struct MouseData
{
    float sensitivity;
    bool leftButtonPressed;
    bool rightButtonPressed;
    bool middleButtonPressed;
    double posX;
    double posY;
    double leftMouseDragDeltaX;
    double leftMouseDragDeltaY;
    double rightMouseDragDeltaX;
    double rightMouseDragDeltaY;
};