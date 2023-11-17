#pragma once

class Geomagic
{
public:
    Geomagic(const char *name);
    ~Geomagic();
    void init_device();
    void set_haptic_device_status();
    
};
