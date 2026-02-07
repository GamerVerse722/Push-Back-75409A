#pragma once

#include "gamers-forge/proslogger.hpp"

namespace autom::Qualifications {
    void left();
    void right();
}

namespace autom::Eliminations {
    void unified();
    void unified_descore();
    void left();
    void right();
}

namespace autom::Skills {
    void skills();
    void skills2();
}

namespace autom::None {
    extern PROSLogger::Logger log;

    void print_debug();
    void none();
}