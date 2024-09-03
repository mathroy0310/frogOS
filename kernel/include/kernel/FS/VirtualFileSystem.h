/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualFileSystem.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:26:00 by maroy             #+#    #+#             */
/*   Updated: 2024/09/03 14:23:26 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/HashMap.h>
#include <FROG/String.h>
#include <kernel/FS/FileSystem.h>
#include <kernel/Storage/StorageController.h>

namespace Kernel {

class VirtualFileSystem : public FileSystem {
  public:
	static FROG::ErrorOr<void> initialize();
	static VirtualFileSystem  &get();
	virtual ~VirtualFileSystem() {};

	virtual const FROG::RefPtr<Inode> root_inode() const override;

	void                               close_inode(FROG::StringView);
	FROG::ErrorOr<FROG::RefPtr<Inode>> from_absolute_path(FROG::StringView);

  private:
	VirtualFileSystem() = default;
	FROG::ErrorOr<void> initialize_impl();

  private:
	FROG::HashMap<FROG::String, FROG::RefPtr<Inode>> m_open_inodes;
	FROG::Vector<StorageController *>              m_storage_controllers;
};

} // namespace Kernel