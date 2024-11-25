# Scripting

For scripting in application uses JavaScript language.

## Post scripting

### Global objects
Post scripts have access to global objects `response`, `result`, `globals` and `session`. Object `response` using for get data about result of HTTP request. Object `result` usings for set of result validation HTTP request.
Object `globals` usings for get access to global variables. The `session` object can be used to manage shared state between requests when used multiple run.

### response object
`headers` - array contains all response headers with values in format `HeaderName HeaderValue`.  
`statusCode` - number in range 0-600 from HTTP status code.  
`errorMessage` - if the request was in error, this field will contain a description of the error.  
`bodySize` - number from header Content-Length (or the actual size of the response body if not specified).  
`route` - the final URL with which the request was launched.
### result object
`hasErrors` (editable) - indicate if some error happened while postsctipr works.  
`errorMessage` (editable) - may contain a description of the user's error, which is displayed next to the status code in the results tab.  
### globals Object
`has(name)` (method) - if global variable set with name passed via parameter **name** it will be **true** in result, in other case **false**.  
`get(name)` (method) - if global variable set with name passed via parameter **name** it will be value as string in result, in other case empty string.  
### session Object
`shared` (member) - this member can be used to store any type of variable (string/number/object/array etc). This member can be useful in cases where we are making multiple requests and need to manage intermediate state between requests. Example: the first request does authorization and stores the username and userid in the `session.shared` profile, and uses it for checks in subsequent queries.  
`setRemapValue(name, value)` (method) - create (and redefine, if a global variable with the same name exists) a session variable. Created/redefined session variables can be used in subsequent requests in the same way as a simple global variable in the format `{{ name }}`.  
`getRemapValue(name)` (method) - get the value from a previously defined variable.  
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

## Pre scripting
In progress
## Web socket scripting
In progress