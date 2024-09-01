#include <string>
#include <vector>
class GapBuffer {
public:
    GapBuffer(int initial_size = 10)
        : gap_start(0), gap_end(initial_size), buffer(initial_size, L'\0') {}

    void insert(wchar_t c) ;
    void insertText(const std::wstring& text);
    void deleteBeforeCursor();
    void deleteAfterCursor() ;
    void moveCursorLeft();
    void moveCursorRight() ;
    std::wstring getText() const ;
    std::wstring getVisibleText(int scroll_y, int scroll_x, int screen_width, int screen_height) const ;
    std::wstring getSelectedText(int start, int end) const;
    int getCursorPos() const ;
    void moveCursorTo(int pos) ;
    void moveCursorToStart() ;
    void moveCursorToEnd() ;
    void deleteSelectedText(int start, int end) ;
    int findNextOccurrence(const std::wstring& word, int start_pos);
    int getLineCount() const ;
    void loadFromFile(const std::string& filename);
private:
    std::vector<wchar_t> buffer;
    int gap_start;
    int gap_end;
    void expandBuffer();
    void handleLineBreak(wchar_t c) ;
    std::vector<std::wstring> splitLines(const std::wstring& text) const;
};
