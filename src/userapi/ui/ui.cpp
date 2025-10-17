#include "userapi/ui/ui.hpp"

#include "autom.hpp"

#include "liblvgl/core/lv_obj_scroll.h"
#include "liblvgl/misc/lv_async.h"
#include "pros/misc.h"
#include "pros/optical.h"
#include "userapi/configuration.hpp"

#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/core/lv_obj_style_gen.h"
#include "liblvgl/core/lv_obj_tree.h"
#include "liblvgl/layouts/flex/lv_flex.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/misc/lv_color.h"
#include "liblvgl/misc/lv_style.h"
#include "liblvgl/widgets/image/lv_image.h"
#include "liblvgl/widgets/label/lv_label.h"
#include "liblvgl/widgets/tabview/lv_tabview.h"
#include "liblvgl/misc/lv_types.h"
#include "userapi/handler/optical_normalize.hpp"

#include <cstddef>
#include <deque>
#include <format>
#include <vector>

LV_IMAGE_DECLARE(EBA_Automotive_Auto_Repair);
LV_IMAGE_DECLARE(PioneerContainerService);
std::vector<lv_image_dsc_t> sponsors = {EBA_Automotive_Auto_Repair, PioneerContainerService};
int currentSponsor = 0;
lv_obj_t* sponsorImage;

lv_obj_t* tabSponsors;
lv_obj_t* tabLogs;
lv_obj_t* tabDebug;
lv_obj_t* tabKeybinds;

lv_obj_t* labelDebug;

lv_obj_t* logsContainer;
int maxLogs = 11;
std::deque<lv_obj_t*> logsListItems;

lv_obj_t* keybindsList;

namespace ui::op_control {
    lv_obj_t* driver_screen = lv_obj_create(NULL);
    // lv_obj_t* tabview = nullptr;

    void initialize() {
        // Driver Screen container
        // driver_screen = lv_obj_create(NULL);

        lv_obj_t* tabview = lv_tabview_create(driver_screen);
        // driver_screen = tabview;

        // Tabview style
        lv_obj_set_style_bg_color(tabview, lv_color_hex(0x191919), 0);
        lv_obj_set_style_text_color(tabview, lv_color_hex(0xffffff), 0);
        lv_tabview_set_tab_bar_size(tabview, 30);

        lv_obj_t* tab_btns = lv_tabview_get_tab_bar(tabview);
        lv_obj_set_style_bg_color(tab_btns, lv_color_hex(0x505050), 0);
        lv_obj_set_style_text_color(tab_btns, lv_color_hex(0xffffff), 0);

        // Create Tabs
        tabSponsors = lv_tabview_add_tab(tabview, "Sponsors");
        tabLogs = lv_tabview_add_tab(tabview, "Logs");
        tabDebug = lv_tabview_add_tab(tabview, "Debug");
        tabKeybinds = lv_tabview_add_tab(tabview, "Keybinds");

        // Create Labels
        labelDebug = lv_label_create(tabDebug);

        // Create Timers
        lv_timer_create(debug_timer, 50, nullptr);

        // Create Logs Container
        lv_obj_set_style_pad_all(tabLogs, 0, LV_PART_MAIN);
        logsContainer = lv_obj_create(tabLogs);
        lv_obj_set_size(logsContainer, lv_pct(100), lv_pct(100));
        lv_obj_set_flex_flow(logsContainer, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_scrollbar_mode(logsContainer, LV_SCROLLBAR_MODE_OFF);

        lv_obj_set_style_radius(logsContainer, 0, 0);
        lv_obj_set_style_pad_all(logsContainer, 2, 0);
        lv_obj_set_style_pad_row(logsContainer, 2, 0);
        lv_obj_set_style_border_width(logsContainer, 0, 0);
        lv_obj_set_style_bg_color(logsContainer, lv_color_hex(0x000000), 0);
        
        PROSLogger::Manager::subscribe(ui::op_control::logs);

        // Create Sponsors
        // Images must bee 480x210
        lv_obj_set_style_pad_all(tabSponsors, 0, LV_PART_MAIN);
        sponsorImage = lv_image_create(tabSponsors);
        lv_obj_align(sponsorImage, LV_ALIGN_TOP_LEFT, 0, 0);
        lv_image_set_src(sponsorImage, &EBA_Automotive_Auto_Repair);

        lv_timer_create(image_render, 5000, nullptr);

        init_keybinds();
    }

    void image_render(lv_timer_t *timer) {
        lv_image_set_src(sponsorImage, &sponsors[currentSponsor]);
        currentSponsor = (currentSponsor + 1) % sponsors.size();
    }

    void debug_timer(lv_timer_t* timer) {
        lemlib::Pose pose = devices::chassis.getPose();
        pros::c::optical_rgb_s_t normal_color = optical_normalize(devices::opticalSensor.get_rgb());

        std::string pos_str = std::format("X: {:.2f} Y: {:.2f} Theta: {:.2f}", pose.x, pose.y, pose.theta);
        std::string autom_mode_str = std::format("Automonous Mode: {}", ui::autom_selector::automModeToString(ui::autom_selector::selected_autom));
        std::string color = std::format("Red: {:.2f}, Blue: {:.2f}, Distance: {}", normal_color.red, normal_color.blue, devices::opticalSensor.get_proximity());
        lv_label_set_text(labelDebug, std::format("{}\n{}\n{}", pos_str, autom_mode_str, color).c_str());
    }


    void logs(PROSLogger::LoggerEvent event) {
        // Capture data by value so it's safe
        std::string formatted = std::format("[{}] [{}] {}", event.time, levelToString(event.level), event.message);

        lv_async_call([](void* msg) {
            std::string* text = static_cast<std::string*>(msg);

            lv_obj_t* label = lv_label_create(logsContainer);
            lv_label_set_text(label, text->c_str());

            logsListItems.push_back(label);

            if (logsListItems.size() > maxLogs) {
                lv_obj_delete(logsListItems.front());
                logsListItems.pop_front();
            }

            lv_obj_scroll_to_y(logsContainer, lv_obj_get_scroll_bottom(logsContainer), LV_ANIM_OFF);

            delete text; // clean up
        }, new std::string(formatted)); // heap alloc because LVGL calls async later
    }


    void init_keybinds() {
        // Keybinds
        lv_obj_set_style_pad_all(tabKeybinds, 0, LV_PART_MAIN);
        keybindsList = lv_obj_create(tabKeybinds);
        lv_obj_set_size(keybindsList, lv_pct(100), lv_pct(100));
        lv_obj_set_flex_flow(keybindsList, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_scroll_dir(keybindsList, LV_DIR_VER);

        lv_obj_set_style_radius(keybindsList, 0, 0);
        lv_obj_set_style_pad_all(keybindsList, 0, 0);
        lv_obj_set_style_pad_row(keybindsList, 0, 0);
        lv_obj_set_style_border_width(keybindsList, 0, 0);
        lv_obj_set_style_bg_color(keybindsList, lv_color_hex(0x000000), 0);

        create_category("Drive");
        create_row("X", "Changes the Driving Modes.");

        // create_category("Scrapper");
        // create_row("U", "Toggle the scrapper.");

        create_category("Load");
        create_row("L1", "Loads balls without scoring.");
        create_row("B-L1", "Toggle loads balls without scoring.");

        create_category("Score");
        create_row("L2", "Moves balls into high score.");
        create_row("B-L2", "Toggle moves balls into high score.");
        create_row("R1", "Move balls into middle score.");
        create_row("B-R1", "Toggle moves balls into middle score.");
        create_row("R2", "Moves balls into low score.");
        create_row("B-R2", "Toggle move balls into low score.");

        if (!pros::c::competition_is_connected() && false) {
            create_category("Testing");
            create_row("R1", "t1 & t2 = 127, t3 & t4 = -127");
            create_row("R2", "t1 & t2 = -127, t3 & t4 = 127");
        }
    }

    void create_category(std::string category) {
        lv_obj_t* category_container = lv_obj_create(keybindsList);
        lv_obj_set_size(category_container, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_style_radius(category_container, 0, 0);
        lv_obj_set_style_pad_all(category_container, 4, 0);
        lv_obj_set_style_border_width(category_container, 1, 0);
        lv_obj_set_style_border_side(category_container, LV_BORDER_SIDE_BOTTOM, 0);
        lv_obj_set_style_border_color(category_container, lv_color_hex(0x000000), 0);
        lv_obj_set_style_border_post(category_container, true, 0);
        lv_obj_set_style_bg_color(category_container, lv_color_hex(0x3f444c), 0);

        lv_obj_t* categoryLabel = lv_label_create(category_container);
        lv_label_set_text(categoryLabel, category.c_str());
    }

    void create_row(std::string key, std::string description) {
        lv_obj_t* row = lv_obj_create(keybindsList);
        lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_style_radius(row, 0, 0);
        lv_obj_set_style_pad_all(row, 4, 0);
        lv_obj_set_style_border_width(row, 1, 0);
        lv_obj_set_style_border_side(row, LV_BORDER_SIDE_BOTTOM, 0);
        lv_obj_set_style_border_color(row, lv_color_hex(0x000000), 0);
        lv_obj_set_style_border_post(row, true, 0);
        lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW); 

        lv_obj_t *id = lv_label_create(row);
        lv_label_set_text(id, std::format("Id: {}", key).c_str());

        lv_obj_t *desc = lv_label_create(row);
        lv_label_set_text(desc, std::format("\"{}\"", description).c_str());
    }
}