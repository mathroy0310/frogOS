/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inode.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:25:42 by maroy             #+#    #+#             */
/*   Updated: 2024/08/26 14:26:09 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/ForwardList.h>
#include <FROG/Memory.h>

#include <stdint.h>

namespace Kernel {

class Inode {
  public:
	virtual bool is_directory() const = 0;
	virtual bool is_regular_file() const = 0;

	virtual uint16_t uid() const = 0;
	virtual uint16_t gid() const = 0;
	virtual uint32_t size() const = 0;

	virtual FROG::StringView name() const = 0;

	virtual FROG::ErrorOr<FROG::Vector<uint8_t>>                 read_all() const = 0;
	virtual FROG::ErrorOr<FROG::Vector<FROG::RefCounted<Inode>>> directory_inodes() const = 0;
	virtual FROG::ErrorOr<FROG::RefCounted<Inode>> directory_find(FROG::StringView) const = 0;
};

} // namespace Kernel