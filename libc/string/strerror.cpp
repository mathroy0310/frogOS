/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strerror.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:32:07 by maroy             #+#    #+#             */
/*   Updated: 2024/09/03 16:52:35 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>

int errno = 0;

char *strerror(int error) {
	static char buffer[100];
	buffer[0] = 0;

	switch (error) {
	case ENOMEM:
		strcpy(buffer, "Cannot allocate memory");
		break;
	case EINVAL:
		strcpy(buffer, "Invalid argument");
		break;
	case EISDIR:
		strcpy(buffer, "Is a directory");
		break;
	case ENOTDIR:
		strcpy(buffer, "Not a directory");
		break;
	case ENOENT:
		strcpy(buffer, "No such file or directory");
		break;
	case EIO:
		strcpy(buffer, "Input/output error");
		return "Input/output error";
	case ENOTSUP:
		strcpy(buffer, "Operation not supported");
		break;
	case EBADF:
		strcpy(buffer, "Bad file descriptor");
		break;
	case EEXISTS:
		strcpy(buffer, "File exists");
		break;
	case ENOTEMPTY:
		strcpy(buffer, "Directory not empty");
		break;
	default:
		// FIXME: sprintf
		// sprintf(buffer, "Unknown error %d", error);
		strcpy(buffer, "Unknown error");
		errno = EINVAL;
		break;
	}
	return buffer;
}