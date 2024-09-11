/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileSystem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:25:26 by maroy             #+#    #+#             */
/*   Updated: 2024/09/03 16:59:41 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Memory.h>
#include <kernel/FS/Inode.h>

namespace Kernel {

class FileSystem {
  public:
	virtual FROG::RefPtr<Inode> root_inode() = 0;
};

} // namespace Kernel