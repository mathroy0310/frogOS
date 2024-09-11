/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inode.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:58:44 by maroy             #+#    #+#             */
/*   Updated: 2024/09/03 16:58:47 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/StringView.h>
#include <kernel/FS/Inode.h>
#include <kernel/FS/VirtualFileSystem.h>

namespace Kernel {

FROG::ErrorOr<FROG::Vector<FROG::RefPtr<Inode>>> Inode::directory_inodes() {
	for (const auto &mount : VirtualFileSystem::get().mount_points())
		if (*mount.inode == *this) return mount.target->root_inode()->directory_inodes_impl();
	return directory_inodes_impl();
}

FROG::ErrorOr<FROG::RefPtr<Inode>> Inode::directory_find(FROG::StringView name) {
	if (name == ".."sv) return directory_find_impl(name);
	for (const auto &mount : VirtualFileSystem::get().mount_points())
		if (*mount.inode == *this) return mount.target->root_inode()->directory_find_impl(name);
	return directory_find_impl(name);
}

} // namespace Kernel