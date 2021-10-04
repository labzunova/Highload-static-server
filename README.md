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

## Testing  

### Func tests result:  

```asm
Ran 24 tests in 0.016s

OK
```
### Load test result:  

- ab -n 10000 -c 10 127.0.0.1:8200/httptest/wikipedia_russia.htm
```asm
This is ApacheBench, Version 2.3 <$Revision: 1843412 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 127.0.0.1 (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        highload_static_server
Server Hostname:        127.0.0.1
Server Port:            8100

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954825 bytes

Concurrency Level:      10
Time taken for tests:   6.390 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9549750000 bytes
HTML transferred:       9548250000 bytes
Requests per second:    1564.88 [#/sec] (mean)
Time per request:       6.390 [ms] (mean)
Time per request:       0.639 [ms] (mean, across all concurrent requests)
Transfer rate:          1459398.60 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       3
Processing:     1    6   2.5      6      24
Waiting:        0    2   1.1      2      18
Total:          2    6   2.5      6      24

Percentage of the requests served within a certain time (ms)
  50%      6
  66%      6
  75%      7
  80%      8
  90%     10
  95%     12
  98%     13
  99%     14
 100%     24 (longest request)

```

### Nginx:  

- ab -n 10000 -c 8 127.0.0.1/httptest/wikipedia_russia.html
```asm
This is ApacheBench, Version 2.3 <$Revision: 1843412 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 127.0.0.1 (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        nginx/1.18.0
Server Hostname:        127.0.0.1
Server Port:            80

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954825 bytes

Concurrency Level:      10
Time taken for tests:   8.208 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9549800000 bytes
HTML transferred:       9548250000 bytes
Requests per second:    1218.35 [#/sec] (mean)
Time per request:       8.208 [ms] (mean)
Time per request:       0.821 [ms] (mean, across all concurrent requests)
Transfer rate:          1136234.10 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       6
Processing:     1    8   4.1      7      40
Waiting:        1    4   2.0      3      34
Total:          1    8   4.1      7      40

Percentage of the requests served within a certain time (ms)
  50%      7
  66%      9
  75%     10
  80%     11
  90%     14
  95%     16
  98%     20
  99%     22
 100%     40 (longest request)
```
