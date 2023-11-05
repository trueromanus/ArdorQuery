# Options

Options are written as a set of words separated by a comma (option 1, option 2).  
Example: `options noautoredirect,noweaksslcheck`.

## Change redirect behaviour

Default behaviour is auto redirect if server responsed any from 30x HTTP codes.

* `noautoredirect` - If the endpoint tries to redirect, this will not happen. In response, you will see information about the redirect.
* `autoredirect` - If the endpoint tries to redirect, this will happen without any limitation. The only exception is a redirect between **https** to **http** which is not allowed.
* `autoredirectsameorigin` - If the endpoint tries to redirect, this will happen only if it happened on same host, port and protocol.

## Check SSL Certificate

Default behaviour is strong SSL check.

* `weaksslcheck` - The request a certificate from the peer will be happened, but does not require this certificate to be valid.
* `noweaksslcheck` - No SSL certificate verification. This can be useful if you are testing a web server using a developer/local certificate on local machine.