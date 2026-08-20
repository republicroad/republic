$ErrorActionPreference = "Stop"
$MaxRetries = 5
$RetryCount = 0

Write-Host "=== Podman Host IP Updater ===" -ForegroundColor Cyan

function Is-PodmanMachineRunning {
    try {
        $status = podman machine ls --format "{{.Name}} {{.Running}}" 2>$null
        return ($status -match "Running")
    } catch {
        return $false
    }
}

try {
    while ($RetryCount -lt $MaxRetries) {
        $RetryCount++
        Write-Host "Attempt $RetryCount of $MaxRetries..." -ForegroundColor Yellow

        if (-not (Is-PodmanMachineRunning)) {
            Write-Host "Podman machine is not running. Starting it..." -ForegroundColor Yellow
            podman machine start
            Start-Sleep -Seconds 10
        }

        Start-Sleep -Seconds 5

        try {
            $HOST_IP = podman machine ssh 'ip route show | grep -i default | awk ''{ print $3}''' 2>&1

            if ($HOST_IP -and $HOST_IP.Trim() -match '^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$') {
                $HOST_IP = $HOST_IP.Trim()
                Write-Host "Detected host IP: $HOST_IP" -ForegroundColor Green
                break
            } else {
                throw "Invalid or empty IP returned"
            }
        } catch {
            Write-Host "Connection failed (attempt $RetryCount). Retrying..." -ForegroundColor DarkYellow
            Start-Sleep -Seconds 6
        }
    }

    if ($RetryCount -eq $MaxRetries) {
        throw "Failed to connect to Podman machine after $MaxRetries attempts."
    }

    $updateHosts = @"
sudo sed -i '/host\.docker\.internal/d; /host\.containers\.internal/d' /etc/hosts
echo '$HOST_IP host.docker.internal host.containers.internal' | sudo tee -a /etc/hosts > /dev/null
"@
    podman machine ssh $updateHosts
    Write-Host "Updated /etc/hosts: host.docker.internal + host.containers.internal -> $HOST_IP" -ForegroundColor Green

    $updateConf = @"
mkdir -p ~/.config/containers
sed -i '/host_containers_internal_ip/d' ~/.config/containers/containers.conf
if ! grep -q '\[containers\]' ~/.config/containers/containers.conf 2>/dev/null; then
  echo '[containers]' >> ~/.config/containers/containers.conf
fi
sed -i '/\[containers\]/a host_containers_internal_ip = "$HOST_IP"' ~/.config/containers/containers.conf
"@
    podman machine ssh $updateConf
    Write-Host "Updated WSL2 containers.conf: host_containers_internal_ip -> $HOST_IP" -ForegroundColor Green

    $winConfFile = "$env:USERPROFILE\.config\containers\containers.conf"

    if (-not (Test-Path "$env:USERPROFILE\.config\containers")) {
        New-Item -ItemType Directory -Path "$env:USERPROFILE\.config\containers" -Force | Out-Null
    }

    $winConfContent = @"
[containers]
http_proxy = true
host_containers_internal_ip = "$HOST_IP"
env = [
  "HTTP_PROXY=http://${HOST_IP}:7890",
  "HTTPS_PROXY=http://${HOST_IP}:7890",
  "NO_PROXY=localhost,127.0.0.1"
]

[engine]
env = [
  "HTTP_PROXY=http://${HOST_IP}:7890",
  "HTTPS_PROXY=http://${HOST_IP}:7890",
  "NO_PROXY=localhost,127.0.0.1"
]
"@

    Set-Content -Path $winConfFile -Value $winConfContent -Encoding UTF8
    Write-Host "Updated user containers.conf: $winConfFile" -ForegroundColor Green

    $appDataConfFile = "$env:APPDATA\containers\containers.conf"

    if (Test-Path $appDataConfFile) {
        $appDataConfContent = @"
[containers]
http_proxy = true
host_containers_internal_ip = "$HOST_IP"
env = [
  "HTTP_PROXY=http://${HOST_IP}:7890",
  "HTTPS_PROXY=http://${HOST_IP}:7890",
  "NO_PROXY=localhost,127.0.0.1"
]

[engine]
env = [
  "HTTP_PROXY=http://${HOST_IP}:7890",
  "HTTPS_PROXY=http://${HOST_IP}:7890",
  "NO_PROXY=localhost,127.0.0.1"
]

[machine]
[network]
[secrets]
[configmaps]
"@

        Set-Content -Path $appDataConfFile -Value $appDataConfContent -Encoding UTF8
        Write-Host "Updated AppData containers.conf: $appDataConfFile" -ForegroundColor Green
    }

    $scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
    $registriesSrc = Join-Path $scriptDir "registries.conf"

    if (Test-Path $registriesSrc) {
        wsl -- sudo cp /mnt/c/Users/RYefccd/AppData/Local/Temp/registries.conf /etc/containers/registries.conf 2>$null
        wsl -- bash -c "cat $registriesSrc | sudo tee /etc/containers/registries.conf > /dev/null" 2>$null

        $wslPath = "/mnt/c/" + $registriesSrc.Replace('\', '/').Replace('C:', 'Users/RYefccd')
        wsl -- sudo cp "$wslPath" /etc/containers/registries.conf 2>$null

        if ($LASTEXITCODE -eq 0) {
            Write-Host "Synced registries.conf to WSL2" -ForegroundColor Green
        } else {
            Write-Host "Note: registries.conf sync skipped (run manually if needed)" -ForegroundColor Yellow
        }
    }

    Write-Host "=== Done ===" -ForegroundColor Cyan
} catch {
    Write-Error "Error: $($_.Exception.Message)"
    exit 1
}
