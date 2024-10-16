# Launch queries in a specific order

In some cases, it will be convenient to run queries not simultaneously, but sequentially, one after another. There are many real cases, for example, first need make authorization and then make several queries as authorized user, to emulate an order in an online store, you first need to add some product to the cart and only then buy it.

## Basic usage

For example, you have two queries - one with the title `First` and another with the title `Second`. If we want the `First` query to be run first, and only after it is finished we run the `Second` query. We need to open the `First` query and add a new field with the content `order 1`. After that we need to open the `Second` query and add a new field with the content `order 2`.  
We run multiple queries by pressing `Ctrl-m`, the progress can be seen in the window footer.

### Grouping with simultaneous launch inside groups
In some cases, you may find it convenient to perform queries not one at a time like this:

1. Query 1
2. Query 2
3. Query 3
4. Query 4
5. Query 5
6. Query 6
  
You can grouping run of queries like this:

1. Query 1
2. Query 2, Query 3, Query 4 (concurrently) 
3. Query 5
4. Query 6, Query 7 (concurrently)

To do this, you need to fill in the `order X` field as follows:

In Query 1 `order 1`
In Query 2 and Query 3 and Query 4 `order 2` (order must be same in all these queries)
In Query 5 `order 3`
In Query 6 and Query 7 `order 4` (order must be same in all these queries)


