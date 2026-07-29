[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"

$repositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
Push-Location $repositoryRoot

try {
    & git submodule update --init -- libKCD2
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to initialize the libKCD2 submodule."
    }

    & git -C libKCD2 config `
        submodule.Projects/KCSE.url `
        https://github.com/JerryYOJ/KCSE-for-kcd2.git
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to configure the KCSE HTTPS submodule URL."
    }

    & git -C libKCD2 submodule update --init -- Projects/KCSE
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to initialize libKCD2's KCSE submodule."
    }

    Write-Host "libKCD2 and KCSE are initialized at their pinned commits."
}
finally {
    Pop-Location
}
