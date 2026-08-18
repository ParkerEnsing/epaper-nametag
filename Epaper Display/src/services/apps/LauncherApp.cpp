#include "services/apps/LauncherApp.h"

#include "services/AppContext.h"
#include "services/AppRegistry.h"
#include "displays/ui/UITheme.h"

#include "input/InputEvent.h"

#include <lvgl.h>
#include <stdio.h>
#include <string.h>


namespace {
    void appendText(char* buffer,
        size_t bufferSize,
        size_t &offset,
        const char* format,
        const char* marker,
        const char* name,
        const char* id
    ) {
        if (offset >= bufferSize) {
            return;
        }

        const int written = snprintf(
            buffer + offset,
            bufferSize - offset,
            format,
            marker,
            name,
            id
        );

        if (written < 0) {
            return;
        }

        const size_t remaining = bufferSize - offset;

        if (static_cast<size_t>(written) >= remaining) {
            offset = bufferSize - 1;
            buffer[offset] = '\0';
            return;
        }

        offset += static_cast<size_t>(written);
    }
}


const char* LauncherApp::id() const {
    return "launcher";
}


const char* LauncherApp::name() const {
    return "Launcher";
}


void LauncherApp::onEnter(AppContext &context) {
    if (_screen == nullptr) {
        createScreen();
    }
    
    const size_t appCount = AppRegistry::count();

    if (appCount == 0) {
        _selectedIndex = 0;
    }
    else if (_selectedIndex >= appCount) {
        _selectedIndex = appCount - 1;
    }

    updateListText();
    context.requestCommit();
}


void LauncherApp::onInput(AppContext &context, const InputEvent &event) {
    switch (event.action) {
    case InputAction::Up:
        moveSelection(-1);
        updateListText();
        context.requestCommit();
        break;
    case InputAction::Down:
        moveSelection(1);
        updateListText();
        context.requestCommit();
        break;
    case InputAction::Select:
        launchSelected(context);
        break;
    case InputAction::Back:
        //break;
    case InputAction::Home:
        context.requestGoHome();
        break;
    case InputAction::StartSlideshow:
        context.requestStartSlideshow();
        break;
    default:
        break;
    }
}


lv_obj_t* LauncherApp::screen() const {
    return _screen;
}


void LauncherApp::createScreen() {
    _screen = lv_obj_create(nullptr);
    lv_obj_add_style(_screen, UITheme::screen(), LV_PART_MAIN);

    _titleLabel = lv_label_create(_screen);
    lv_label_set_text(_titleLabel, "App Launcher");
    lv_obj_add_style(_titleLabel, UITheme::titleLabel(), LV_PART_MAIN);
    lv_obj_align(_titleLabel, LV_ALIGN_TOP_MID, 0, 8);

    _listLabel = lv_label_create(_screen);
    lv_obj_add_style(_listLabel, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_set_width(_listLabel, 760);
    lv_label_set_long_mode(_listLabel, LV_LABEL_LONG_WRAP);
    lv_obj_align(_listLabel, LV_ALIGN_TOP_LEFT, 16, 48);

    _hintLabel = lv_label_create(_screen);
    lv_label_set_text(_hintLabel, "Input not connected yet");
    lv_obj_add_style(_hintLabel, UITheme::bodyLabel(), LV_PART_MAIN);
    lv_obj_align(_hintLabel, LV_ALIGN_BOTTOM_MID, 0, -12);
}


void LauncherApp::updateListText() {
    size_t offset = 0;

    snprintf(_textBuffer, sizeof(_textBuffer), "Registered apps: \n\n");

    offset = strlen(_textBuffer);

    const size_t appCount = AppRegistry::count();

    if (appCount == 0) {
        snprintf(_textBuffer, sizeof(_textBuffer), "No apps registerd.");
        lv_label_set_text(_listLabel, _textBuffer);
        return;
    }

    for (size_t i = 0; i < appCount; i++) {
        App* app = AppRegistry::getByIndex(i);

        const char* marker = (i == _selectedIndex) ? "> " : "  ";

        const char* appName = (app != nullptr) ? app->name() : "(null)";

        const char* appId = (app != nullptr) ? app->id() : "null";

        appendText(
            _textBuffer,
            sizeof(_textBuffer),
            offset,
            "%s%s [%s]\n",
            marker,
            appName,
            appId
        );
    }

    lv_label_set_text(_listLabel, _textBuffer);
}


void LauncherApp::moveSelection(uint32_t delta) {
    const size_t appCount = AppRegistry::count();

    if (appCount == 0) {
        _selectedIndex = 0;
        return;
    }

    int32_t nextIndex = static_cast<int32_t>(_selectedIndex) + delta;

    const int32_t count = static_cast<int32_t>(appCount);

    while (nextIndex < 0) {
        nextIndex += count;
    }

    while (nextIndex >= count) {
        nextIndex -= count;
    }

    _selectedIndex = static_cast<size_t>(nextIndex);
}


void LauncherApp::launchSelected(AppContext &context) {
    const size_t appCount = AppRegistry::count();

    if (appCount == 0) {
        return;
    }

    if (_selectedIndex >= appCount) {
        _selectedIndex = appCount - 1;
    }

    App* selectedApp = AppRegistry::getByIndex(_selectedIndex);

    if (selectedApp == nullptr) {
        return;
    }

    /*
        The launcher currently appears in the registry and therefore
        appears in its own list. Selecting it does nothing for now.

        Later, metadata can be added, such as:
            showInLauncer()
            isSystemApp()
            icon()
    */
    if (selectedApp == this) {
        context.requestCommit();
        return;
    }

    context.requestLaunchApp(selectedApp->id());
}