/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualFileSystem.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:33 by maroy             #+#    #+#             */
/*   Updated: 2024/08/26 16:06:06 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/StringView.h>
#include <FROG/Vector.h>
#include <kernel/FS/VirtualFileSystem.h>

namespace Kernel {

static VirtualFileSystem *s_instance = nullptr;

void VirtualFileSystem::initialize(FROG::RefCounted<Inode> root_inode) {
	ASSERT(s_instance == nullptr);
	s_instance = new VirtualFileSystem(root_inode);
	ASSERT(s_instance);
}

VirtualFileSystem &VirtualFileSystem::get() {
	ASSERT(s_instance);
	return *s_instance;
}

bool VirtualFileSystem::is_initialized() { return s_instance != nullptr; }

FROG::ErrorOr<FROG::RefCounted<Inode>> VirtualFileSystem::from_absolute_path(FROG::StringView path) {
	if (path.front() != '/') return FROG::Error::from_string("Path must be an absolute path");

	auto inode = root_inode();
	auto path_parts = TRY(path.split('/'));

	for (FROG::StringView part : path_parts)
		inode = TRY(inode->directory_find(part));

	return inode;
}

} // namespace Kernel