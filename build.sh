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

echo "===== Starting OS Build Process ====="

set -e

echo "Installing system headers first..."
. ./headers.sh
echo "  ✓ Headers installation complete"

echo "Building and installing all projects: $PROJECTS"
for PROJECT in $PROJECTS; do
  echo "========================================"
  echo "  → Building project: $PROJECT"
  echo "    • Changing to directory: $PROJECT"
  echo "    • Running make install with DESTDIR=$SYSROOT"
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
  echo "    ✓ Project $PROJECT built and installed successfully"
  echo "========================================"
done

echo "All components have been built and installed to $SYSROOT"
