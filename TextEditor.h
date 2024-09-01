#include "GapBuffer.h"
#include <set>
#include <iostream>
class TextEditor {
public:
    TextEditor(int initial_size = 10, int screen_w = 80, int screen_h = 24)
        : gap_buffer(initial_size), screen_width(screen_w), screen_height(screen_h),
          scroll_x(0), scroll_y(0), cursor_x(0), cursor_y(0), selection_active(false),
          selection_start(0), selection_end(0), shift_pressed(false) {}

    void handleKeyPress(wchar_t key) ;

    void handleCtrlKey(wchar_t key);

    void handleArrowKey(bool is_vertical, bool is_positive);

    void handleScroll(bool is_vertical, bool is_positive) ;

    void render() const;

    void pressShift(bool shift);
private:
    GapBuffer gap_buffer;
    int screen_width;
    int screen_height;
    int scroll_x;
    int scroll_y;
    int cursor_x;
    int cursor_y;
    bool selection_active;
    int selection_start;
    int selection_end;
    bool shift_pressed;
    std::wstring clipboard;
    std::set<int> multicursor_positions;

    void moveCursorLeft() ;

    void moveCursorRight() ;

    void moveCursorUp();

    void moveCursorDown();

    void updateCursor() ;
};
