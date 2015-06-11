
# Benchmark Testing

To test if mod_dart is going to be of any use in the real world we need to start doing some stress testing, so here goes.
The box this is run on is a GCE n1-highcpu-2 (2 vCPU, 1.8 GB memory) instance with out of the box Apache, runing the index.dart
test file.

## Initial tests at 0.1.0

The following ab command simulates 5 concurrent users each doing 10 page hits, this represents a peak load of a 
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

OK, with a faster popen on the same box :-

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
Time taken for tests:   5.949 seconds
Complete requests:      50
Failed requests:        0
Keep-Alive requests:    50
Total transferred:      92160 bytes
HTML transferred:       80300 bytes
Requests per second:    8.41 [#/sec] (mean)
Time per request:       1189.709 [ms] (mean)
Time per request:       118.971 [ms] (mean, across all concurrent requests)
Transfer rate:          15.13 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   34  69.2      0     172
Processing:   590 1103 207.2   1147    1410
Waiting:      588 1101 207.3   1141    1409
Total:        693 1137 225.0   1163    1439

Percentage of the requests served within a certain time (ms)
  50%   1163
  66%   1308
  75%   1346
  80%   1368
  90%   1388
  95%   1410
  98%   1439
  99%   1439
 100%   1439 (longest request)

```

Better requests served, time for request etc. You can see the difference but its not much, not significant,
so lets now up the machine and see what happens :-

Same test on a GCE n1-highcpu-8 (8 vCPU, 7.2 GB memory) machine 

```
ab -l -r -n 50 -c 10 -k -H "Accept-Encoding: gzip, deflate"  http://146.148.122.193/index.dart  
This is ApacheBench, Version 2.3 <$Revision: 1638069 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 146.148.122.193 (be patient).....done


Server Software:        Apache/2.4.6
Server Hostname:        146.148.122.193
Server Port:            80

Document Path:          /index.dart
Document Length:        Variable

Concurrency Level:      10
Time taken for tests:   0.261 seconds
Complete requests:      50
Failed requests:        0
Non-2xx responses:      50
Keep-Alive requests:    50
Total transferred:      21710 bytes
HTML transferred:       10400 bytes
Requests per second:    191.78 [#/sec] (mean)
Time per request:       52.143 [ms] (mean)
Time per request:       5.214 [ms] (mean, across all concurrent requests)
Transfer rate:          81.32 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   14  29.9      0     102
Processing:    22   35  17.1     28      82
Waiting:       22   35  17.1     28      82
Total:         22   49  43.4     28     158

Percentage of the requests served within a certain time (ms)
  50%     28
  66%     30
  75%     34
  80%    102
  90%    133
  95%    151
  98%    158
  99%    158
 100%    158 (longest request)

```
Wow!, so whats the pain point, CPU or memory?

Same test on a GCE n1-highcpu-4 (4 vCPU, 3.6 GB memory)

```
ab -l -r -n 50 -c 10 -k -H "Accept-Encoding: gzip, deflate"  http://146.148.122.193/index.dart
This is ApacheBench, Version 2.3 <$Revision: 1638069 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 146.148.122.193 (be patient).....done


Server Software:        Apache/2.4.6
Server Hostname:        146.148.122.193
Server Port:            80

Document Path:          /index.dart
Document Length:        Variable

Concurrency Level:      10
Time taken for tests:   0.241 seconds
Complete requests:      50
Failed requests:        0
Non-2xx responses:      50
Keep-Alive requests:    50
Total transferred:      21710 bytes
HTML transferred:       10400 bytes
Requests per second:    207.59 [#/sec] (mean)
Time per request:       48.171 [ms] (mean)
Time per request:       4.817 [ms] (mean, across all concurrent requests)
Transfer rate:          88.02 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   11  24.4      0      82
Processing:    22   35  18.7     29      87
Waiting:       22   35  18.7     29      87
Total:         22   47  39.9     29     140

Percentage of the requests served within a certain time (ms)
  50%     29
  66%     30
  75%     30
  80%    117
  90%    121
  95%    127
  98%    140
  99%    140
 100%    140 (longest request)

```

Looks as though my smaller 2 CPU 1.8Gb machine just hasn't got the oomph, I don't think a 
4 vCPU, 3.6 GB memory machine can be taken as excessively powerful these days, so I think
I'll stop here for the mo and use this machine.



