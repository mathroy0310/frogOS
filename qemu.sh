# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    qemu.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/04 01:33:07 by mathroy0310       #+#    #+#              #
#    Updated: 2024/09/03 17:25:56 by maroy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/sh
set -e
. ./disk.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) 			\
    -m 128													\
    -smp 2													\
    -drive format=raw,media=disk,file=frog-os.img			\
    -serial stdio 											\
	-accel kvm												\
