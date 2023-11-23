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

> This conclusion is wrong!
> {style="warning"}


## Issues

All of the following tests are performed when a thread is started to receive data, and the data is read and written without the mutex.

### The subscriber receives binary data and serializes it into protobuf format, resulting in performance degradation.

```C++
static void OnReceive(const char * /*topic_name_*/, 
const struct eCAL::SReceiveCallbackData *data_) {
    pb::FusionData::FusionData fusion_data;
    fusion_data.ParsePartialFromArray(data_->buf, data_->size);
    
    SingletonDataReceiver::getInstance().SetValue(
        fusion_data.endoscope_pos().x(),
        fusion_data.endoscope_pos().y(),
        fusion_data.endoscope_pos().z(),
        ...
    );
}
```
If we do this, the frame time will rise to about **24ms** again.

However, if we do not do `SingletonDataReceiver::getInstance().SetValue`, then the performance will not degrade.

> So I think getting data from protobuf is causing performance degradation.
> {style="warning"}

Then, we continue to test the case where pb is parsed, but the assignment still uses binary.

```C++
static void OnReceive(const char * /*topic_name_*/, const struct eCAL::SReceiveCallbackData *data_) {    
    pb::FusionData::FusionData fusion_data;
    fusion_data.ParsePartialFromArray(data_->buf, data_->size);
    
    void *databuf = data_->buf;
    
    SingletonDataReceiver::getInstance().SetValue(
        static_cast<float *>(databuf)[0],
        static_cast<float *>(databuf)[1],
        static_cast<float *>(databuf)[2],
        static_cast<float *>(databuf)[3],
        ...
        );
}
```
In this case,frame time is **16.67ms**.

### If I serialize protobuf into binary when I publish data, then when the subscriber receives it directly in binary, the data is unstable and jumps.

For example, the pose of the tube and the endoscope jumped dramatically, completely different from the values I set.

The result maybe a data parsing error. For example, SwitchingInstrument:-2147483648 is triggered.

But if I just send and receive in binary, there's no problem.

### Time taken by function: 0 microsecond

```C++
    auto start = std::chrono::high_resolution_clock::now();
    fusion_data.ParsePartialFromArray(data_->buf, data_->size);
    
    SingletonDataReceiver::getInstance().SetValue(
        fusion_data.endoscope_pos().x(),
        fusion_data.endoscope_pos().y(),
        fusion_data.endoscope_pos().z(),
        ...
        );
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
```

If we test the execution time of this part of the code, the result is very small,0 or 1 μs. But it caused an overall delay in the rise.


### If the subscriber here,only part of the pb value is accepted, and the rest is constant. Still 24ms

```C++
SingletonDataReceiver::getInstance().SetValue(
        fusion_data.endoscope_pos().x(),
        fusion_data.endoscope_pos().y(),
        fusion_data.endoscope_pos().z(),
        fusion_data.endoscope_euler().x(),
        fusion_data.endoscope_euler().y(),
        fusion_data.endoscope_euler().z(),
        fusion_data.tube_pos().x(),
        fusion_data.tube_pos().y(),
        fusion_data.tube_pos().z(),
        fusion_data.tube_euler().x(),
        fusion_data.tube_euler().y(),
        fusion_data.tube_euler().z(),
        -1.22898,
        -3.61975,
        60,
        0.9,
        2,
        0,
        0.707106,
        0,
        0.707106,
        -10,
        4.9,
        -0.9,
        ...
        );
```
We found that receiving even 12 pb values would still cause **24ms**, so I reasonably hypothesized that only accessing the pb data structure would cause a delay.

So I will verify that only one pb of data is received, and none at all.

### only one pb of data is received
Okay,okay,In this case,still **24ms**.

```C++
SingletonDataReceiver::getInstance().SetValue(
        fusion_data.endoscope_pos().x(),
        3.000009,
        7.999690,
        338.5304,
        13.23317,
        11.95613,
        ...
        );
```

### none at all,still 24ms!
```C++
SingletonDataReceiver::getInstance().SetValue(
        2,
        3.000009,
        7.999690,
        338.5304,
        13.23317,
        11.95613,
        ...
        );
```
In this case,it's also **24ms**


### If I comment out the SetValue method ,is 16.67ms

Only the statement parsing pb is left.

### If I keep Parse pb and SetValue with data_->buf ,is 16.67ms

```C++
  static void OnReceive(const char * /*topic_name_*/, const struct eCAL::SReceiveCallbackData *data_) {
    pb::FusionData::FusionData fusion_data;
    fusion_data.ParsePartialFromArray(data_->buf, data_->size);
    void *databuf = data_->buf;
    auto start = std::chrono::high_resolution_clock::now();
    
    SingletonDataReceiver::getInstance().SetValue(
        static_cast<float *>(databuf)[0],
        static_cast<float *>(databuf)[1],
        static_cast<float *>(databuf)[2],
        );
}    
```
In this case ,is 16.67ms

### If I add f to all the constants I assign,24ms

```C++
    pb::FusionData::FusionData fusion_data;
    fusion_data.ParsePartialFromArray(data_->buf, data_->size);
    SingletonDataReceiver::getInstance().SetValue(
        2.0f,
        3.000009f,
        7.999690f,
        338.5304f,
        13.23317f,
        ...
        );
```
still 24ms


## 方案

pb,Captain,flatter,对比一下,pb不一定最好
