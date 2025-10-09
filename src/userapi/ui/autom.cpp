#include "userapi/ui/autom.hpp"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/display/lv_display.h"
#include "liblvgl/layouts/grid/lv_grid.h"
#include "liblvgl/misc/lv_types.h"
#include "liblvgl/themes/lv_theme.h"
#include "liblvgl/widgets/button/lv_button.h"
#include "liblvgl/widgets/label/lv_label.h"

namespace ui::autom_selector {

    /* Event handler function */
    static void button_event_handler(lv_event_t *e) {
        
    }

    void initialize() {
        /* Container for the grid */
        lv_obj_t *cont = lv_obj_create(lv_screen_active());
        lv_obj_set_size(cont, 480, 240);
        lv_obj_center(cont);

        lv_obj_set_style_pad_all(cont, 0, 0);
        lv_obj_set_style_pad_gap(cont, 2, 0);
        lv_obj_set_style_border_width(cont, 0, 0);

        /* Grid layout: 2 columns × 3 rows */
        static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
        lv_obj_set_layout(cont, LV_LAYOUT_GRID);

        /* Colors */
        lv_color_t red   = lv_palette_main(LV_PALETTE_RED);
        lv_color_t blue  = lv_palette_main(LV_PALETTE_BLUE);
        lv_color_t white = lv_color_white();

        /* Labels for each cell */
        const char *labels[6] = {
            "Left",  "Right",   /* top row */
            "None",  "None",    /* middle row */
            "Right", "Left"     /* bottom row */
        };

        /* Colors for each cell */
        lv_color_t bg_colors[6] = {
            red,   blue,
            white, white,
            red,   blue
        };

        for (int i = 0; i < 6; i++) {
            lv_obj_t *btn = lv_button_create(cont);

            /* Position button in the grid */
            int col = i % 2;
            int row = i / 2;
            lv_obj_set_grid_cell(btn,
                                 LV_GRID_ALIGN_STRETCH, col, 1,
                                 LV_GRID_ALIGN_STRETCH, row, 1);

            /* Style each button */
            lv_obj_set_style_bg_color(btn, bg_colors[i], LV_PART_MAIN);
            lv_obj_set_style_radius(btn, 0, 0);
            lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);

            /* Label text */
            lv_obj_t *label = lv_label_create(btn);
            lv_label_set_text(label, labels[i]);
            lv_obj_set_style_text_color(label,
                (i == 2 || i == 3) ? lv_color_black() : lv_color_white(),
                LV_PART_MAIN);
            lv_obj_set_style_text_font(label, lv_theme_get_font_large(btn), 0);
            lv_obj_center(label);

            /* Attach the event handler */
            lv_obj_add_event_cb(btn, button_event_handler, LV_EVENT_ALL, NULL);
        }
    }

    void register_autom(AutomMode mode, std::function<void()> method) {
        
    }
}
