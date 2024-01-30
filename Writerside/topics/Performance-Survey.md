# Performance Survey

## Old version

### Pure Release (no render) 
| server | soft | performance(ms) |
|--------|------|-----------------|
| x      | x    | 1.56            |
| x      | √    | 24.73           |
| √      | x    | 6.32            |
| √      | √    | 35.22           |

**Result**
- Turn on the server will increase about 5 ms 
- Turn on both server and soft body will increase about 10 ms

## Optimize CollisionDetect()

**Unmap the buffers and replace multiple thread with sequential main loop**

### Pure Release + Render

| server | soft | performance(ms) | CollisionDetect() |
|--------|------|-----------------|-------------------|
| x      | x    | 16.7            | NULL              |
| x      | √    | 30.78           | NULL              |
| √      | x    | 16.7            | 0.4               |
| √      | √    | 32.7            | 1.8               |

### Pure Release

| server | soft | performance(ms) | CollisionDetect() |
|--------|------|-----------------|-------------------|
| x      | x    | 2.75            | NULL              |
| x      | √    | 27.87           | NULL              |
| √      | x    | 3.37            | 0.4               |
| √      | √    | 29.54           | 1.25              |

**Conclusion**

When the simulated server is turned on, 
there is a slight increase in consumption due to increased computation.