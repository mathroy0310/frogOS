/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PIT.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:54:45 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/18 00:26:15 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

#define PIT_IRQ 0

namespace PIT {

uint64_t ms_since_boot();

void initialize();
void sleep(uint64_t);

} // namespace PIT