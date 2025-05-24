#pragma once

namespace keybindActions::intake {
    static bool toggleInEnabled = false;
    static bool toggleOutEnabled = false;

    void toggleIn();
    void toggleOut();

    void in();
    void out();
    void stop();

    void resetToggle();
}