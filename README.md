## Thread

### Reference:
https://www.cplusplus.com/reference/thread/thread/joinable/
https://cloud.tencent.com/developer/article/1584149
https://www.runoob.com/w3cnote/cpp-std-thread.html



### Key word

#### join
The function returns when the thread execution has completed.


#### detached
Detaches the thread represented by the object from the calling thread, allowing them to execute independently from each other.
After a call to this function, the thread object becomes non-joinable and can be destroyed safely.


#### joinable
Returns whether the thread object is joinable.
A thread object is not joinable in any of these cases:
- if it was default-constructed.
- if it has been moved from (either constructing another thread object, or assigning to it).
- if either of its members join or detach has been called.

