/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirent.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:57:00 by maroy             #+#    #+#             */
/*   Updated: 2024/09/20 14:57:33 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/types.h>

__BEGIN_DECLS

struct DIR;

struct dirent {
	ino_t d_ino;
	char  d_name[];
};

__END_DECLS