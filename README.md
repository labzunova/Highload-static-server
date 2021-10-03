# Highload-static-server
Server to serve static files from disk using thread pool architecture

=====================
## Requirements ##

* Respond to `GET` with status code in `{200,404,403}`
* Respond to `HEAD` with status code in `{200,404,403}`
* Respond to all other request methods with status code `405`
* Directory index file name `index.html`
* Respond to requests for `/<file>.html` with the contents of `DOCUMENT_ROOT/<file>.html`
* Requests for `/<directory>/` should be interpreted as requests for `DOCUMENT_ROOT/<directory>/index.html`
* Respond with the following header fields for all requests:
    * `Server`
    * `Date`
    * `Connection`
* Respond with the following additional header fields for all `200` responses to `GET` and `HEAD` requests:
    * `Content-Length`
    * `Content-Type`
* Respond with correct `Content-Type` for `.html, .css, js, jpg, .jpeg, .png, .gif, .swf`
* Respond to percent-encoding URLs
* Correctly serve a 2GB+ files
* No security vulnerabilities

## Testing environment ##

Config file spec:
```
cpu_limit 4       # maximum CPU count to use (for non-blocking servers)
thread_limit 256  # maximum simultaneous connections (for blocking servers)
document_root /var/www/html
```

Run tests:
```
git clone https://github.com/init/http-test-suite.git
cd http-test-suite

docker build -t server-httpd https://github.com/labzunova/Highload-static-server.git
docker run -p 80:80 -v /etc/httpd.conf:/etc/httpd.conf:ro -v /var/www/html:/var/www/html:ro --name server-httpd -t server-httpd

./httptest.py
```