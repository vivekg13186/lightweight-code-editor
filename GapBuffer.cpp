#include "GapBuffer.h"

void GapBuffer::insert(wchar_t c) {
        handleLineBreak(c);
        if (gap_start == gap_end) {
            expandBuffer();
        }
        buffer[gap_start++] = c;
    }

    void GapBuffer::insertText(const std::wstring& text) {
        for (size_t i = 0; i < text.size(); ++i) {
            handleLineBreak(text[i]);
            if (text[i] == L'\r' && (i + 1) < text.size() && text[i + 1] == L'\n') {
                insert(L'\n');
                ++i;  // Skip the '\n' since we treat \r\n as a single unit
            } else {
                insert(text[i]);
            }
        }
    }

    void GapBuffer::deleteBeforeCursor() {
        if (gap_start > 0) {
            wchar_t c = buffer[--gap_start];
            if (c == L'\n' && gap_start > 0 && buffer[gap_start - 1] == L'\r') {
                --gap_start;  // Handle \r\n as a single unit
            }
        }
    }

    void GapBuffer::deleteAfterCursor() {
        if (gap_end < buffer.size()) {
            wchar_t c = buffer[gap_end];
            if (c == L'\r' && (gap_end + 1) < buffer.size() && buffer[gap_end + 1] == L'\n') {
                ++gap_end;  // Handle \r\n as a single unit
            }
            ++gap_end;
        }
    }

    void GapBuffer::moveCursorLeft() {
        if (gap_start > 0) {
            if (gap_start > 1 && buffer[gap_start - 1] == L'\n' && buffer[gap_start - 2] == L'\r') {
                gap_start -= 2;  // Move across \r\n as a single unit
            } else {
                gap_start--;
            }
            buffer[--gap_end] = buffer[gap_start];
        }
    }

    void GapBuffer::moveCursorRight() {
        if (gap_end < buffer.size()) {
            if (gap_end + 1 < buffer.size() && buffer[gap_end] == L'\r' && buffer[gap_end + 1] == L'\n') {
                gap_end += 2;  // Move across \r\n as a single unit
                buffer[gap_start++] = L'\r';
                buffer[gap_start++] = L'\n';
            } else {
                buffer[gap_start++] = buffer[gap_end++];
            }
        }
    }

    std::wstring GapBuffer::getText() const {
        std::wstring text;
        text.reserve(buffer.size() - (gap_end - gap_start));
        for (int i = 0; i < gap_start; ++i) {
            text += buffer[i];
        }
        for (int i = gap_end; i < buffer.size(); ++i) {
            text += buffer[i];
        }
        return text;
    }

    std::wstring GapBuffer::getVisibleText(int scroll_y, int scroll_x, int screen_width, int screen_height) const {
        std::wstring full_text = getText();
        std::vector<std::wstring> lines = splitLines(full_text);

        std::wstring visible_text;
        for (int i = scroll_y; i < std::min(scroll_y + screen_height, static_cast<int>(lines.size())); ++i) {
            if (lines[i].size() > scroll_x) {
                visible_text += lines[i].substr(scroll_x, screen_width) + L'\n';
            } else {
                visible_text += L'\n';
            }
        }
        return visible_text;
    }

    std::wstring GapBuffer::getSelectedText(int start, int end) const {
        std::wstring selected_text;
        for (int i = start; i < end; ++i) {
            if (i < gap_start) {
                selected_text += buffer[i];
            } else {
                selected_text += buffer[gap_end + (i - gap_start)];
            }
        }
        return selected_text;
    }

    int GapBuffer::getCursorPos() const {
        return gap_start;
    }

    void GapBuffer::moveCursorTo(int pos) {
        while (gap_start < pos) {
            moveCursorRight();
        }
        while (gap_start > pos) {
            moveCursorLeft();
        }
    }

    void GapBuffer::moveCursorToStart() {
        while (gap_start > 0) {
            moveCursorLeft();
        }
    }

    void GapBuffer::moveCursorToEnd() {
        while (gap_end < buffer.size()) {
            moveCursorRight();
        }
    }

    void GapBuffer::deleteSelectedText(int start, int end) {
        if (start < end) {
            int length = end - start;
            while (length-- > 0) {
                moveCursorTo(start);
                deleteAfterCursor();
            }
        }
    }

    int GapBuffer::findNextOccurrence(const std::wstring& word, int start_pos) {
        std::wstring text = getText();
        size_t found_pos = text.find(word, start_pos);
        return (found_pos == std::wstring::npos) ? -1 : static_cast<int>(found_pos);
    }

    int GapBuffer::getLineCount() const {
        std::wstring text = getText();
        return std::count(text.begin(), text.end(), L'\n') + 1;
    }

    void GapBuffer::loadFromFile(const std::string& filename) {
        std::wifstream file(filename);
        file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
        std::wstring line;
        while (std::getline(file, line)) {
            insertText(line + L'\n');
        }
        file.close();
    }



    void GapBuffer::expandBuffer() {
        int new_size = buffer.size() * 2;
        std::vector<wchar_t> new_buffer(new_size, L'\0');

        for (int i = 0; i < gap_start; ++i) {
            new_buffer[i] = buffer[i];
        }
        int new_gap_end = new_size - (buffer.size() - gap_end);
        for (int i = gap_end; i < buffer.size(); ++i) {
            new_buffer[new_gap_end++] = buffer[i];
        }
        gap_end = new_gap_end;
        buffer = std::move(new_buffer);
    }

    void GapBuffer::handleLineBreak(wchar_t c) {
        if (c == L'\r' || c == L'\n') {
            insert(L'\n');  // Normalize line breaks to just '\n'
        }
    }

    std::vector<std::wstring> GapBuffer::splitLines(const std::wstring& text) const {
        std::vector<std::wstring> lines;
        std::wstring line;
        for (wchar_t c : text) {
            if (c == L'\n') {
                lines.push_back(line);
                line.clear();
            } else {
                line += c;
            }
        }
        if (!line.empty()) {
            lines.push_back(line);
        }
        return lines;
    }
