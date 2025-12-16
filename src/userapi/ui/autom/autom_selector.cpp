#include "userapi/ui/autom/autom_selector.hpp"

#include "gamers-forge/proslogger.hpp"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/misc/lv_event.h"
#include "liblvgl/widgets/button/lv_button.h"
#include "liblvgl/widgets/label/lv_label.h"
#include "liblvgl/display/lv_display.h"

#include <cstdint>
#include <format>

#include "userapi/ui/autom/autom_handler.hpp"
#include "userapi/ui/op_control.hpp"


using namespace ui::autom::handler;

namespace ui::autom::location_selector {
    PROSLogger::Logger log{"AutonomousLocationSelector"};

    void initialize() {
        log.info("Initializing Autonomous Location Selector Screen");
        lv_obj_set_size(selector, 480, 240);
        lv_obj_center(selector);

        lv_obj_set_style_pad_all(selector, 0, 0);
        lv_obj_set_style_pad_gap(selector, 2, 0);
        lv_obj_set_style_border_width(selector, 0, 0);

        static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        lv_obj_set_grid_dsc_array(selector, col_dsc, row_dsc);
        lv_obj_set_layout(selector, LV_LAYOUT_GRID);

        register_button(AutomSideColor::RED_LEFT, 0, 0);
        register_button(AutomSideColor::RED_RIGHT, 0, 1);
        register_button(AutomSideColor::BLUE_RIGHT, 1, 0);
        register_button(AutomSideColor::BLUE_LEFT, 1, 1);

        log.info("Finished initialize of Autonomous Location Selector Screen");
    }

    void register_button(AutomSideColor color, int col, int row) {
        AutomColor autom_color = get_color_from_side_color(color);

        lv_obj_t* btn = lv_button_create(selector);
        lv_obj_set_grid_cell(
            btn,
            LV_GRID_ALIGN_STRETCH, col, 1,
            LV_GRID_ALIGN_STRETCH, row, 1
        );
        
        // Style Button
        lv_color_t bg_color;
        if (autom_color == AutomColor::RED) {bg_color = lv_palette_main(LV_PALETTE_RED);}
        else {bg_color = lv_palette_main(LV_PALETTE_BLUE);}

        lv_obj_set_style_bg_color(btn, bg_color, LV_PART_MAIN);
        lv_obj_set_style_radius(btn, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);

        // Buton Label
        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, automPositionToString(get_position_from_side_color(color)).data());
        lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);

        // Set Font Style
        static lv_style_t label_style;
        lv_style_init(&label_style);
        lv_style_set_text_font(&label_style, &lv_font_montserrat_30);
        lv_obj_add_style(label, &label_style, LV_PART_MAIN);

        lv_obj_center(label);

        // Add Callback
        lv_obj_add_event_cb(btn, button_event_handler, LV_EVENT_PRESSED, (void*)color);

        log.debug(std::format("Registered button for color: {}", automColorToString(autom_color)));
    }

    void button_event_handler(lv_event_t* e) {
        AutomSideColor side_color = (AutomSideColor)(intptr_t)lv_event_get_user_data(e);
        select_autom(active_mode, side_color);

        lv_screen_load_anim(ui::driver::driver_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);

        log.info(std::format("{} {} {} button was selected", automModeToString(active_mode), automPositionToString(get_position_from_side_color(side_color)), automColorToString(get_color_from_side_color(side_color))));
    }
}