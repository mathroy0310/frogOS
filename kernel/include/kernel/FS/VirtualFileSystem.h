/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualFileSystem.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:26:00 by maroy             #+#    #+#             */
/*   Updated: 2024/08/26 16:05:30 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/FS/FileSystem.h>

namespace Kernel {

class VirtualFileSystem : public FileSystem {
  public:
	static void               initialize(FROG::RefCounted<Inode> root_inode);
	static VirtualFileSystem &get();
	static bool               is_initialized();

	virtual const FROG::RefCounted<Inode> root_inode() const override { return m_root_inode; }

	FROG::ErrorOr<FROG::RefCounted<Inode>> from_absolute_path(FROG::StringView);

  private:
	VirtualFileSystem(FROG::RefCounted<Inode> root_inode) : m_root_inode(root_inode) {}

  private:
	FROG::RefCounted<Inode> m_root_inode;
};

} // namespace Kernel