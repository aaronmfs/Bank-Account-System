# Bank Account System - Project Initialization Script
# This script clones the repository, downloads dependencies (WebView2), and sets up the project

param(
    [string]$ProjectPath = "C:\Projects\Bank-Account-System",
    [switch]$SkipClone = $false,
    [switch]$CleanBuild = $false
)

Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host "  Bank Account System - Project Initialization" -ForegroundColor Cyan
Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host ""

# Repository URL
$repoUrl = "https://github.com/aaronmfs/Bank-Account-System.git"

# Function to check if a command exists
function Test-Command {
    param($Command)
    try {
        if (Get-Command $Command -ErrorAction Stop) {
            return $true
        }
    }
    catch {
        return $false
    }
}

# Function to test internet connectivity
function Test-Internet {
    try {
        $null = Test-Connection -ComputerName "github.com" -Count 1 -Quiet
        return $true
    }
    catch {
        return $false
    }
}

# =============================================================================
# Pre-flight Checks
# =============================================================================
Write-Host "[Pre-flight Checks]" -ForegroundColor Cyan
Write-Host "-------------------" -ForegroundColor Cyan

# Check Git
if (-not (Test-Command "git")) {
    Write-Host "✗ Git is not installed or not in PATH" -ForegroundColor Red
    Write-Host "  Please install Git first or run the setup-dev-environment.ps1 script" -ForegroundColor Yellow
    exit 1
}
Write-Host "✓ Git is available" -ForegroundColor Green

# Check CMake
if (-not (Test-Command "cmake")) {
    Write-Host "✗ CMake is not installed or not in PATH" -ForegroundColor Red
    Write-Host "  Please install CMake first or run the setup-dev-environment.ps1 script" -ForegroundColor Yellow
    exit 1
}
Write-Host "✓ CMake is available" -ForegroundColor Green

# Check Internet
if (-not (Test-Internet)) {
    Write-Host "✗ No internet connection detected" -ForegroundColor Red
    Write-Host "  Internet is required to clone repository and download dependencies" -ForegroundColor Yellow
    exit 1
}
Write-Host "✓ Internet connection available" -ForegroundColor Green

Write-Host ""

# =============================================================================
# Step 1: Clone Repository
# =============================================================================
Write-Host "[1/4] Cloning Repository" -ForegroundColor Cyan
Write-Host "------------------------" -ForegroundColor Cyan

if ($SkipClone) {
    Write-Host "Skipping clone (using existing directory)" -ForegroundColor Yellow
    if (-not (Test-Path $ProjectPath)) {
        Write-Host "✗ Project path does not exist: $ProjectPath" -ForegroundColor Red
        exit 1
    }
}
else {
    # Check if directory already exists
    if (Test-Path $ProjectPath) {
        Write-Host "Directory already exists: $ProjectPath" -ForegroundColor Yellow
        $overwrite = Read-Host "Do you want to delete it and re-clone? (y/N)"
        if ($overwrite -eq "y" -or $overwrite -eq "Y") {
            Write-Host "Removing existing directory..." -ForegroundColor Yellow
            Remove-Item -Path $ProjectPath -Recurse -Force
        }
        else {
            Write-Host "Using existing directory (will update submodules)" -ForegroundColor Yellow
            $SkipClone = $true
        }
    }
    
    if (-not $SkipClone) {
        # Get parent directory
        $parentDir = Split-Path -Parent $ProjectPath
        
        # Create parent directory if it doesn't exist
        if (-not (Test-Path $parentDir)) {
            Write-Host "Creating parent directory: $parentDir" -ForegroundColor Gray
            New-Item -ItemType Directory -Force -Path $parentDir | Out-Null
        }
        
        # Clone repository
        Write-Host "Cloning from: $repoUrl" -ForegroundColor Gray
        Write-Host "To: $ProjectPath" -ForegroundColor Gray
        
        try {
            git clone --recurse-submodules $repoUrl $ProjectPath
            if ($LASTEXITCODE -eq 0) {
                Write-Host "✓ Repository cloned successfully!" -ForegroundColor Green
            }
            else {
                throw "Git clone failed with exit code $LASTEXITCODE"
            }
        }
        catch {
            Write-Host "✗ Failed to clone repository" -ForegroundColor Red
            Write-Host "  Error: $_" -ForegroundColor Red
            exit 1
        }
    }
}

# Navigate to project directory
Set-Location $ProjectPath
Write-Host "Working directory: $ProjectPath" -ForegroundColor Gray

# Update submodules if directory already existed
if ($SkipClone) {
    Write-Host "Updating submodules..." -ForegroundColor Yellow
    git submodule update --init --recursive
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Submodules updated" -ForegroundColor Green
    }
}

Write-Host ""

# =============================================================================
# Step 2: Download WebView2 SDK
# =============================================================================
Write-Host "[2/4] Downloading WebView2 SDK" -ForegroundColor Cyan
Write-Host "-------------------------------" -ForegroundColor Cyan

$webview2Path = "lib/webview2"
$webview2Url = "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.2792.45"

# Check if WebView2 already exists
if ((Test-Path $webview2Path) -and -not $CleanBuild) {
    Write-Host "WebView2 SDK already exists at: $webview2Path" -ForegroundColor Yellow
    $redownload = Read-Host "Do you want to re-download it? (y/N)"
    if ($redownload -ne "y" -and $redownload -ne "Y") {
        Write-Host "Skipping WebView2 download" -ForegroundColor Yellow
        $skipWebView2 = $true
    }
    else {
        Write-Host "Removing existing WebView2..." -ForegroundColor Yellow
        Remove-Item -Path $webview2Path -Recurse -Force
    }
}

if (-not $skipWebView2) {
    try {
        Write-Host "Downloading WebView2 SDK from NuGet..." -ForegroundColor Gray
        Write-Host "URL: $webview2Url" -ForegroundColor Gray
        
        # Download
        $tempZip = "webview2_temp.zip"
        $ProgressPreference = 'SilentlyContinue'
        Invoke-WebRequest -Uri $webview2Url -OutFile $tempZip -UseBasicParsing
        $ProgressPreference = 'Continue'
        
        Write-Host "✓ Downloaded WebView2 package" -ForegroundColor Green
        
        # Create lib directory if it doesn't exist
        if (-not (Test-Path "lib")) {
            New-Item -ItemType Directory -Force -Path "lib" | Out-Null
        }
        
        # Extract
        Write-Host "Extracting WebView2 SDK..." -ForegroundColor Gray
        Expand-Archive -Path $tempZip -DestinationPath $webview2Path -Force
        
        # Clean up zip file
        Remove-Item $tempZip -Force
        
        Write-Host "✓ WebView2 SDK extracted to: $webview2Path" -ForegroundColor Green
        
        # Display extracted contents
        $fileCount = (Get-ChildItem -Path $webview2Path -Recurse -File).Count
        Write-Host "  Extracted $fileCount files" -ForegroundColor Gray
    }
    catch {
        Write-Host "✗ Failed to download/extract WebView2 SDK" -ForegroundColor Red
        Write-Host "  Error: $_" -ForegroundColor Red
        Write-Host "  You may need to download it manually from:" -ForegroundColor Yellow
        Write-Host "  $webview2Url" -ForegroundColor Yellow
        exit 1
    }
}

Write-Host ""

# =============================================================================
# Step 3: Check for Additional Dependencies
# =============================================================================
Write-Host "[3/4] Checking Additional Dependencies" -ForegroundColor Cyan
Write-Host "---------------------------------------" -ForegroundColor Cyan

# Check if CMakeLists.txt exists
if (-not (Test-Path "CMakeLists.txt")) {
    Write-Host "✗ CMakeLists.txt not found in project root" -ForegroundColor Red
    Write-Host "  This might not be a CMake project" -ForegroundColor Yellow
}
else {
    Write-Host "✓ CMakeLists.txt found" -ForegroundColor Green
    
    # Try to detect other dependencies from CMakeLists.txt
    Write-Host "Scanning CMakeLists.txt for dependencies..." -ForegroundColor Gray
    $cmakeContent = Get-Content "CMakeLists.txt" -Raw
    
    # Check for common dependencies
    $dependencies = @()
    if ($cmakeContent -match "find_package\((\w+)") {
        $dependencies += $matches[1]
    }
    
    if ($dependencies.Count -gt 0) {
        Write-Host "Detected dependencies:" -ForegroundColor Gray
        foreach ($dep in $dependencies) {
            Write-Host "  - $dep" -ForegroundColor Gray
        }
    }
}

# Check for package.json (if it's a Node.js project too)
if (Test-Path "package.json") {
    Write-Host "✓ package.json found" -ForegroundColor Green
    if (Test-Command "npm") {
        Write-Host "Installing npm dependencies..." -ForegroundColor Yellow
        npm install
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ npm dependencies installed" -ForegroundColor Green
        }
    }
    else {
        Write-Host "⚠ npm not found, skipping npm install" -ForegroundColor Yellow
    }
}

# Check for requirements.txt (if it's a Python project too)
if (Test-Path "requirements.txt") {
    Write-Host "✓ requirements.txt found" -ForegroundColor Green
    if (Test-Command "pip") {
        Write-Host "Installing pip dependencies..." -ForegroundColor Yellow
        pip install -r requirements.txt
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ pip dependencies installed" -ForegroundColor Green
        }
    }
    else {
        Write-Host "⚠ pip not found, skipping pip install" -ForegroundColor Yellow
    }
}

Write-Host ""

# =============================================================================
# Step 4: Project Summary
# =============================================================================
Write-Host "[4/4] Project Summary" -ForegroundColor Cyan
Write-Host "---------------------" -ForegroundColor Cyan

Write-Host ""
Write-Host "Project Location: $ProjectPath" -ForegroundColor White
Write-Host "Repository: $repoUrl" -ForegroundColor White
Write-Host ""

# Display directory structure (top level)
Write-Host "Project Structure:" -ForegroundColor Gray
Get-ChildItem -Directory | ForEach-Object {
    Write-Host "  📁 $($_.Name)" -ForegroundColor Gray
}
Get-ChildItem -File | Select-Object -First 10 | ForEach-Object {
    Write-Host "  📄 $($_.Name)" -ForegroundColor Gray
}

Write-Host ""
Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host "  Initialization Complete!" -ForegroundColor Green
Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "Next Steps:" -ForegroundColor Yellow
Write-Host ""
Write-Host "1. Open Developer PowerShell for VS 2022" -ForegroundColor White
Write-Host "   (Search in Start Menu)" -ForegroundColor Gray
Write-Host ""
Write-Host "2. Navigate to project directory:" -ForegroundColor White
Write-Host "   cd `"$ProjectPath`"" -ForegroundColor Cyan
Write-Host ""
Write-Host "3. Configure the build:" -ForegroundColor White
Write-Host "   cmake -B build -DCMAKE_BUILD_TYPE=Release" -ForegroundColor Cyan
Write-Host ""
Write-Host "4. Build the project:" -ForegroundColor White
Write-Host "   cmake --build build --config Release" -ForegroundColor Cyan
Write-Host ""
Write-Host "5. Run the executable:" -ForegroundColor White
Write-Host "   .\build\bin\Release\bank_account_system.exe" -ForegroundColor Cyan
Write-Host ""

# Offer to open the directory in explorer
$openExplorer = Read-Host "Do you want to open the project directory in File Explorer? (Y/n)"
if ($openExplorer -ne "n" -and $openExplorer -ne "N") {
    Start-Process explorer.exe $ProjectPath
}

Write-Host ""
Write-Host "Happy coding! 🚀" -ForegroundColor Green
Write-Host ""
