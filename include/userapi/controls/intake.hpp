#pragma once

namespace keybindActions::intake {
    static bool toggleHighGoalEnabled = false;
    static bool toggleMiddleGoalEnabled = false;
    static bool toggleIntakeInEnabled = false;
    static bool toggleOutEnabled = false;

    void toggleHighGoal();
    void toggleMiddleGoal();
    void toggleIntakeIn();
    void toggleOut();

    void highGoalStorage();
    void middleGoalOut();
    void intakeIn();
    void out();
    void stop();

    void resetToggle();
}