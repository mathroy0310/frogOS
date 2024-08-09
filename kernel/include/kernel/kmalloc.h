/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kmalloc.h                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 01:34:12 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 11:30:10 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

void kmalloc_initialize();
void kmalloc_dump_nodes();

void *kmalloc_eternal(size_t);
void *kmalloc(size_t);
void *kmalloc(size_t, size_t);
void  kfree(void *);