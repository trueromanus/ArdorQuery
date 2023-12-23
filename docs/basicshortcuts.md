# Getting started and Basic shortcuts

Any HTTP request in an application is represented as a set of lines, each line contains some field.
Each field contains a separate part of the HTTP request, such as a URL address, method, header, parameter, etc.
There can be only one selected field, and it is highlighted with a colored frame.  

## Simple steps to get started with the application

* Once application is launched, enter `url https://github.com/` in the one existing field.
* You need to add a new field by pressing `Ctrl-Enter`(`Cmd-Enter` on macOS).
* Enter in the new field - `method GET`
* To make an HTTP request, press `Ctrl-Z` or `F5`.
* For change tab on Result need press `F11`.
* After the HTTP request completes, you can see the `Status`, `Response Time` and `Response Size`, headers and body of the response if it was received from the server.

**Most used shortcuts:**

## List of keyboard shortcuts

* `F1` or `Ctrl-H` - If you forgot a shortcut, you can check it at any time in the popup-help. It contains search by name or keyboard shortcut.

## Manage fields

* `Ctrl-Enter` - Adding a new empty field below the selected field.
* `Alt-Enter` - Adding a new empty field on top of the selected field.
* `Shift-Enter` - Adding a new empty field at the end of the fields.

## Tabs management

* `F11` - Switching between tabs for HTTP request fields and `Result`.

## Executing the request

* `Ctrl-Z` or `F5` - Run HTTP request
* `Ctrl-B` or `F4` - Cancel an HTTP request if it is not completed

You can check the full documentation about all fields at this [link](commonfields.md)