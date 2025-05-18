# shell.nix for frogOS development environment
{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  name = "frogOS-dev-environment";

  buildInputs = with pkgs; [
    # Basic development tools
    gnumake
    gdb
    qemu
    parted

    # Basic utilities
    coreutils
    util-linux
    clang-tools # clang-format

    # x86_64-elf cross-compiler and tools
    pkgsCross.x86_64-embedded.buildPackages.gcc
    pkgsCross.x86_64-embedded.buildPackages.binutils
  ];

  # Environment variables for the shell
  shellHook = ''
    export PROJECT_ROOT=$(pwd)
    export SYSROOT=$PROJECT_ROOT/sysroot

    # Set up cross compiler executables
    export CC="x86_64-elf-gcc"
    export CXX="x86_64-elf-g++"
    export LD="x86_64-elf-ld"
    export AR="x86_64-elf-ar"
    export AS="x86_64-elf-as"

    # Make build scripts executable automatically
    chmod +x $PROJECT_ROOT/default-host.sh
    chmod +x $PROJECT_ROOT/target-triplet-to-arch.sh
    chmod +x $PROJECT_ROOT/headers.sh
    chmod +x $PROJECT_ROOT/config.sh

    echo "==============================================="
    echo "frogOS Development Environment"
    echo "==============================================="
    echo "Available cross-compiler tools:"
    echo "  CC       : $CC"
    echo "  CXX      : $CXX"
    echo "  LD       : $LD"
    echo "  AR       : $AR"
    echo "  AS       : $AS"
    echo ""
    echo "Project configuration:"
    echo "  PROJECT_ROOT : $PROJECT_ROOT"
    echo "  SYSROOT      : $SYSROOT"
    echo "==============================================="
    echo "Use 'nix-shell' to enter this environment"
    echo "Run 'make' to build the entire system"
    echo "==============================================="
  '';
}
