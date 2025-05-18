# **************************************************************************** #
#                                                              _               #
#                                                  __   ___.--'_\`.            #
#    headers.sh                                   ( _\`.' -   'o\` )           #
#                                                 _\\.'_'      _.-'            #
#    By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`             #
#                                                 \\_'-`---'\\__,              #
#    Created: 2024/08/04 01:33:02 by mathroy0310   \`        `-\\              #
#    Updated: 2024/08/04 01:33:03 by mathroy0310    `                          #
#                                                                              #
# **************************************************************************** #

#!/bin/sh

echo "===== Starting Headers Installation Script ====="
set -e

echo "Loading configuration from config.sh..."
. ./config.sh
 
echo "Creating system root directory if it doesn't exist..."
echo "  SYSROOT: $SYSROOT"
mkdir -p "$SYSROOT"
echo "  ✓ SYSROOT directory created/confirmed at: $SYSROOT"
 
echo "Installing headers for system projects: $SYSTEM_HEADER_PROJECTS"
for PROJECT in $SYSTEM_HEADER_PROJECTS; do
  echo "========================================"
  echo "  → Processing project: $PROJECT"
  echo "    • Changing to directory: $PROJECT"
  echo "    • Installing headers to DESTDIR=$SYSROOT"
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install-headers)
  echo "    ✓ Headers installed for $PROJECT"
  echo "========================================"
done

echo "===== Headers Installation Complete ====="
echo "System header files are now available in $SYSROOT$INCLUDEDIR"