# windows 系统下安装ubuntu及lxd

+ 参考文档： https://learn.microsoft.com/zh-cn/windows/wsl/install-manual#step-4---download-the-linux-kernel-update-package


# 详细步骤

## window系统下安装linux子系统

### 1. window系统--> 以管理员身份打开 PowerShell（“开始”菜单 >“PowerShell” >单击右键 >“以管理员身份运行”）


### 2. 按照文档提示第一步，然后输入以下命令：

> dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart

### 3. 重新启动计算机，然后继续执行下一步

### 4. 按照文档提示第二步，检查系统版本

    1. 首先检查 Windows 版本及内部版本号，选择 Windows 徽标键 + R，然后键入“winver”，选择“确定”。低于 18362 的版本不支持 WSL 2。 使用 Windows Update 助手更新 Windows 版本。 

    2.Windows系统升级到最新版本后，重启计算机

### 5. 按照文档第三步，启用虚拟机功能

    1. 在powershell 中执行下面的命令
    dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart

    2. 重启计算机

### 6. 按照文档第四步，下载 Linux 内核更新包

    1.点击文档链接安装 适用于 x64 计算机的 WSL2 Linux 内核更新包

    2.运行上一步中下载的更新包。 （双击以运行 - 系统将提示你提供提升的权限，选择“是”以批准此安装。）

### 7. 将 WSL 2 设置为默认版本

    1. 打开 PowerShell，然后在安装新的 Linux 发行版时运行以下命令，将 WSL 2 设置为默认版本：

    wsl --set-default-version 2

    2. 检验wsl版本， 在PowerShell中执行以下命令, 查看结果

    wsl --version

    如果结果显示为如下，则说明配置wsl2成功

        WSL 版本： 2.0.9.0
        内核版本： 5.15.133.1-1
        WSLg 版本： 1.0.59
        MSRDC 版本： 1.2.4677
        Direct3D 版本： 1.611.1-81528511
        DXCore 版本： 10.0.25131.1002-220531-1700.rs-onecore-base2-hyp
        Windows 版本： 10.0.19045.3693

    如果没有成功执行以上命令，则需要重新升级wsl 可执行以下命令

        wsl.exe --update

    升级完毕后，继续执行上面2中的命令”wsl --version“ 查看结果

### 8.  安装Ubuntu子系统
    1. 打开 Microsoft Store 搜索Ubuntu应用，可参考文档链接进行跳转到对应版本
    2. 点击安装


## 在Ubuntu子系统安装lxd

### 1. window系统--> 以管理员身份打开 PowerShell（“开始”菜单 >“PowerShell” >单击右键 >“以管理员身份运行”）执行命令：

    wsl


### 2.执行命令，确保主进程是由systemd启动
    1. 在ubuntu 终端中执行命令, 查看结果

    ps -e
    结果应该为：

     PID   TTY          TIME CMD
      1    ?        00:00:00 systemd
      2    ?        00:00:00 init-systemd(Ub
      5    ?        00:00:00 init
      56   ?        00:00:00 systemd-journal
      81   ?        00:00:00 systemd-udevd
      93   ?        00:00:00 systemd-network
      250  ?        00:00:03 snapfuse
      ...

    如果pid 为1 的进程不是由systemd 启动而是init启动的，则后续会影响snap的安装， 需要排查wsl2是否正常安装启动

### 3.安装依赖包管理工具snap

    1. 在ubuntu 终端中执行命令, 安装依赖包管理工具snap

    sudo apt update
    sudo apt install snapd

    2. 安装完毕后可执行以下命令查看安装结果

    sudo systemctl status snapd.service
    成功安装的结果显示为：

    ● snapd.service - Snap Daemon
        Loaded: loaded (/lib/systemd/system/snapd.service; enabled; vendor preset: enabled)
        Active: active (running) since Tue 2023-12-12 17:20:03 CST; 7min ago
    TriggeredBy: ● snapd.socket
        Main PID: 272 (snapd)
           Tasks: 13 (limit: 9449)
          Memory: 86.2M
          CGroup: /system.slice/snapd.service
                  └─272 /usr/lib/snapd/snapd


### 4.通过snap 安装最新版本的lxd

- 执行以下命令查看当前系统内的lxd版本

    >   snap info lxd

- 执行以下命令查询lxd 版本信息

    > snap info lxd

- 安装最新的lxd

    > sudo snap refresh --channel=latest/stable lxd
    







    