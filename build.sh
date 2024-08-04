# **************************************************************************** #
#                                                              _               #
#                                                  __   ___.--'_\`.            #
#    build.sh                                     ( _\`.' -   'o\` )           #
#                                                 _\\.'_'      _.-'            #
#    By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`             #
#                                                 \\_'-`---'\\__,              #
#    Created: 2024/08/04 01:32:54 by mathroy0310   \`        `-\\              #
#    Updated: 2024/08/04 01:32:55 by mathroy0310    `                          #
#                                                                              #
# **************************************************************************** #

#!/bin/sh
set -e
. ./headers.sh
 
for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
