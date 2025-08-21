#pragma once

// #include "liblvgl/draw/lv_image_dsc.h"
// #include "liblvgl/misc/lv_types.h"
// #include <optional>
// #include <vector>


// namespace images {
//     struct ImageEntry {
//         lv_image_dsc_t image;
//         std::optional<int> delay;
//     };

//     class ImageHandler {
//         private:
//             lv_obj_t *img;
//             std::vector<ImageEntry> images;
//             bool activated = false;
//             int index = 0;
//             int delay;
//         public:
//             ImageHandler(int default_delay = 5000);
//             void register_image(ImageEntry image);
//             void register_image(lv_image_dsc_t image);
//             void register_image(lv_image_dsc_t image, int delay);
//             void register_images(std::vector<ImageEntry> images);
//             void set_default_delay(int delay);
//             int get_default_delay();
//             void remove();
//             void start();
//             void stop();
//     };
// }