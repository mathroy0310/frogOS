/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strncmp.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 02:25:12 by maroy             #+#    #+#             */
/*   Updated: 2024/08/27 02:25:13 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int strncmp(const char *s1, const char *s2, size_t n) {
	const unsigned char *u1 = (unsigned char *) s1;
	const unsigned char *u2 = (unsigned char *) s2;
	for (; --n && *u1 && *u2; u1++, u2++)
		if (*u1 != *u2) break;
	return *u1 - *u2;
}