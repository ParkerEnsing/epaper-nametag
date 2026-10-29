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
        static constexpr size_t MAX_PATH_LENGTH = 192;  

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

        char _textBuffer[768] = {};
        char _message[96] = {};
};

#endif