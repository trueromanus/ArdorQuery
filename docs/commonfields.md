# Fields

Fields marked **[single]** must only be specified once in the request. If you specify them several times, the value will be taken from the last line in order.

## Basic fields

#### `url X` **[single]**  
Specifies URL for the HTTP request, where X it full URL.  
Example: `url https://github.com`

#### `method X` **[single]**  
Specifies the HTTP request method. Where X can be one of the possible values: **GET**, **POST**, **PUT**, **DELETE**, **PATCH**.  
Example: `method POST`

#### `body X` **[single]**  
Specify the content in the body for the HTTP request.  
Example: `body {"jsonField": "jsonValue" }`

## Form fields
 
#### `form X=Y`  
Add form field where X it name of field and Y it value.  
Example: `form count=100`

#### `file X=Y`  
Add file field where X it name of file field and Y it full path for file in filesystem.  
Example: `file poster=~/images/poster.jpg`

## Header fields

#### `<header name> <value>`  
Add header field for one of standart headers, where **header name** it name of standart header and **value** it header value.  
You can see full list of standard headers [on this page](https://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Standard_request_fields).  
  
Example: `Content-Type application/json`

#### `X-<header name> <value>`  
Add header field for non standart headers, where **header name** it name of header and **value** it header value.  
For a while there was a format for custom headers like `X-<Header Name>`.
  
Example: `X-USERID 550`

#### `header <header name> <value>`  
Add header field for non standart headers, where **header name** it name of header and **value** it header value.  
  
Example: `header USERID 550`

#### `pastry <cookie name>=<value>`
Adding parameter to `Cookie` header, where **cookie name** it name of header and **value** it value.  

  
Example: `header USERID 550`

#### `protocol <value>`
By default, all HTTPS requests will attempt to use the HTTP/2 protocol. If this is undesirable behavior, you can force requests to run over HTTP/1.1 defining this field.  
  
Example: `protocol 1.1`


## URL improve fields

#### `param <parameter name>=<value>`  
Adding a URL parameter, where **parameter name** is the name of the parameter and **value** is its value.
For ease of readability, it may be helpful if the URL itself and its parameters are separated. For example, I have a full URL **http://test.com?id=20**.
We can split it in the application into two lines: the first `url http://test.com` and the second `param id=20`.  
  
Example: `param userId=1`

#### `route <route name>=<value>`  
Adding a URL parameter, where **route name** is the name of the parameter and **value** is its value.
The modern way is to add URL parameters as part of the route, like **http://test.com/products/20/product**, where **20** is actually an  parameter like identifier.
We can split it in the application into two lines: the first `url http://test.com/products/{id}/product` and the second `route id=20`.
The application supports placing the same segment in multiple places, for example `http://test.com/products/{id}/product/{id}`.  
  
Example: `route segment=2`

## Scripting

#### `postscript <script>`  
Adding a postscript, where **script** is a script written in JavaScript to process the request result.  
The script can be used to validate results using custom logic. You can read more about scripting on [this page](postscript.md).
  
Example: `postscript result.hasErrors = response.statusCode != 200;`

## Extra fields

#### `title <name>`  
Adding title for query, where **name** is the name of the query.  
The title will appear in the request list and can be used as a human-readable description.
  
Example: `title Query for get all products`

#### `options <option1>,<option2>,<option3>`
Adding options for performing query, where **optionX** is the name of the option.  
You can check the list of all options on [this page](options.md).
  
Example: `options noautoredirect,weaksslcheck`