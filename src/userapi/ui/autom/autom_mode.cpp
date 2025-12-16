#include "userapi/ui/autom/autom_mode.hpp"

#include "liblvgl/misc/lv_event.h"
#include "liblvgl/misc/lv_palette.h"
#include "liblvgl/widgets/button/lv_button.h"
#include "liblvgl/widgets/label/lv_label.h"
#include "liblvgl/display/lv_display.h"

#include "userapi/ui/autom/autom_handler.hpp"
#include "userapi/ui/autom/autom_selector.hpp"
#include "userapi/ui/op_control.hpp"


namespace ui::autom::mode_selector {
    void initialize() {
        lv_obj_set_size(selector, 480, 240);
        lv_obj_center(selector);

        lv_obj_set_style_pad_all(selector, 0, 0);
        lv_obj_set_style_pad_gap(selector, 2, 0);
        lv_obj_set_style_border_width(selector, 0, 0);

        static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        lv_obj_set_grid_dsc_array(selector, col_dsc, row_dsc);
        lv_obj_set_layout(selector, LV_LAYOUT_GRID);

        register_button(AutomMode::QUALIFICATIONS, 0);
        register_button(AutomMode::ELIMINATIONS, 1);
        register_button(AutomMode::SKILLS, 2);
        register_button(AutomMode::NONE, 3);
    }
    
    void register_button(AutomMode mode, int row) {
        lv_obj_t* btn = lv_button_create(selector);
        lv_obj_set_grid_cell(
            btn,
            LV_GRID_ALIGN_STRETCH, 0, 1,
            LV_GRID_ALIGN_STRETCH, row, 1
        );
        
        // Style Button
        lv_color_t bg_color;
        switch (((int) mode) % 3) {
            case 0: bg_color = lv_color_white();
            case 1: bg_color = lv_palette_main(LV_PALETTE_RED);
            case 2: bg_color = lv_palette_main(LV_PALETTE_BLUE);
        }

        lv_obj_set_style_bg_color(btn, bg_color, LV_PART_MAIN);
        lv_obj_set_style_radius(btn, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);

        // Buton Label
        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, "Button");
        lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);

        // Set Font Style
        static lv_style_t label_style;
        lv_style_init(&label_style);
        lv_style_set_text_font(&label_style, &lv_font_montserrat_30);
        lv_obj_add_style(label, &label_style, LV_PART_MAIN);

        // Add Callback
        lv_obj_add_event_cb(btn, button_event_handler, LV_EVENT_PRESSED, (void*)mode);
    }

    void button_event_handler(lv_event_t* e) {
        AutomMode mode = (AutomMode)(intptr_t)lv_event_get_user_data(e);
        switch (mode) {
            case AutomMode::QUALIFICATIONS: {
                location_selector::active_mode = AutomMode::QUALIFICATIONS;
                lv_screen_load(location_selector::selector);
                break;
            }
            case AutomMode::ELIMINATIONS: {
                location_selector::active_mode = AutomMode::ELIMINATIONS;
                lv_screen_load(location_selector::selector);
                break;
            }
            case AutomMode::SKILLS: {
                handler::select_autom(AutomMode::SKILLS, AutomSideColor::NO_COLOR_AND_POSITION);
                lv_screen_load(ui::driver::driver_screen);
                break;
            }
            case AutomMode::NONE: {
                handler::select_autom(AutomMode::NONE, AutomSideColor::NO_COLOR_AND_POSITION);
                lv_screen_load(ui::driver::driver_screen);
                break;
            }
            break;
        }
    }
}