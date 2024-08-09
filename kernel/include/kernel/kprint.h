/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kprint.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:25:40 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 02:33:32 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Formatter.h>
#include <kernel/TTY.h>

#define kprint(...) FROG::Formatter::print<TTY::PutCharCurrent>(__VA_ARGS__)
#define kprintln(...) FROG::Formatter::println<TTY::PutCharCurrent>(__VA_ARGS__)
