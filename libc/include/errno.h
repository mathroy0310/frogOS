/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errno.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:31:39 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 16:13:22 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/cdefs.h>

#define ENOMEM 1
#define EINVAL 2
#define ENOTDIR 3
#define EISDIR 4
#define ENOENT 5
#define EIO 6
#define ENOTSUP 7
#define EBADF 8
#define EEXISTS 9
#define ENOTEMPTY 10
#define ENAMETOOLONG 11
#define ENOBUFS 12

__BEGIN_DECLS

extern int errno;

__END_DECLS