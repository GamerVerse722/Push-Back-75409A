#include "userapi/ui/autom/autom_handler.hpp"

#include <cstddef>
#include <format>
#include <string_view>

#include "gamers-forge/proslogger.hpp"

namespace ui::autom::handler {
    static PROSLogger::Logger log{"Autom Handler"};

    // Fixed size callback arrays
    static std::array<callback_method, 5> qualification_methods {};
    static std::array<callback_method, 5> elimination_methods {};
    static callback_method skills_callback {nullptr};

    // Current Selection
    AutomMode active_mode = AutomMode::NONE;
    AutomSideColor active_side = AutomSideColor::NO_COLOR_AND_POSITION;
    AutomPosition active_position = AutomPosition::NO_POSITION;
    AutomColor active_color = AutomColor::COLOR_NONE;
    callback_method active_callback = nullptr;

    // Helper Conversion
    AutomSideColor get_side_color(AutomColor color, AutomPosition pos) {
        switch (color) {
            case AutomColor::RED: return (pos == AutomPosition::LEFT) ? AutomSideColor::RED_LEFT : AutomSideColor::RED_RIGHT;
            case AutomColor::BLUE: return (pos == AutomPosition::LEFT) ? AutomSideColor::BLUE_LEFT : AutomSideColor::BLUE_RIGHT;
            case AutomColor::COLOR_NONE: return AutomSideColor::NO_COLOR_AND_POSITION;
            default: return AutomSideColor::NO_COLOR_AND_POSITION;
        }
    }

    AutomPosition get_position_from_side_color(AutomSideColor sc) {
        switch (sc) {
            case AutomSideColor::RED_LEFT:
            case AutomSideColor::BLUE_LEFT: return AutomPosition::LEFT;

            case AutomSideColor::RED_RIGHT:
            case AutomSideColor::BLUE_RIGHT: return AutomPosition::RIGHT;

            case AutomSideColor::NO_COLOR_AND_POSITION: return AutomPosition::NO_POSITION;
            default: return AutomPosition::NO_POSITION;
        }
    }

    AutomColor get_color_from_side_color(AutomSideColor sc) {
        switch (sc) {
            case AutomSideColor::RED_LEFT:
            case AutomSideColor::RED_RIGHT: return AutomColor::RED;

            case AutomSideColor::BLUE_LEFT:
            case AutomSideColor::BLUE_RIGHT: return AutomColor::BLUE;

            case AutomSideColor::NO_COLOR_AND_POSITION: return AutomColor::COLOR_NONE;
            default: return AutomColor::COLOR_NONE;
        }
    }

    std::string_view automModeToString(AutomMode mode) {
        switch (mode) {
            case AutomMode::QUALIFICATIONS: return "Qualifications";
            case AutomMode::ELIMINATIONS: return "Eliminations";
            case AutomMode::SKILLS: return "Skills";
            case AutomMode::NONE: return "None";
            default: return "Unknown";
        }
    }

    std::string_view automColorToString(AutomColor color) {
        switch (color) {
            case AutomColor::RED: return "Red";
            case AutomColor::BLUE: return "Blue";
            case AutomColor::COLOR_NONE: return "None";
            default: return "Unknown";
        }
    }

    std::string_view automPositionToString(AutomPosition pos) {
        switch (pos) {
            case AutomPosition::LEFT: return "Left";
            case AutomPosition::RIGHT: return "Right";
            case AutomPosition::NO_POSITION: return "None";
            default: return "Unknown";
        }
    }

    std::string_view automPositionColorToString(AutomSideColor sc) {
        switch (sc) {
            case AutomSideColor::RED_LEFT: return "Red Left";
            case AutomSideColor::RED_RIGHT: return "Red Right";
            case AutomSideColor::BLUE_LEFT: return "Blue Left";
            case AutomSideColor::BLUE_RIGHT: return "Blue Right";
            case AutomSideColor::NO_COLOR_AND_POSITION: return "None";
            default: return "Unknown";
        }
    }

    void register_callback_method(AutomMode mode, AutomColor color, AutomPosition position, callback_method callback) {
        AutomSideColor side_color = get_side_color(color, position);
        size_t index = static_cast<size_t>(side_color);

        switch (mode) {
            case AutomMode::QUALIFICATIONS:
                if (qualification_methods[index]) { log.warn("Overwriting existing QUALIFICATION callback"); }
                else { log.debug("Registering QUALIFICATION callback"); }

                qualification_methods[index] = callback;
                break;

            case AutomMode::ELIMINATIONS:
                if (elimination_methods[index]) { log.warn("Overwriting existing ELIMINATION callback"); }
                else { log.debug("Registering ELIMINATION callback"); }

                elimination_methods[index] = callback;
                break;

            case AutomMode::SKILLS:
                if (skills_callback) { log.warn("Overwriting existing SKILLS callback"); }
                else { log.debug("Registering SKILLS callback"); }

                skills_callback = callback;
                break;

            default:
                log.warn("Attempted registration with AutomMode::NONE");
            }
    }

    void select_autom(AutomMode mode, AutomSideColor side_color) {
        active_mode = mode;
        active_side = side_color;
        active_position = get_position_from_side_color(side_color);
        active_color = get_color_from_side_color(side_color);

        size_t index = static_cast<size_t>(side_color);
        switch (mode) {
            case AutomMode::QUALIFICATIONS: active_callback = qualification_methods[index]; break;
            case AutomMode::ELIMINATIONS: active_callback = elimination_methods[index]; break;
            case AutomMode::SKILLS: active_callback = skills_callback; break;
            default: active_callback = nullptr; break;

            if (!active_callback) {
                log.warn("Attempted to select AutomMode::NONE");
                return;
            }

            log.info(std::format("Selected {} {}", automModeToString(mode), automPositionColorToString(side_color)));
        }
    }

    void run_automous() {
        if (active_callback==nullptr) {
            log.warn(std::format("Automous mode {} has no callback", automModeToString(active_mode)));
            return;
        }
        log.info(std::format("Running {}", automModeToString(active_mode)));
        active_callback();
    }
}