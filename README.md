# ArdorQuery
Cross platform desktop a tool for testing HTTP(S) endpoints from websites or RESTful APIs.  
The main goal is to make an application with minimal user interface and keyboard oriented.  
Application is fully native, your don't need to install additional stuff.  
The HTTP request is broken down into a human-readable set of lines.  
Each line describes the type at the beginning, for example `url ` means we define the URL for the HTTP request.  
Each type of line has a different color to help visually identify that content.
  
![Screenshoot](https://github.com/trueromanus/ArdorQuery/raw/main/src/screenshoot.png)
## Features
* Making insecure and secure HTTP(s) requests
* Small set of human-readable fields types
* Parts of request filled in separate text blocks with word wrap. The entire request is visible and readable.
* Import/Export field commands to/from clipboard or file
* Supporting h2 by default (can be lowered to HTTP/1.1 with special field)
* Sending any HTTP headers or custom headers
* Attaching files from the file system
* Sending forms
* Sending any body (aliases for JSON and XML)
* Highlight responses for popular formats - JSON, XML, HTML
* Showing responses as images - JPEG, PNG, GIF, WEBP, SVG
* Adding additional queries and switching between it
* Search text in body
* Image generation for attaching to the messager, bug tracker etc
* Import from OpenAPI v3 (supported only JSON body)
* Running multiple queries
* Post Scripting in Modern JavaScript
* Control over redirection and SSL checks
* Global variables

## Field types
* `url X` - where X is valid url. This is the URL that will be requested.
* `method X` - where X can be - GET, POST, PUT, DELETE. This command defines the request method.
* `form X=Y` - where X it name of form parameter and Y it value. This command adds a value to the form in the body.
* `file X=Y` - where X it name of form parameter and Y it path to file in filesystem. This command adds a file to the form in the body.
* `param X=Y` - where X it name of query parameter and Y it value. This command adds parameter to the URL.
* `pastry X=Y` - where X it name of cookie parameter and Y it value. This command adds parameter to `Cookie` header.
* `body X` - where X it body content. This command adding body to request.
* `header X` (also it have shortcut `X-` if header started with it) - where X it any custom header. This command adds a custom header to the request.
* `X Y` - where X it any header described in HTTP spec. This command adds a standard header to the request.
* `title X` - where X it any characters. This command specifies the title for the current request, by default it is called "Unnamed".
* `protocol X` - where X can be - `1.1`. By default, all requests will try to make requests in the h2 protocol, but if you need to force it to do in the HTTP/1.1 protocol, you can add this command.
* `route X=Y` - where X it name of URL segment and Y of it value. `http://test.com/{segment}` can be adjusted with `route segment=3`. Result will be `http://test.com/3`
* `postscript X` - where X is the JavaScript source code for validating the HTTP response and generating custom errors. Check out JavaScript docs in section [PostScripting](https://github.com/trueromanus/ArdorQuery/blob/main/README.md#post-scripting).
* `options X,Y,Z` - where X Y and Z it option name. Supported options in section [Options](https://github.com/trueromanus/ArdorQuery/blob/main/README.md#options)

## Options
* `noautoredirect` - If the endpoint tries to redirect, this will not happen. In response, you will see information about the redirect.
* `autoredirect` - If the endpoint tries to redirect, this will happen without any limitation. The only exception is a redirect between **https** to **http** which is not allowed.
* `autoredirectsameorigin` - If the endpoint tries to redirect, this will happen only if it happened on same host, port and protocol.
* `weaksslcheck` - The request a certificate from the peer will be happened, but does not require this certificate to be valid.
* `noweaksslcheck` - No SSL certificate verification. This can be useful if you are testing a web server using a developer/local certificate on local machine.

## Alias commands
### bearer
`bearer X` - where X is bearer token.  
  
`bearer 345453` turn on to `Authorization Bearer 345453`

### json
`json X` - where X it body content. This can be useful if you are making a request to a restful API.
  
`json {"result": true}` turn into several field commands:  
`Content-Type application/json`  
`Accept application/json`  
`body {"result": true}` 

### xmla
`xmla X` - where X it body content. This can be useful if you are making a request to a XML service or API.  
  
`xmla <xml></xml>` turn into several field commands:  
`Content-Type application/xml`  
`Accept application/xml`  
`body <xml></xml>` 

## Post Scripting
### Global objects
All scripts have access to global objects `response` and `result`. Object `response` using for get data about result of HTTP request. Object `result` usings for set of result validation HTTP request.

### Response
`headers` - array contains all response headers with values in format `HeaderName HeaderValue`.
`statusCode` - number in range 0-600 from HTTP status code.
`errorMessage` - if the request was in error, this field will contain a description of the error.
`bodySize` - number from header Content-Length (or the actual size of the response body if not specified).
`route` - the final URL with which the request was launched.
### Result
`hasErrors` (editable) - indicate if some error happened while postsctipr works.
`errorMessage` (editable) - may contain a description of the user's error, which is displayed next to the status code in the results tab.  
### Examples
Check status code it is equal 200 (OK):
```js
postscript result.hasErrors = response.statusCode == 200;
```
Check if the response has a `server` header:
```js
postscript 
if(!response.headers.find(a => a.indexOf("server"))) {
  result.hasErrors = true;
  result.errorMessage = "Header server not specified!!!";
}
```

## Global variables
You can define a global variable and use it in your queries. To open the `Global Variables` window, press "Ctrl-F6".  
The format of a global variable is the same as in the `<variable name> <value>` query editor.
You can press `Ctrl-Enter` to add a new line for create a new global variable.
At the end of edit session you need to save the global variables by pressing `Ctrl-S`.
After this you can use the global variable in your queries like `{{<name of variable>}}`.  
  
As example global variable `mytoken 4354534534534534534`
```
bearer {{mytoken}}
```
header will be replaced on `bearer 4354534534534534534`.

## Supported platforms
* Windows 10+
* Linux
* macOS (binary release coming soon)

## Requirements
* Qt >= 6.2.3
