/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdlib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 01:33:22 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 16:02:16 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

__attribute__((__noreturn__)) void abort(void);

void *malloc(size_t);
void  free(void *);

__END_DECLS