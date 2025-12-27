#!/bin/bash
# Setup script to create compile_commands.json symlink for clangd
# This script automatically finds the compile_commands.json in the build directory
# and creates a symlink in the project root, making it work across different hosts.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"

# Find compile_commands.json in build directories
# Prefer the main one in build/*/compile_commands.json over subproject ones
COMPILE_COMMANDS=$(find "$PROJECT_ROOT/build" -maxdepth 2 -name "compile_commands.json" -type f 2>/dev/null | head -1)
if [ -z "$COMPILE_COMMANDS" ]; then
    # Fallback to any compile_commands.json if main one not found
    COMPILE_COMMANDS=$(find "$PROJECT_ROOT/build" -name "compile_commands.json" -type f 2>/dev/null | head -1)
fi

if [ -z "$COMPILE_COMMANDS" ]; then
    echo "Warning: No compile_commands.json found in build directory."
    echo "Please run CMake to generate it first."
    echo ""
    echo "To generate compile_commands.json:"
    echo "  mkdir -p build && cd build"
    echo "  cmake .."
    exit 1
fi

# Create symlink in project root
SYMLINK="$PROJECT_ROOT/compile_commands.json"

# Remove existing symlink or file if it exists
if [ -L "$SYMLINK" ] || [ -f "$SYMLINK" ]; then
    rm "$SYMLINK"
fi

# Create symlink using relative path (works across different hosts)
RELATIVE_PATH=$(realpath --relative-to="$PROJECT_ROOT" "$COMPILE_COMMANDS" 2>/dev/null || \
               python3 -c "import os; print(os.path.relpath('$COMPILE_COMMANDS', '$PROJECT_ROOT'))")

ln -s "$RELATIVE_PATH" "$SYMLINK"

echo "✓ Created symlink: $SYMLINK -> $RELATIVE_PATH"
echo "✓ clangd should now work correctly. Restart your IDE's language server if needed."
