# Field aliases

An alias combines multiple fields into a single field.

#### `bearer <token>`
Added authorization header with bearer type and value from `token`.
  
Example:
`bearer ABCDEF` -> `Authorization Bearer ABCDEF`

#### `json <content>`
where `content` it body content. This can be useful if you are making a request to a restful API.
  
Example:
`json {"result": true}`
->
`Content-Type application/json`
`Accept application/json`
`body {"result": true}`

#### `xmla <content>`
where `content` it body content. This can be useful if you are making a request to a restful API.
  
Example:
`xmla <xml></xml>`
->
`Content-Type application/xml`
`Accept application/xml`
`body <xml></xml>`