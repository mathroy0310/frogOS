/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inode.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:25:42 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 00:11:58 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/ForwardList.h>
#include <FROG/Memory.h>

#include <FROG/String.h>
#include <FROG/Vector.h>
#include <sys/types.h>
#include <time.h>

namespace Kernel {

class Inode : public FROG::RefCounted<Inode> {
  public:
	enum Mode : mode_t {
		IXOTH = 0x0001,
		IWOTH = 0x0002,
		IROTH = 0x0004,
		IXGRP = 0x0008,
		IWGRP = 0x0010,
		IRGRP = 0x0020,
		IXUSR = 0x0040,
		IWUSR = 0x0080,
		IRUSR = 0x0100,
		ISVTX = 0x0200,
		ISGID = 0x0400,
		ISUID = 0x0800,
		IFIFO = 0x1000,
		IFCHR = 0x2000,
		IFDIR = 0x4000,
		IFREG = 0x8000,
	};

	enum class Type {
		General,
		Ext2,
	};

  public:
	virtual ~Inode() {}

	bool ifdir() const { return mode() & Mode::IFDIR; }
	bool ifreg() const { return mode() & Mode::IFREG; }

	virtual ino_t     ino() const = 0;
	virtual mode_t    mode() const = 0;
	virtual nlink_t   nlink() const = 0;
	virtual uid_t     uid() const = 0;
	virtual gid_t     gid() const = 0;
	virtual off_t     size() const = 0;
	virtual timespec  atime() const = 0;
	virtual timespec  mtime() const = 0;
	virtual timespec  ctime() const = 0;
	virtual blksize_t blksize() const = 0;
	virtual blkcnt_t  blocks() const = 0;

	virtual FROG::StringView name() const = 0;

	FROG::ErrorOr<FROG::RefPtr<Inode>>       read_directory_inode(FROG::StringView);
	FROG::ErrorOr<FROG::Vector<FROG::String>> read_directory_entries(size_t);

	virtual FROG::ErrorOr<size_t> read(size_t, void *, size_t) = 0;
	virtual FROG::ErrorOr<void>   create_file(FROG::StringView, mode_t) = 0;

	virtual Type type() const = 0;
	virtual bool operator==(const Inode &) const = 0;

  protected:
	virtual FROG::ErrorOr<FROG::RefPtr<Inode>>       read_directory_inode_impl(FROG::StringView) = 0;
	virtual FROG::ErrorOr<FROG::Vector<FROG::String>> read_directory_entries_impl(size_t) = 0;
};

} // namespace Kernel