# bash 

https://www.gnu.org/software/bash/manual/html_node/Pattern-Matching.html  
https://unix.stackexchange.com/questions/255338/how-to-know-if-extglob-is-enabled-in-the-current-bash-session  

## 技巧

### 删除文件但是保留某些尾缀文件

比如编译的时候保留源码文件

```bash
shopt -s extglob
rm !(*.c|*.md)
```