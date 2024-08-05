#pragma once

#include <FROG/Formatter.h>

#define dprint FROG::Formatter::print<Serial::serial_putc>
#define dprintln FROG::Formatter::println<Serial::serial_putc>

namespace Serial {

void initialize();

void serial_putc(char);

} // namespace Serial