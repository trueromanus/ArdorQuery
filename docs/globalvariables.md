# Global variables

You can define a global variable and use it in your queries. To open the `Global Variables` window, press `Ctrl-F6`/`Command-F6`.
The format of a global variable is the same as in the `<variable name> <value>` query editor.
You can press `Ctrl-Enter` to add a new line for create a new global variable.
At the end of edit session you need to save the global variables by pressing `Ctrl-S`/`Command-S`.
After this you can use the global variable in your queries like `{{<name of variable>}}`.  
  
As example global variable `mytoken 4354534534534534534`
```
bearer {{mytoken}}
```
header will be replaced on `bearer 4354534534534534534`.
