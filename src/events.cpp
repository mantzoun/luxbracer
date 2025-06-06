/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include "include/events.h"

namespace luxbracer {
Events::Events(Logger * logger) : logger(logger) {
  from_json(load_file("events.json"), *this);
}

void Events::apply_events(Planet * planet) {
}

void Events::process_events(Planet * planet) {
}

void Events::apply_events(System * system) {
}

void Events::process_events(System * system) {
}
}  // namespace luxbracer
