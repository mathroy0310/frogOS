# **************************************************************************** #
#                                                              _               #
#                                                  __   ___.--'_\`.            #
#    qemu.sh                                      ( _\`.' -   'o\` )           #
#                                                 _\\.'_'      _.-'            #
#    By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`             #
#                                                 \\_'-`---'\\__,              #
#    Created: 2024/08/04 01:33:07 by mathroy0310   \`        `-\\              #
#    Updated: 2024/08/04 15:21:19 by mathroy0310    `                          #
#                                                                              #
# **************************************************************************** #

#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) 		\
	-m 128												\
	-drive format=raw,media=cdrom,file=frog-os.iso		\
	-serial stdio	
