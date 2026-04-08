
### general cheetsheet

link:  
[keyboard-shortcuts-windows.pdf](https://code.visualstudio.com/shortcuts/keyboard-shortcuts-windows.pdf)

inline linke:  
![](keyboard-shortcuts-windows.pdf)


### 代码浏览

vscode 菜单栏的 **go** 选项

| cmd                        | Windows/Linux                         | macOS                            | content               |
| -------------------------- | ------------------------------------- | -------------------------------- | --------------------- |
| trigger suggest            | Ctrl + i                              |                                  | 写代码时触发代码补全提示框         |
| go to Definition           | F12                                   |                                  | 查看变量定义                |
| back                       | Ctrl + Alt + - /或 Alt + <-(leftarrow) |                                  | 返回原来位置                |
| go to Reference            | Shift + F12                           |                                  | 查看变量在哪些地方被引用(包括调用和实现) |
| go to implementations      | Ctrl + F12                            |                                  | 查看变量实现的定义             |
| **Format Document**        | Shift+Alt+F                           |                                  | 对代码文档格式化              |
| word warp                  | Alt+Z                                 |                                  | 代码折行展示                |
| Go to Bracket              | Ctrl + Shift + \                      |                                  | 寻找当前位置所在的"括号"(范围)     |
| **Fold** (Current block)   | `Ctrl` + `Shift` + `[`                | `Cmd` + `Option` + `[`           | 折叠代码块                 |
| **Unfold** (Current block) | `Ctrl` + `Shift` + `]`                | `Cmd` + `Option` + `]`           | 展开代码块                 |
| **Fold All**               | `Ctrl` + `K` then `Ctrl` + `0`        | `Cmd` + `K` then `Cmd` + `0`     | 折叠所有代码块               |
| **Unfold All**             | `Ctrl` + `K` then `Ctrl` + `J`        | `Cmd` + `K` then `Cmd` + `J`     | 展开所有代码块               |
| **Fold Level [1-5]**       | `Ctrl` + `K` then `Ctrl` + `[1-5]`    | `Cmd` + `K` then `Cmd` + `[1-5]` | 折叠不同级别                |


### vscode配置格式

```json
{
  "key": "ctrl+i",
  "command": "workbench.action.chat.stopListeningAndSubmit",
  "when": "hasChatProvider && hasSpeechProvider && inChatInput && voiceChatInProgress || hasChatProvider && hasSpeechProvider && inlineChatFocused && voiceChatInProgress"
}
```

https://github.com/codebling/vs-code-default-keybindings/blob/master/windows.negative.keybindings.json#L722


### 资料

https://code.visualstudio.com/docs/editor/intellisense#:~:text=You%20can%20trigger%20IntelliSense%20in,name%20to%20limit%20the%20suggestions.
https://github.com/codebling/vs-code-default-keybindings/tree/master