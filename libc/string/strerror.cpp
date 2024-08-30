/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strerror.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:32:07 by maroy             #+#    #+#             */
/*   Updated: 2024/08/30 15:32:08 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>

int errno = 0;

char *strerror(int error) {
	switch (error) {
	case ENOMEM:
		return "Cannot allocate memory";
	case EINVAL:
		return "Invalid argument";
	case EISDIR:
		return "Is a directory";
	case ENOTDIR:
		return "Not a directory";
	case ENOENT:
		return "No such file or directory";
	case EIO:
		return "Input/output error";
	default:
		break;
	}

	// FIXME: sprintf
	// static char buffer[26];
	// sprintf(buffer, "Unknown error %d", error);
	// return buffer;
	errno = EINVAL;
	return "Unknown error";
}