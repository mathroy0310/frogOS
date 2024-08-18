# **************************************************************************** #
#                                                              _               #
#                                                  __   ___.--'_\`.            #
#    clean.sh                                     ( _\`.' -   'o\` )           #
#                                                 _\\.'_'      _.-'            #
#    By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`             #
#                                                 \\_'-`---'\\__,              #
#    Created: 2024/08/04 01:32:56 by mathroy0310   \`        `-\\              #
#    Updated: 2024/08/04 01:32:57 by mathroy0310    `                          #
#                                                                              #
# **************************************************************************** #

#!/bin/sh
set -e
. ./config.sh
 
for PROJECT in $PROJECTS; do
  (cd $PROJECT && $MAKE clean)
done
 
rm -rf sysroot
rm -rf frog-os.img
