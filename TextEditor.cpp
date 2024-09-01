#include "TextEditor.h"

    void TextEditor::handleKeyPress(wchar_t key) {
        if (key == L'\033') {  // Escape key or special sequence start
            return;  // Handle escape sequences in another function
        }

        if (selection_active && key != 127 && key != 8) {
            gap_buffer.deleteSelectedText(selection_start, selection_end);
            selection_active = false;
        }

        switch (key) {
            case 127:  // Backspace
                if (selection_active) {
                    gap_buffer.deleteSelectedText(selection_start, selection_end);
                    selection_active = false;
                } else {
                    gap_buffer.deleteBeforeCursor();
                }
                break;
            case 8:  // Delete
                if (selection_active) {
                    gap_buffer.deleteSelectedText(selection_start, selection_end);
                    selection_active = false;
                } else {
                    gap_buffer.deleteAfterCursor();
                }
                break;
            default:
                gap_buffer.insert(key);
                break;
        }
        updateCursor();
    }

    void TextEditor::handleCtrlKey(wchar_t key) {
        if (selection_active) {
            switch (key) {
                case L'c':  // Ctrl+C
                    clipboard = gap_buffer.getSelectedText(selection_start, selection_end);
                    break;
                case L'x':  // Ctrl+X
                    clipboard = gap_buffer.getSelectedText(selection_start, selection_end);
                    gap_buffer.deleteSelectedText(selection_start, selection_end);
                    selection_active = false;
                    break;
                case L'v':  // Ctrl+V
                    gap_buffer.deleteSelectedText(selection_start, selection_end);
                    gap_buffer.insertText(clipboard);
                    selection_active = false;
                    break;
                case L'd':  // Ctrl+D
                    multicursor_positions.insert(gap_buffer.getCursorPos());
                    int next_occurrence = gap_buffer.findNextOccurrence(clipboard, gap_buffer.getCursorPos() + 1);
                    if (next_occurrence != -1) {
                        gap_buffer.moveCursorTo(next_occurrence);
                    }
                    break;
            }
        }
    }

    void TextEditor::handleArrowKey(bool is_vertical, bool is_positive) {
        if (is_vertical) {
            if (is_positive) {
                moveCursorDown();
            } else {
                moveCursorUp();
            }
        } else {
            if (is_positive) {
                moveCursorRight();
            } else {
                moveCursorLeft();
            }
        }
        updateCursor();
    }

    void TextEditor::handleScroll(bool is_vertical, bool is_positive) {
        if (is_vertical) {
            if (is_positive && scroll_y + screen_height < gap_buffer.getLineCount()) {
                scroll_y++;
            } else if (!is_positive && scroll_y > 0) {
                scroll_y--;
            }
        } else {
            if (is_positive && cursor_x >= screen_width) {
                scroll_x++;
            } else if (!is_positive && scroll_x > 0) {
                scroll_x--;
            }
        }
    }

    void TextEditor::render() const {
        std::wstring visible_text = gap_buffer.getVisibleText(scroll_y, scroll_x, screen_width, screen_height);
        std::vector<std::wstring> lines = gap_buffer.splitLines(visible_text);
        for (size_t i = 0; i < lines.size(); ++i) {
            std::wcout << (scroll_y + i + 1) << L": " << lines[i] << std::endl;
        }
        std::wcout << L"Cursor at (" << cursor_x << ", " << cursor_y << L")\n";
    }

    void TextEditor::pressShift(bool shift) {
        shift_pressed = shift;
    }


    void TextEditor::moveCursorLeft() {
        if (cursor_x > 0) {
            cursor_x--;
            gap_buffer.moveCursorLeft();
        } else if (scroll_x > 0) {
            scroll_x--;
        }
    }

    void TextEditor::moveCursorRight() {
        if (cursor_x < screen_width - 1) {
            cursor_x++;
            gap_buffer.moveCursorRight();
        } else {
            scroll_x++;
        }
    }

    void  TextEditor::moveCursorUp() {
        if (cursor_y > 0) {
            cursor_y--;
            gap_buffer.moveCursorTo(gap_buffer.getCursorPos() - (cursor_x + 1 + scroll_x));
        } else if (scroll_y > 0) {
            scroll_y--;
        }
    }

    void TextEditor::moveCursorDown() {
        if (cursor_y < screen_height - 1) {
            cursor_y++;
            gap_buffer.moveCursorTo(gap_buffer.getCursorPos() + cursor_x + 1 + scroll_x);
        } else {
            scroll_y++;
        }
    }

    void TextEditor::updateCursor() {
        cursor_y = gap_buffer.getCursorPos() / screen_width;
        cursor_x = gap_buffer.getCursorPos() % screen_width;
    }
