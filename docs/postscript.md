# Scripting

For scripting in application uses JavaScript language.

## Post scripting

### Global objects
Post scripts have access to global objects `response` and `result`. Object `response` using for get data about result of HTTP request. Object `result` usings for set of result validation HTTP request.

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

## Pre scripting

## Web socket scripting
