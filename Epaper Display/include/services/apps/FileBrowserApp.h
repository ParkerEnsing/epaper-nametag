#ifndef FILE_BROWSER_APP_H
#define FILE_BROWSER_APP_H

#include "services/ScreenApp.h"
#include "services/StorageService.h"

#include <stddef.h>
#include <stdint.h>


class FileBrowserApp : public ScreenApp {
    public:
        const char* id() const override;
        const char* name() const override;

        void onInput(AppContext &context, const InputEvent &event) override;
    private:
        static constexpr size_t PAGE_SIZE = 6;
        static constexpr size_t MAX_PATH_LENGTH = 192; // reduce to optimize, increase if paths are consistently too long
        static constexpr size_t DISPLAY_FILENAME_BYTES = 44; // mostly arbitrary; adjust based on fontsize and desired visual row length
        static constexpr size_t ROW_BUFFER_BYTES = 64; // current formatting needs 54 bytes; leaves overhead for format changes
        static constexpr size_t TEXT_BUFFER_BYTES = PAGE_SIZE * ROW_BUFFER_BYTES + 1; // how much text can fit in _textBuffer
        static constexpr size_t MAX_STATUS_MESSAGE_BYTES = 96; // buffer size for warnings and error messages

        void createScreen() override;
        void onScreenEnter(AppContext &context) override;

        bool loadPage(const char* path, size_t offset);
        bool changeDirectory(const char* path);

        bool moveSelection(int32_t direction);
        bool openSelection();
        bool goToParent();

        void renderView();

        lv_obj_t* _titleLabel = nullptr;
        lv_obj_t* _pathLabel = nullptr;
        lv_obj_t* _bodyLabel = nullptr;
        lv_obj_t* _footerLabel = nullptr;

        char _currentPath[MAX_PATH_LENGTH] = "/";

        StorageService::DirectoryEntry _entries[PAGE_SIZE];
        StorageService::DirectoryEntry _selectedFile;

        size_t _entryCount = 0;
        size_t _pageOffset = 0;
        size_t _selectedRow = 0;

        bool _hasMore = false;
        bool _showingDetails = false;

        char _textBuffer[TEXT_BUFFER_BYTES] = {};
        char _message[MAX_STATUS_MESSAGE_BYTES] = {};
};

#endif