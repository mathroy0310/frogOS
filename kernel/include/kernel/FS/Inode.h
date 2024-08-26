/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inode.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:25:42 by maroy             #+#    #+#             */
/*   Updated: 2024/08/26 15:51:07 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/ForwardList.h>
#include <FROG/Memory.h>

#include <stdint.h>

namespace Kernel {

class Inode {
  public:
	union Mode {
		struct {
			uint16_t IXOTH : 1; // 0x0001
			uint16_t IWOTH : 1; // 0x0002
			uint16_t IROTH : 1; // 0x0004
			uint16_t IXGRP : 1; // 0x0008
			uint16_t IWGRP : 1; // 0x0010
			uint16_t IRGRP : 1; // 0x0020
			uint16_t IXUSR : 1; // 0x0040
			uint16_t IWUSR : 1; // 0x0080
			uint16_t IRUSR : 1; // 0x0100
			uint16_t ISVTX : 1; // 0x0200
			uint16_t ISGID : 1; // 0x0400
			uint16_t ISUID : 1; // 0x0800
			uint16_t IFIFO : 1; // 0x1000
			uint16_t IFCHR : 1; // 0x2000
			uint16_t IFDIR : 1; // 0x4000
			uint16_t IFREG : 1; // 0x8000
		};
		uint16_t mode;
	};

  public:
	bool ifdir() const { return mode().IFDIR; }
	bool ifreg() const { return mode().IFREG; }

	virtual uint16_t uid() const = 0;
	virtual uint16_t gid() const = 0;
	virtual uint32_t size() const = 0;

	virtual Mode mode() const = 0;

	virtual FROG::StringView name() const = 0;

	virtual FROG::ErrorOr<FROG::Vector<uint8_t>>                 read_all() = 0;
	virtual FROG::ErrorOr<FROG::Vector<FROG::RefCounted<Inode>>> directory_inodes() = 0;
	virtual FROG::ErrorOr<FROG::RefCounted<Inode>> directory_find(FROG::StringView) = 0;
};

} // namespace Kernel