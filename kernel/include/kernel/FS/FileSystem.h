/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileSystem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:25:26 by maroy             #+#    #+#             */
/*   Updated: 2024/08/26 14:26:08 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Memory.h>
#include <kernel/FS/Inode.h>

namespace Kernel {

class FileSystem {
  public:
	virtual const FROG::RefCounted<Inode> root_inode() const = 0;
};

} // namespace Kernel