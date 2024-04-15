# Import from OpenAPI

If you use OpenAPI for your API you can import routes from schema. To open the `Import from OpenAPI` window, press `F6`.
Now supported only schemes v3+ schemas in JSON format.
  
### Download routes from schema
To download a schema, you must at first fill in the `Scheme URL` field in the `Importer` tab.
After it just press `F5` or `Ctrl-Z` for start download.
List of loaded routes will be showed in lower part of window.
If you need to load the schema from a local file, you need adjust URL with protocol `file://` (`file:///` on Windows) and add the file path after it
(example `file:///home/tester/schema.json`).

### Navigation in routes
You can use the mouse to scroll the list, but you can also use the keys to navigate:
`PgUp` - Select previous route.  
`PgDown` - Select next route.  

### Import route to Query Editor
After you download schema and select routes you can add selected route to `Query Editor`.  
First of all you need to fill the `Base URL` field, the content from this field will be placed in the `url ` field as the base path.
As an example, I have an API address `https://myapi.com/api`, I fill the `Base URL` field with it, and after that, if I select the `/test` route, it will be automatically merged with the base URL - address and the result will be `https://myapi.com/api/test`.  
`Ctrl-Enter` - Add the selected route to a new query.  
`Shift-Enter` - Replace current query on selected route.  
You can also click on the desired route with the left mouse button.

### Filter routes
You can filter routes by route path, route description or method. For filtering just type something in field `Filter`.
Field `Filter` support multi filter conditions just type word between space and each of word typed like this mean additional filter.

### Select authorization method
It quite often API have two and more authorization options.
If some route have two and more options you need select it manually in `Security` field.
You can check in the schema what names it has.

### Saved Options
If you work with the same API frequently, it may be useful to save field values rather than having to enter them each time.

#### Add new profile
After you fill fields

#### Load and edit profile
`Saved Options`


#### Delete profile

