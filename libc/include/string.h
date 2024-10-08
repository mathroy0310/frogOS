/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 01:33:24 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 15:31:52 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

int    memcmp(const void *, const void *, size_t);
void  *memcpy(void *__restrict, const void *__restrict, size_t);
void  *memmove(void *, const void *, size_t);
void  *memset(void *, int, size_t);
size_t strlen(const char *);

int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);

char *strcpy(char *__restrict, const char *__restrict);
char *strncpy(char *__restrict, const char *__restrict, size_t);
char *strstr(const char *, const char *);

char *strerror(int);

__END_DECLS
