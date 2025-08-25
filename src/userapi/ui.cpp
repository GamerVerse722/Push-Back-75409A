#include "userapi/ui.hpp"

#include "configuration.hpp"

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
#include "liblvgl/display/lv_display.h"
#include "liblvgl/misc/lv_types.h"

#include <deque>
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


void ui::op_control::initialize() {
    // Create Tabview
    lv_obj_t* tabview = lv_tabview_create(lv_screen_active());

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
    lv_timer_create(debug_timer, 20, nullptr);

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

void ui::op_control::image_render(lv_timer_t *timer) {
    lv_image_set_src(sponsorImage, &sponsors[currentSponsor]);
    currentSponsor = (currentSponsor + 1) % sponsors.size();
}

void ui::op_control::debug_timer(lv_timer_t* timer) {
    lemlib::Pose pose = devices::chassis.getPose();

    lv_label_set_text(labelDebug, std::format("X: {:.2f} Y: {:.2f} Theta: {:.2f}", pose.x, pose.y, pose.theta).c_str());
}


void ui::op_control::logs(PROSLogger::LoggerEvent event) {
    lv_obj_t* label = lv_label_create(logsContainer);
    lv_label_set_text(label, std::format("[{}] [{}] {}", event.time, levelToString(event.level), event.message).c_str());

    logsListItems.push_back(label);

    if (logsListItems.size() > maxLogs) {
        lv_obj_delete(logsListItems.front());
        logsListItems.pop_front();
    }

    lv_obj_scroll_to_y(logsContainer, lv_obj_get_scroll_bottom(logsContainer), LV_ANIM_OFF);
}

void ui::op_control::init_keybinds() {
    // Keybinds
    lv_obj_set_style_pad_all(tabKeybinds, 0, LV_PART_MAIN);
    keybindsList = lv_obj_create(tabKeybinds);
    lv_obj_set_size(keybindsList, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(keybindsList, LV_FLEX_FLOW_COLUMN);

    lv_obj_set_style_radius(keybindsList, 0, 0);
    lv_obj_set_style_pad_all(keybindsList, 0, 0);
    lv_obj_set_style_pad_row(keybindsList, 0, 0);
    lv_obj_set_style_border_width(keybindsList, 0, 0);
    lv_obj_set_style_bg_color(keybindsList, lv_color_hex(0x000000), 0);

    create_category("Drive");
    create_row("X", "Changes the Driving Modes.");

    create_category("Intake");
    create_row("L1", "Moves balls into bucket.");
    create_row("B-L1", "Toggle move balls into bucket.");

    create_category("Bucket");
    create_row("L2", "Moves balls out of bucket into low score.");
    create_row("B-L2", "Toggle move balls out of bucket into low score.");
    create_row("R1", "Moves balls out of bucket into middle score.");
    create_row("B-R1", "Toggle move balls out of bucket into middle score.");
    create_row("R2", "Moves balls out of bucket into high score.");
    create_row("B-R2", "Toggle move balls out of bucket into high score.");
}

void ui::op_control::create_category(std::string category) {
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

void ui::op_control::create_row(std::string key, std::string description) {
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