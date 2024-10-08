/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:00:27 by maroy             #+#    #+#             */
/*   Updated: 2024/09/20 14:43:36 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

using blkcnt_t = int32_t;
using blksize_t = int32_t;
using dev_t = uint64_t;
using ino_t = uint32_t;
using mode_t = uint16_t;
using nlink_t = uint32_t;
using off_t = int64_t;
using time_t = uint64_t;
using id_t = int32_t;
using pid_t = id_t;
using uid_t = id_t;
using gid_t = id_t;

__END_DECLS