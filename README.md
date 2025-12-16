# custom-network-protocol

A small project to build and test a custom network protocol on top of TCP using C++.

## Packet Envelope
```text
LSB                                                 MSB
  1 2 3 4 5 6 7 8   1 2 3 4 5 6 7 8   1 2 3 4 5 6 7 8
+ - - - - - - - - + - - - - - - - - + - - - - - - - - +
|     version     |       type      |       len       |
+ - - - - - - - - + - - - - - - - - + - - - - - - - - +
|     data (len byes)                                 |                 
+ - - - - - - - - + - - - - - - - - + - - - - - - - - +
```
## Future Work
Future:
  - Adapt the protocol to a real project or use case
