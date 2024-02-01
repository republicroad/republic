
## 系统包管理器

### apt & dpkg 异同点  
    1. apt 与 dpkg 均为 ubuntu 下面的包管理工具。
    2. dpkg 仅用于安装本地的软件包，安装时不会安装依赖包，不解决依赖问题。
    3. apt 默认会从远程仓库搜索包的名字，下载并安装，安装时会自动安装依赖包，并解决依赖问题。如果需要使用apt 从本地安装，需要在包名前指定路径，否则只从远程仓库查找。

### apt 包管理工具
    - 参考文档: https://manpages.ubuntu.com/manpages/focal/en/man8/apt.8.html 

    1. apt 包管理器简介
        最常用的 Linux 包管理命令都被分散在了 apt-get、apt-cache、apt-config 这三条命令当中。
        apt 命令的引入就是为了解决命令过于分散的问题，它包括了 apt-get 命令出现以来使用最广泛的功能选项，以及 apt-cache 和 apt-config 命令中很少用到的功能。
        在使用 apt 命令时，用户不必再由 apt-get 转到 apt-cache 或 apt-config，而且 apt 更加结构化，并为用户提供了管理软件包所需的必要选项。
        简单来说就是：apt = apt-get、apt-cache 和 apt-config 中最常用命令选项的集合。
    
    2. apt 常用命令
        ```bash
        apt --help                 
          apt 2.0.6 (amd64)
          用法： apt [选项] 命令
            
          常用命令：
              list         - 根据名称列出软件包
              search       - 搜索软件包描述
              show         - 显示软件包细节
              install      - 安装软件包
              reinstall    - 重新安装软件包
              remove       - 移除软件包
              autoremove   - 卸载所有自动安装且不再使用的软件包
              update       - 更新可用软件包列表
              upgrade      - 通过 安装/升级 软件来更新系统
              full-upgrade - 通过 卸载/安装/升级 来更新系统
              edit-sources - 编辑软件源信息文件
              satisfy      - 使系统满足依赖关系字符串   
        ```


### dpkg
    - 参考文档: https://manpages.ubuntu.com/manpages/focal/man1/dpkg.1.html 

    1. dpkg 包管理器简介
        dpkg 包管理器是 Linux 系统中一个基本的软件包管理工具，用于安装、升级、卸载和查询软件包。它通常用于 Debian 系统和基于 Debian 的 Linux 发行版，如 Ubuntu 和 Linux Mint 等。
    
    2. dpkg 常用命令
        ```bash
        dpkg --help           
          用法：dpkg [<选项> ...] <命令>

        命令：
            -i|--install       <.deb 文件名> ... | -R|--recursive <目录> ...
            --unpack           <.deb 文件名> ... | -R|--recursive <目录> ...
            -A|--record-avail  <.deb 文件名> ... | -R|--recursive <目录> ...
            --configure        <软件包名>    ... | -a|--pending
            --triggers-only    <软件包名>    ... | -a|--pending
            -r|--remove        <软件包名>    ... | -a|--pending
            -P|--purge         <软件包名>    ... | -a|--pending
            -V|--verify <软件包名> ...        检查包的完整性。
            --get-selections [<表达式> ...]  把已选中的软件包列表打印到标准输出。
            --set-selections                 从标准输入里读出要选择的软件。
            --clear-selections               取消选中所有不必要的软件包。
            --update-avail <软件包文件>      替换现有可安装的软件包信息。
            --merge-avail  <软件包文件>      把文件中的信息合并到系统中。
            --clear-avail                    清除现有的软件包信息。
            --forget-old-unavail             忘却已被卸载的不可安装的软件包。
            -s|--status      <软件包名> ...  显示指定软件包的详细状态。
            -p|--print-avail <软件包名> ...  显示可供安装的软件版本。
            -L|--listfiles   <软件包名> ...  列出属于指定软件包的文件。
            -l|--list  [<表达式> ...]        简明地列出软件包的状态。
            -S|--search <表达式> ...         搜索含有指定文件的软件包。
            -C|--audit [<表达式> ...]        检查是否有软件包残损。
            --yet-to-unpack                  列出标记为待解压的软件包。
            --predep-package                 列出待解压的预依赖。
            --add-architecture    <体系结构> 添加 <体系结构> 到体系结构列表。
            --remove-architecture <体系结构> 从架构列表中移除 <体系结构>。
            --print-architecture             显示 dpkg 体系结构。
            --print-foreign-architectures    显示已启用的异质体系结构。
            --assert-<特性>                  对指定特性启用断言支持。
            --validate-<属性> <字符串>       验证一个 <属性>的 <字符串>。
            --compare-vesions <a> <关系> <b>  比较版本号 - 见下。
            --force-help                     显示本强制选项的帮助信息。
            -Dh|--debug=help                 显示有关出错调试的帮助信息。

            -?, --help                       显示本帮助信息。
                --version                    显示版本信息。 
        ```



### yum


### dnf

- [rpm软件包管理器-dnf](https://wangchujiang.com/linux-command/c/dnf.html)


### nix


## lang-包管理器


### pip(python)


### maven(java)