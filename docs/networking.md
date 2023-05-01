
# This document describes communication between bot, operator and c2server

### Control message header (from "c2server" to "bot")

```text
         Control message header structure 
    /- - - - - - - - - - - - - - - - - - - - - -\
    | 3 bytes | 2 bytes |  8 bytes   |  1 byte  |
    |  MAGIC  | H. SIZE | REQUEST ID | MSG TYPE |  <- 14 bytes in total
    | ----------------------------------------- |
    |  Additional info depends on message type  |  <- content and size depends on mgs type
    | ----------------------------------------- |
    |               MODULE PAYLOAD              |  <- size: max transferring - header
    \- - - - - - - - - - - - - - - - - - - - - -/
    
    
       'SINGLE' message additional info
    / --------------------------------- \
    |      1 byte    | up to 255 chars  |
    | MODULE ID SIZE | MODULE ID STRING |
    \ --------------------------------  /
    
       'FIRST' message additional info
    / ----------------------------------------------------- \
    | 2 bytes | 4 bytes |     1 byte     | up to 255 chars  |
    |  PARTS  |  SIZE   | MODULE ID SIZE | MODULE ID STRING |
    \ ----------------------------------------------------  /
    
      'CONTINUATION'/'NEXT' message additional info
    / --------------- \
    |     2 bytes     |
    | SEQUENCE NUMBER |
    \ --------------- /

     'LAST' message does not have additional section 
     
```

#### Control message header contain:

1. "MAGIC BYTES" - sequence of 2 characters (3 bytes):
    - 'RCM' - RAT control message (message will be executed by specified module)
    - 'SIG' - Signalling message (will be handled separately)

    In case if received message from command and control server does not start with 'RCM' or 'SIG', message will be ignored.

2. "HEADER SIZE" - unsigned short int (2 bytes)
3. "REQUEST ID" - unsigned long int (8 bytes)
4. "MESSAGE TYPE" - type of received message (1 byte, used 2 bits + one bit accumulation flag):
   
   | TYPE           | DESCRIPTION AND USAGE                                                                    |
   |----------------|------------------------------------------------------------------------------------------|
   | SINGLE (0/b00) | all payload in one message; task will be passed to execution                             |
   | FIRST (1/b01)  | first part of the task; task will be registered; see accumulation option                 |
   | NEXT (2/b10)   | some part of the task (can be skipped if message was divided on 2 parts: FIRST and LAST) |
   | LAST (3/b11)   | last part of the task; Task will be passed to the execution                              |

5. Additional info that depends on message type:

   - SINGLE: we have to pass module id (to deserialize string its size goes first )
   - FIRST: as 'SINGLE' + full payload size and number of parts. 
   - NEXT: sequence number: value between 1 and 'PARTS'
   - LAST: nothing to add 


```text
   'FIRST' message binary form:
    first 5 bits are '0', next accumulation bit, next 2 bits left to identify type 
    
    0b00000 + (accumulation flag) + (2 bits - message type)
```


