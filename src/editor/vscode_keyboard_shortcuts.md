
### general cheetsheet

link:  
[keyboard-shortcuts-windows.pdf](https://code.visualstudio.com/shortcuts/keyboard-shortcuts-windows.pdf)

inline linke:  
![](keyboard-shortcuts-windows.pdf)


### 代码浏览

vscode 菜单栏的 **go** 选项

| cmd                   | keyboard                              | content               | 图示  |
| --------------------- | ------------------------------------- | --------------------- | --- |
| trigger suggest       | Ctrl + i                              | 写代码时触发代码补全提示框         |     |
| go to Definition      | F12                                   | 查看变量定义                |     |
| back                  | Ctrl + Alt + - /或 Alt + <-(leftarrow) | 返回原来位置                |     |
| go to Reference       | Shift + F12                           | 查看变量在哪些地方被引用(包括调用和实现) |     |
| go to implementations | Ctrl + F12                            | 查看变量实现的定义             |     |

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