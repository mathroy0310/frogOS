/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualFileSystem.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:33 by maroy             #+#    #+#             */
/*   Updated: 2024/08/26 14:31:58 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

} // namespace Kernel