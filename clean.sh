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
echo "===== Starting Clean Script ====="

set -e

echo "Loading configuration from config.sh..."
. ./config.sh
 
 echo "Building and installing all projects: $PROJECTS"
for PROJECT in $PROJECTS; do
  echo "========================================"
  echo "  → Building project: $PROJECT"
  echo "    • Changing to directory: $PROJECT"
  (cd $PROJECT && $MAKE clean)
  echo "    ✓ Project $PROJECT built and installed successfully"
  echo "========================================"
done
 
rm -rf sysroot
rm -rf frog-os.img
echo "===== Done Clean Script ====="