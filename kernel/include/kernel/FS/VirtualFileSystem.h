/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualFileSystem.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:26:00 by maroy             #+#    #+#             */
/*   Updated: 2024/08/30 17:56:17 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/FS/FileSystem.h>
#include <kernel/Storage/StorageController.h>

namespace Kernel {

class VirtualFileSystem : public FileSystem {
  public:
	static FROG::ErrorOr<void> initialize();
	static VirtualFileSystem  &get();
	virtual ~VirtualFileSystem() {};

	virtual const FROG::RefPtr<Inode> root_inode() const override { return m_root_inode; }

	FROG::ErrorOr<FROG::RefPtr<Inode>> from_absolute_path(FROG::StringView);

  private:
	VirtualFileSystem() = default;
	FROG::ErrorOr<void> initialize_impl();

  private:
	FROG::RefPtr<Inode> m_root_inode;

	FROG::Vector<StorageController *> m_storage_controllers;
};

} // namespace Kernel