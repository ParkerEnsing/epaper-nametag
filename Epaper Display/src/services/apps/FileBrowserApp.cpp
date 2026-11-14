#include "services/apps/FileBrowserApp.h"

#include "services/AppContext.h"
#include "input/InputEvent.h"
#include "displays/ui/UITheme.h"

#include <lvgl.h>
#include <stdio.h>
#include <string.h>


namespace {
    // for local private functions
}


const char* FileBrowserApp::id() const {
    return "file_browser";
}


const char* FileBrowserApp::name() const {
    return "File Browser";
}


void FileBrowserApp::onInput(AppContext &context, const InputEvent &event) {
    switch (event.action) {
        case InputAction::Up:
            if (!_showingDetails && moveSelection(-1)) {
                renderView();
                context.requestCommit();
            }
            break;
        case InputAction::Down:
            if (!_showingDetails && moveSelection(1)) {
                renderView();
                context.requestCommit();
            }
            break;
        case InputAction::Select:
            if (!_showingDetails && openSelection()) {
                renderView();
                context.requestCommit();
            }
            break;
        case InputAction::Back:
            if (_showingDetails) {
                _showingDetails = false;
                renderView();
                context.requestCommit();
            }
            else if(strcmp(_currentPath, "/") == 0) {
                context.requestGoHome();
            }
            else {
                goToParent();
                renderView();
                context.requestCommit();
            }
            break;
        default:
            break;
    }
}


void FileBrowserApp::createScreen() {
    _screen = lv_obj_create(nullptr);
    lv_obj_add_style(_screen, UITheme::screen(), LV_PART_MAIN);

    _titleLabel = lv_label_create(_screen);
    lv_obj_add_style(_titleLabel, UITheme::titleLabel(), LV_PART_MAIN);
    lv_obj_align(_titleLabel, LV_ALIGN_TOP_MID, 0, 8);

    _pathLabel = lv_label_create(_screen);
    lv_obj_add_style(_pathLabel, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_set_width(_pathLabel, 760);
    lv_label_set_long_mode(_pathLabel, LV_LABEL_LONG_DOT);
    lv_obj_align(_pathLabel, LV_ALIGN_TOP_LEFT, 16, 40);

    _bodyLabel = lv_label_create(_screen);
    lv_obj_add_style(_bodyLabel, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_set_width(_bodyLabel, 760);
    lv_label_set_long_mode(_bodyLabel, LV_LABEL_LONG_WRAP);
    lv_obj_align(_bodyLabel, LV_ALIGN_TOP_LEFT, 16, 72);

    _footerLabel = lv_label_create(_screen);
    lv_obj_add_style(_footerLabel, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_align(_footerLabel, LV_ALIGN_BOTTOM_LEFT, 16, -10);
}


void FileBrowserApp::onScreenEnter(AppContext &context) {
    (void)context;

    _showingDetails = false;

    if (!StorageService::isMounted()) {
        _entryCount = 0;
        _hasMore = false;
        snprintf(_message, sizeof(_message), "SD card not mounted.");
    }
    else {
        const size_t previousSelection = _selectedRow;

        if (loadPage(_currentPath, _pageOffset)) {
            if (previousSelection < _entryCount) {
                _selectedRow = previousSelection;
            }

            _message[0] = '\0';
        }
        else {
            _entryCount = 0;
            _hasMore = false;
            snprintf(_message, sizeof(_message), "Directory unavailable.");
        }
    }

    renderView();
    // Display commit and refresh is handled through ScreenApp::onEnter()
}


bool FileBrowserApp::loadPage(const char* path, size_t offset) {
    StorageService::DirectoryEntry nextEntries[PAGE_SIZE];

    size_t nextCount = 0;
    bool nextHasMore = false;

    if (!StorageService::listDirectoryPage(
        path,
        nextEntries,
        PAGE_SIZE,
        offset,
        nextCount,
        nextHasMore
    )) {
        return false;
    }

    for (size_t i = 0; i < nextCount; i++) {
        _entries[i] = nextEntries[i];
    }

    _entryCount = nextCount;
    _hasMore = nextHasMore;
    _pageOffset = offset;
    _selectedRow = 0;

    return true;
}


bool FileBrowserApp::changeDirectory(const char* path) {
    if (path == nullptr) {
        snprintf(_message, sizeof(_message), "Path is invalid: nullptr.");
        return false;
    }

    if (strlen(path) >= sizeof(_currentPath)) {
        snprintf(_message, sizeof(_message), "Path is invalid: too long.");
        return false;
    }

    if (!loadPage(path, 0)) {
        snprintf(_message, sizeof(_message), "Cannot open directory.");
        return false;
    }

    strcpy(_currentPath, path);

    _showingDetails = false;
    _message[0] = '\0';
    
    return true;
}


bool FileBrowserApp::moveSelection(int32_t direction) {
    if (_entryCount == 0) {
        return false;
    }

    if (direction > 0) {
        if (_selectedRow + 1 < _entryCount) {
            _selectedRow++;
            _message[0] = '\0';
            return true;
        }

        if (_hasMore) {
            if (!loadPage(_currentPath, _pageOffset + _entryCount)) {
                snprintf(_message, sizeof(_message), "Page read failed.");
            }
            else {
                _message[0] = '\0';
            }

            return true;
        }
    }
    else if (direction < 0) {
        if (_selectedRow > 0) {
            _selectedRow--;
            _message[0] = '\0';
            return true;
        }

        if (_pageOffset > 0) {
            const size_t previousOffset = (_pageOffset >= PAGE_SIZE) ? _pageOffset - PAGE_SIZE : 0;

            if (!loadPage(_currentPath, previousOffset)) {
                snprintf(_message, sizeof(_message), "Page read failed.");
            }
            else {
                _selectedRow = (_entryCount > 0) ? _entryCount - 1 : 0;
                _message[0] = '\0';
            }

            return true;
        }
    }

    return false;
}


bool FileBrowserApp::openSelection() {
    if (_selectedRow >= _entryCount) {
        return false;
    }

    const StorageService::DirectoryEntry &entry = _entries[_selectedRow];

    if (entry.nameTruncated) {
        snprintf(_message, sizeof(_message), "Filename invalid: too long.");
        return true;
    }

    if (!entry.isDirectory) {
        _selectedFile = entry;
        _showingDetails = true;
        _message[0] = '\0';
        return true;
    }

    if (strcmp(entry.name, ".") == 0 || strcmp(entry.name, "..") == 0) {
        return false;
    }

    char nextPath[MAX_PATH_LENGTH];

    const int written = strcmp(_currentPath, "/") == 0
        ? snprintf(
            nextPath,
            sizeof(nextPath),
            "/%s",
            entry.name
        )
        : snprintf(
            nextPath,
            sizeof(nextPath),
            "%s/%s",
            _currentPath,
            entry.name
        );
    
    if (written < 0) {
        snprintf(_message, sizeof(_message), "Path invalid: character encoding failure.");
        return true;
    }
    
    if (static_cast<size_t>(written) >= sizeof(nextPath)) {
        snprintf(_message, sizeof(_message), "Path invalid: too long.");
        return true;
    }

    changeDirectory(nextPath);
    return true;
}


bool FileBrowserApp::goToParent() {
    if (strcmp(_currentPath, "/") == 0) {
        return false;
    }

    char parentPath[MAX_PATH_LENGTH];

    snprintf(parentPath, sizeof(parentPath), "%s", _currentPath);

    char* lastSlash = strrchr(parentPath, '/');

    if (lastSlash == nullptr) {
        return false;
    }

    if (lastSlash == parentPath) {
        parentPath[1] = '\0';
    }
    else {
        *lastSlash = '\0';
    }
    
    Serial.print("Current path: ");
    Serial.println(_currentPath);

    Serial.print("Calculated parent: ");
    Serial.println(parentPath);
    changeDirectory(parentPath);
    return true;
}


void FileBrowserApp::renderView() {
    lv_label_set_text(_titleLabel, _showingDetails ? "FILE DETAILS" : "FILE BROWSER");
    lv_label_set_text_fmt(_pathLabel, "PATH: %s", _currentPath);

    if (_showingDetails) {
        snprintf(
            _textBuffer,
            sizeof(_textBuffer),
            "Name: %s\n"
            "Type: File\n"
            "Size: %llu bytes",
            _selectedFile.name,
            static_cast<unsigned long long>(_selectedFile.sizeBytes)
        );

        lv_label_set_text(_bodyLabel, _textBuffer);
        lv_label_set_text(_footerLabel, "Back: return to files");
        return;
    }

    if (_entryCount == 0) {
        lv_label_set_text(_bodyLabel, "(No entries to display)");
    }
    else {
        size_t used = 0;
        _textBuffer[0] = '\0';

        for (size_t i = 0; i < _entryCount; i++) {
            const StorageService::DirectoryEntry &entry = _entries[i];
            const char marker = (i == _selectedRow) ? '>' : ' ';
            const char type = entry.isDirectory ? 'D' : 'F';
            const bool shortenDisplay = strlen(entry.name) > DISPLAY_FILENAME_BYTES;
            const int written = snprintf(
                _textBuffer + used,
                sizeof(_textBuffer) - used,
                "%c [%c] %.*s%s\n",
                marker,
                type,
                static_cast<int>(DISPLAY_FILENAME_BYTES),
                entry.name,
                shortenDisplay ? "..." : ""
            );
            
            if (written < 0) {
                break;
            }

            if (static_cast<size_t>(written) >= sizeof(_textBuffer) - used) {
                break;
            }

            used += static_cast<size_t>(written);
        }

        lv_label_set_text(_bodyLabel, _textBuffer);
    }

    if (_message[0] != '\0') {
        lv_label_set_text(_footerLabel, _message);
    }
    else {
        lv_label_set_text(_footerLabel, "Up/Down: Move    Select: Open    Back: Parent");
    }
}