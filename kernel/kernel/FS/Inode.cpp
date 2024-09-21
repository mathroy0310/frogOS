/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inode.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:58:44 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 00:25:03 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/StringView.h>
#include <kernel/FS/Inode.h>
#include <kernel/FS/VirtualFileSystem.h>

namespace Kernel {

FROG::ErrorOr<FROG::RefPtr<Inode>> Inode::read_directory_inode(FROG::StringView name) {
	if (name == ".."sv) return read_directory_inode_impl(name);
	for (const auto &mount : VirtualFileSystem::get().mount_points())
		if (*mount.inode == *this)
			return mount.target->root_inode()->read_directory_inode_impl(name);
	return read_directory_inode_impl(name);
}

FROG::ErrorOr<FROG::Vector<FROG::String>> Inode::read_directory_entries(size_t index) {
	for (const auto &mount : VirtualFileSystem::get().mount_points())
		if (*mount.inode == *this)
			return mount.target->root_inode()->read_directory_entries_impl(index);
	return read_directory_entries_impl(index);
}

} // namespace Kernel