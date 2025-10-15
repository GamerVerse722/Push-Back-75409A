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
#include "ui.hpp"
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

        register_button("Left", 0, 0, lv_palette_main(LV_PALETTE_RED), AutomMode::RED_LEFT, nullptr);
        register_button("None", 0, 1, lv_color_white(), AutomMode::NONE, nullptr);
        register_button("Right", 0, 2, lv_palette_main(LV_PALETTE_RED), AutomMode::RED_RIGHT, nullptr);

        register_button("Right", 1, 0, lv_palette_main(LV_PALETTE_BLUE), AutomMode::BLUE_RIGHT, nullptr);
        register_button("Skills", 1, 1, lv_color_white(), AutomMode::SKILLS, nullptr);
        register_button("Left", 1, 2, lv_palette_main(LV_PALETTE_BLUE), AutomMode::BLUE_LEFT, nullptr);
    }

    /* Event handler function */
    static void button_event_handler(lv_event_t* e) {
        AutomMode mode = *(AutomMode*)lv_event_get_user_data(e);
        selected_autom = mode;


        log.info(std::format("Button {} was selected", automModeToString(mode)));
        lv_screen_load_anim(ui::op_control::driver_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);
    }

    void register_button(std::string text, int col, int row, lv_color_t bg_color, AutomMode mode, callback_method callback) {
        callback_map[mode] = callback; 

        lv_obj_t* btn = lv_button_create(autom_screen);
        lv_obj_set_grid_cell(
            btn,
            LV_GRID_ALIGN_STRETCH, col, 1,
            LV_GRID_ALIGN_STRETCH, row, 1
        );

        // Style Button
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
        AutomMode* stored_mode = (AutomMode*)lv_malloc(sizeof(AutomMode));
        *stored_mode = mode;

        lv_obj_add_event_cb(btn, button_event_handler, LV_EVENT_PRESSED, stored_mode);

        lv_obj_center(label);
        log.debug("Registered " + automModeToString(mode));
    }

    void run_automous() {
        log.info(std::format("Running {}", automModeToString(selected_autom)));
        selected_callback();
    }
}
