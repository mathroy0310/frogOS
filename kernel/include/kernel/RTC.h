/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RTC.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 20:04:22 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/21 00:36:29 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Time.h>

namespace RTC {

FROG::Time get_current_time();

} // namespace RTC