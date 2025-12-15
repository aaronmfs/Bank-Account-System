# Check if the 'build' directory exists
if (Test-Path -Path ".\build") {
    # Remove the 'build' directory recursively if it exists
    Remove-Item -Path ".\build" -Recurse -Force
} else {
    Write-Host "'build' directory does not exist, skipping removal."
}

# Launch the Visual Studio Developer Shell
& "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\Launch-VsDevShell.ps1"

# Run cmake to configure the build
cmake -B build

cmake --build build --config Release