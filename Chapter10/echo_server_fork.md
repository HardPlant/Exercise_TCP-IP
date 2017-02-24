#echo_server_fork
###Multi Process Server
Pros:
* Simple
  call fork() at right position
  easy to catch errors
* Safety
  indepenent child, unaffect to parent

Cons:
* Performance
  Copy entire codes, very expensive method
* IPC problem
  Not easy to exchange data

Mosy frenquently used, because it
* can develop fast
* tested in a long time
