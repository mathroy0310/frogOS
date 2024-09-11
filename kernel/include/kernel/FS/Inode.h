/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inode.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:25:42 by maroy             #+#    #+#             */
/*   Updated: 2024/09/03 16:56:42 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/ForwardList.h>
#include <FROG/Memory.h>

#include <FROG/String.h>
#include <FROG/Vector.h>

namespace Kernel {

class Inode : public FROG::RefCounted<Inode> {
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

	enum class Type {
		General,
		Ext2,
	};

  public:
	virtual ~Inode() {}

	bool ifdir() const { return mode().IFDIR; }
	bool ifreg() const { return mode().IFREG; }

	virtual uint16_t uid() const = 0;
	virtual uint16_t gid() const = 0;
	virtual uint32_t size() const = 0;

	virtual Mode mode() const = 0;

	virtual FROG::StringView name() const = 0;

	FROG::ErrorOr<FROG::Vector<FROG::RefPtr<Inode>>> directory_inodes();
	FROG::ErrorOr<FROG::RefPtr<Inode>>               directory_find(FROG::StringView);

	virtual FROG::ErrorOr<size_t> read(size_t, void *, size_t) = 0;

	virtual Type type() const = 0;
	virtual bool operator==(const Inode &) const = 0;

  protected:
	virtual FROG::ErrorOr<FROG::Vector<FROG::RefPtr<Inode>>> directory_inodes_impl() = 0;
	virtual FROG::ErrorOr<FROG::RefPtr<Inode>>              directory_find_impl(FROG::StringView) = 0;
};

} // namespace Kernel