#include "userapi/ui/driver/logs.hpp"

#include "liblvgl/widgets/label/lv_label.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/misc/lv_timer.h"

#include <deque>
#include <array>

static lv_obj_t* logsContainer = nullptr;

static constexpr int maxVisibleLogs = 11;

/* Pending messages */
static std::deque<std::string> pending_logs;

/* Reusable label pool */
static std::array<lv_obj_t*, maxVisibleLogs> label_pool;

/* Ring position for visible labels */
static int visible_index = 0;

namespace ui::driver::logs {
    void initialize(lv_obj_t* parent) {

        lv_obj_set_style_pad_all(parent, 0, LV_PART_MAIN);

        logsContainer = lv_obj_create(parent);
        lv_obj_set_size(logsContainer, lv_pct(100), lv_pct(100));
        lv_obj_set_flex_flow(logsContainer, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_scrollbar_mode(logsContainer, LV_SCROLLBAR_MODE_OFF);

        lv_obj_set_style_radius(logsContainer, 0, 0);
        lv_obj_set_style_pad_all(logsContainer, 2, 0);
        lv_obj_set_style_pad_row(logsContainer, 2, 0);
        lv_obj_set_style_border_width(logsContainer, 0, 0);
        lv_obj_set_style_bg_color(logsContainer, lv_color_hex(0x000000), 0);

        /* Pre-create labels */
        for (int i = 0; i < maxVisibleLogs; i++) {
            label_pool[i] = lv_label_create(logsContainer);
            lv_label_set_text(label_pool[i], "");
            lv_obj_set_style_text_color(label_pool[i], lv_color_white(), 0);
        }

        PROSLogger::Manager::subscribe(logs);

        /* UI timer processes queue */
        lv_timer_create([](lv_timer_t*) {
            process_log_queue();
        }, 16, nullptr);
    }

    void logs(PROSLogger::LoggerEvent event) {

        /* Use preformatted message directly */
        pending_logs.push_back(event.formattedMessage);

        /* Prevent runaway growth */
        if (pending_logs.size() > 50)
            pending_logs.pop_front();
    }

    void process_log_queue() {

        while (!pending_logs.empty()) {

            const std::string& text = pending_logs.front();

            lv_label_set_text(label_pool[visible_index], text.c_str());

            lv_obj_move_to_index(label_pool[visible_index], -1);

            visible_index++;
            if (visible_index >= maxVisibleLogs)
                visible_index = 0;

            pending_logs.pop_front();
        }

        lv_obj_scroll_to_y(
            logsContainer,
            lv_obj_get_scroll_bottom(logsContainer),
            LV_ANIM_OFF
        );
    }

}