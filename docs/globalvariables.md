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

### Navigation
`PgDown` - Select a text field below the currently selected field.  
`PgUp` - Select a text field above the currently selected field.  
`Ctrl-PgDown` - Selecting end a text field.  
`Ctrl-PgUp` - Selecting start a text field.

### Working with lines
`Alt-Enter` - Adding a new empty line on top of the selected line.  
`Shift-Enter` - Adding a new empty line at the end of the lines.  
`Control-R` - Remove selected line.  
`Shift-Alt-R` - Remove all lines.

### Quickly insert global variables in the Query Editor
You can insert global variables in the Query Editor. To do this, you can place the cursor where you want, insert a global variable and press `Ctrl-0`.
After this, a pop-up window opens with all global variables. You can select the desired variable by pressing `Up` or `Down`, and press `Enter` to insert the variable or `Escape` to cancel operation.

### Predefined global variables
You can use predefined global variables to inject some stuff like date, time, etc.

#### `dateTimeNow`

It will be replaced to the current date according to local time in ISO 8601 format.
Example: `2024-01-01T12:00:00`

#### `dateTimeUtcNow`

It will be replaced to the current date according to UTC in ISO 8601 format.
Example: `2024-01-01T12:00:00`

#### `time24HoursNow`

It will be replaced to current local time in 24 hours format.
Example: `18:00:00`
