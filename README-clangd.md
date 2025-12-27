# Clangd Configuration

This project is configured to work with clangd (the C++ language server) across different hosts.

## Automatic Setup

The `compile_commands.json` symlink is automatically created when you run CMake. The CMake configuration will:
1. Generate `compile_commands.json` in your build directory
2. Automatically create a symlink in the project root pointing to it

This means clangd will work immediately after running CMake, without any manual setup.

## Manual Setup (if needed)

If the automatic symlink creation doesn't work, you can manually run:

```bash
./setup-clangd.sh
```

This script will:
- Find `compile_commands.json` in your build directory
- Create a symlink in the project root

## How It Works

- `.clangd` - Configuration file checked into git (host-independent)
- `compile_commands.json` - Symlink to build directory (host-specific, gitignored)
- The symlink contains relative paths, so it works across different hosts

## Troubleshooting

If clangd still shows errors:
1. Make sure you've run CMake to generate `compile_commands.json`
2. Verify the symlink exists: `ls -la compile_commands.json`
3. Restart your IDE's clangd language server
4. Run `./setup-clangd.sh` if the symlink is missing
