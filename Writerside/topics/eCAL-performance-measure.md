# eCAL performance measure

| Transport     | pb  | binary | thread | lock | sub | tick | frame time |  |
|---------------|-----|--------|--------|------|-----|------|------------|--|
| shared memory | no  | no     | no     | no   | no  | yes  | 16.67ms    |  |
| eCAL          | no  | yes    | yes    | yes  | yes | no   | 16.67ms    |  |
| eCAL          | yes | no     | yes    | yes  | yes | no   | 24.3ms     |  |
| eCAL          | yes | no     | no     | yes  | yes | no   | 24.3ms     |  |
| ...           | ... | ...    | ...    | ...  | ... | ...  | ...        |  |

There are so many permutations that I'm not going to go into detail.

However, the **core idea** is that the serialization and antisequence transmission directly through the protobuf of eCAL
itself has very low performance, about 200hz.

If the binary transmission through eCAL, about 400,000-800,000 hz.

The performance of binary transfers using eCAL is about the same as that of native Shared Memory. I haven't tested the
specific minor performance differences in detail, but eCAL might be better in theory.

I'm not sure why eCAL's native protobuf mode of transport is so inefficient.

However, there is no need to explore the end, has been tested on a project-based basis whether to open a separate
thread, whether to rely on the main loop, whether to add mutex, whether to use the subscription publishing mode of these
cases.

Our **conclusion** is that protobuf is serialized first, then converted to binary, and then transmitted to binary via
eCAL. After receiving binary data, the receiver serializes it back to protobuf data, and then reads and accesses it.

> We can change the `memfile_ack_timeout` in the `ecal.ini` file, such as 50 or 100, to ensure that the data frequency
> in pub/sub is the same.
> {style="note"}