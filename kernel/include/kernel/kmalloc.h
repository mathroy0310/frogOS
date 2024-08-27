/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kmalloc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 01:34:12 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/27 01:41:34 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

void kmalloc_initialize();
void kmalloc_dump_info();

void *kmalloc(size_t size);
void *kmalloc(size_t size, size_t align);
void  kfree(void *);