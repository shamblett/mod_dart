
# Benchmark Testing

To test if mod_dart is going to be of any use in the real world we need to start doing some stress testing, so here goes.
The box this is run on is a n1-highcpu-2 (2 vCPU, 1.8 GB memory) instance with out of the box Apache, runing the index.dart
test file.

## Initial tests at 0.1.0

The following ab command simulates 5 concurrent users each doing 10 page hits, this This represents a peak load of a 
website that gets about 50,000+ hits a month.

```
ab -l -r -n 50 -c 10 -k -H "Accept-Encoding: gzip, deflate"  http://moddart.no-ip.net/index.dart 
This is ApacheBench, Version 2.3 <$Revision: 1638069 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking moddart.no-ip.net (be patient).....done


Server Software:        Apache/2.4.6
Server Hostname:        moddart.no-ip.net
Server Port:            80

Document Path:          /index.dart
Document Length:        Variable

Concurrency Level:      10
Time taken for tests:   5.964 seconds
Complete requests:      50
Failed requests:        0
Keep-Alive requests:    50
Total transferred:      92160 bytes
HTML transferred:       80300 bytes
Requests per second:    8.38 [#/sec] (mean)
Time per request:       1192.837 [ms] (mean)
Time per request:       119.284 [ms] (mean, across all concurrent requests)
Transfer rate:          15.09 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   10  22.1      0      92
Processing:   609 1148 239.9   1212    1531
Waiting:      607 1145 239.9   1208    1527
Total:        609 1158 248.1   1233    1531

Percentage of the requests served within a certain time (ms)
  50%   1233
  66%   1281
  75%   1306
  80%   1415
  90%   1494
  95%   1507
  98%   1531
  99%   1531
 100%   1531 (longest request)
```

So we are at 8.38 requests per second, wating just over 1.2 secs for a page load, not horrible for a 
first cut but we can do better.


