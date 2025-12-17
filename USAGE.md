
# Bank Account System – Installation and Usage Guide

This guide explains how to set up and build the **Bank Account System** project on a fresh Windows environment.

---

## Prerequisites

- **PowerShell 5.1+** (Windows PowerShell or PowerShell Core)
- **Git**
- **CMake**
- **MSVC / Visual Studio Build Tools**
- **VS Code** (recommended)

---

## 1. Setup Environment (One-Time)

Windows may block PowerShell scripts downloaded from the internet (such as from GitHub).
Follow these steps to allow the setup script to run safely.

1. Open **PowerShell** (no administrator rights required).

2. Allow locally downloaded scripts for your user:

```powershell
Set-ExecutionPolicy -Scope CurrentUser RemoteSigned
```

3. Navigate to the project root:

```powershell
cd path\to\Bank-Account-System
```

4. Unblock the setup script:

```powershell
Unblock-File .\.scripts\setup-environment.ps1
```

5. Run the setup script:

```powershell
.\.scripts\setup-environment.ps1
```

## 2. Initialize Project (Once per Clone)

In the same PowerShell window, run:

```powershell
Unblock-File .\.scripts\initialize-project.ps1
.\.scripts\initialize-project.ps1
```

This prepares the project files and configuration.

## 3. Build Project

To build the project, run:

```powershell
Unblock-File .\.scripts\build.ps1
.\.scripts\build.ps1
```

Alternatively, in VS Code press **Ctrl + Shift + B**
