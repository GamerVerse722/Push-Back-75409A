#include "userapi/ui/autom.hpp"

#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_event.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/core/lv_obj_style_gen.h"
#include "liblvgl/font/lv_font.h"
#include "liblvgl/layouts/grid/lv_grid.h"
#include "liblvgl/misc/lv_event.h"
#include "liblvgl/misc/lv_palette.h"
#include "liblvgl/misc/lv_style.h"
#include "liblvgl/misc/lv_style_gen.h"
#include "liblvgl/misc/lv_types.h"
#include "liblvgl/widgets/button/lv_button.h"
#include "liblvgl/widgets/label/lv_label.h"
#include "autom.hpp"
#include "op_control.hpp"
#include <cstddef>
#include <format>

namespace ui::autom_selector {
    lv_obj_t *autom_screen = lv_obj_create(NULL);
    std::function<void()> selected_callback = nullptr;
    std::map<AutomMode, callback_method> callback_map;

    void initialize() {
        log.info("Starting autom selector menu");

        /* Container for the grid */
        lv_obj_set_size(autom_screen, 480, 240);
        lv_obj_center(autom_screen);

        lv_obj_set_style_pad_all(autom_screen, 0, 0);
        lv_obj_set_style_pad_gap(autom_screen, 2, 0);
        lv_obj_set_style_border_width(autom_screen, 0, 0);

        /* Grid layout: 2 columns × 3 rows */
        static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        lv_obj_set_grid_dsc_array(autom_screen, col_dsc, row_dsc);
        lv_obj_set_layout(autom_screen, LV_LAYOUT_GRID);

        // Register Button
        log.debug("Registering Buttons");

        register_button("Left", 0, 0, AutomMode::RED_LEFT, AutomColor::RED, nullptr);
        register_button("None", 0, 1, AutomMode::NONE, AutomColor::COLOR_NONE,nullptr);
        register_button("Right", 0, 2, AutomMode::RED_RIGHT,AutomColor::RED, nullptr);

        register_button("Right", 1, 0, AutomMode::BLUE_RIGHT,AutomColor::BLUE, nullptr);
        register_button("Skills", 1, 1, AutomMode::SKILLS,AutomColor::COLOR_NONE, nullptr);
        register_button("Left", 1, 2, AutomMode::BLUE_LEFT,AutomColor::BLUE, nullptr);
    }

    /* Event handler function */
    static void button_event_handler(lv_event_t* e) {
        // void* user_data = lv_event_get_user_data(e);
        AutomMode mode = (AutomMode)(intptr_t)lv_event_get_user_data(e);
        // AutomMode mode = *(AutomMode*)lv_event_get_user_data(e);
        selected_autom = mode;

        if (mode == AutomMode::RED_LEFT || mode == AutomMode::RED_RIGHT) {selected_color = AutomColor::RED;}
        else if (mode == AutomMode::BLUE_LEFT || mode == AutomMode::BLUE_RIGHT) {selected_color = AutomColor::BLUE;}
        else {selected_color = AutomColor::COLOR_NONE;}

        log.info(std::format("Button {} was selected", automModeToString(mode)));
        lv_screen_load_anim(ui::driver::driver_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
    }

    void register_button(std::string text, int col, int row, AutomMode mode, AutomColor color, callback_method callback) {
        callback_map[mode] = callback; 

        lv_obj_t* btn = lv_button_create(autom_screen);
        lv_obj_set_grid_cell(
            btn,
            LV_GRID_ALIGN_STRETCH, col, 1,
            LV_GRID_ALIGN_STRETCH, row, 1
        );

        // Style Button
        lv_color_t bg_color;
        if (color == AutomColor::RED) {bg_color = lv_palette_main(LV_PALETTE_RED);}
        else if (color == AutomColor::BLUE) {bg_color = lv_palette_main(LV_PALETTE_BLUE);}
        else {bg_color = lv_color_white();}

        lv_obj_set_style_bg_color(btn, bg_color, LV_PART_MAIN);
        lv_obj_set_style_radius(btn, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);

        // Button Label
        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, text.c_str());
        lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);

        // Set font style
        static lv_style_t label_style;
        lv_style_init(&label_style);
        lv_style_set_text_font(&label_style, &lv_font_montserrat_30);
        lv_obj_add_style(label, &label_style, LV_PART_MAIN);

        // Add Callback
        lv_obj_add_event_cb(btn, button_event_handler, LV_EVENT_PRESSED, (void*)mode);

        lv_obj_center(label);
        log.debug("Registered " + automModeToString(mode));
    }

    void run_automous() {
        log.info(std::format("Running {}", automModeToString(selected_autom)));
        selected_callback();
    }
}
