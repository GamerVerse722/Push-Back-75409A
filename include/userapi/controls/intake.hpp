#pragma once

namespace keybindActions::intake {
    static bool toggleHighGoalEnabled = false;
    static bool toggleMiddleGoalEnabled = false;
    static bool toggleOutEnabled = false;

    void toggleHighGoal();
    void toggleMiddleGoal();
    void toggleOut();

    void highGoalStorage();
    void middleGoalOut();
    void out();
    void stop();

    void resetToggle();
}