module;

#include <string>
#include <iostream>
#include "vec3.h"
#include "HD/hd.h"
#include "HD/hdExport.h"
#include "vector"

export module Geomagic;

HDerror CatchHdError(bool logError = true);

HDCallbackCode HDCALLBACK StateCallBack(void *userData);

HDCallbackCode HDCALLBACK CopyDeviceDataCallback(void *userData);

export class Geomagic {
public:
    explicit Geomagic(const char *name);

    ~Geomagic();

    // Securely initializes the Geomagic device and handles errors that may occur during initialization.
    void InitDevice();

    void SetHapticDeviceStatus(int *hapticStatus);

    void Initialize();

    void UpdatePosition();

    void Loop() const;

public:

    std::string device_name;
    Vec3 device_base_pos;
    static bool is_scheduler_running;
    HHD hhd;
    std::vector<HDSchedulerHandle> hd_scheduler_handles;
    struct DeviceData {
        HDdouble angle1[3];
        HDdouble angle2[3];
        HDdouble transform[16];
        int button_state;
    };
    DeviceData haptic_data;
    DeviceData simulate_data;
    double max_input_fore_feedback;// Maximum input force allowed, considering security
    bool is_init_done = false;
    int *haptic_status = nullptr;
    bool is_left_hand_frame = false;
    std::thread *loop_thread = nullptr;
    bool is_looping = true;
    bool is_using_DH_method;// Whether to use DH matrix multiplication method, if not, will directly use API to get pose
    Vec3 device_angle_1;
    Vec3 device_angle_2;
    Vec3 device_offset_base;// Some dof angular offset of the device base
    Vec3 device_offset_base_c;// Some Angular offset factor of the device base
    Vec3 device_offset_stylus;// Some dof angular offset of the stylus
    Vec3 device_offset_stylus_c;// Some Angular offset factor of the stylus;

};

HDerror CatchHdError(bool logError) {
    HDErrorInfo error;
    if (HD_DEVICE_ERROR(error = hdGetError())) {
        if (logError) {
            std::cout << "HDError:" << error.hHD << "returns error code" << error.errorCode << ":"
                      << hdGetErrorString(error.errorCode) << "\n";
        }
        return error.errorCode;
    }
    return HD_SUCCESS;
}

HDCallbackCode HDCALLBACK StateCallBack(void *userData) {

}

HDCallbackCode HDCALLBACK CopyDeviceDataCallback(void *userData) {
    const auto driver = static_cast<Geomagic *>(userData);
    driver->simulate_data = driver->haptic_data;
    return HD_CALLBACK_CONTINUE;
}

bool Geomagic::is_scheduler_running = false;

Geomagic::Geomagic(const char *name) : device_name(std::string("Default")), device_base_pos(Vec3(0, 0, 0)) {
    InitDevice();
}

void Geomagic::InitDevice() {
    HDSchedulerHandle hdSchedulerHandle = HD_INVALID_HANDLE;

    // Stop Scheduler
    if (is_scheduler_running) {
        hdStopScheduler();
        is_scheduler_running = false;
    }

    // Init Device
    hhd = hdInitDevice(device_name.c_str());

    // Catch init error
    if (CatchHdError(false) == HD_DEVICE_ALREADY_INITIATED) {
        std::cout << "The device has been initialized, the driver is cleared, and the device is restarted\n";
        // loop 10 times,to see whether restart successful
        HDerror tempError = HD_DEVICE_ALREADY_INITIATED;
        int securityLoop = 0;
        while (tempError == HD_DEVICE_ALREADY_INITIATED && securityLoop < 10) {
            //release device
            hhd = hdGetCurrentDevice();
            if (hhd != HD_INVALID_HANDLE) {
                hdDisableDevice(hhd);
                hhd = HD_INVALID_HANDLE;
            }
            //init device
            hhd = hdInitDevice(device_name.c_str());
            tempError = CatchHdError(false);
            securityLoop++;
        }
        if (tempError != HD_SUCCESS) {
            hhd = HD_INVALID_HANDLE;
            return;
        }

    }

    // Check the calibration of haptic
    if (hdCheckCalibration() != HD_CALIBRATION_OK) {
        std::cout << "GeomagicDriver initialization failed,because device [" << device_name
                  << "] has not been calibrated,Must be calibrated before simulation starts\n";
        return;
    }

    // Open the scheduler
    hdEnable(HD_FORCE_OUTPUT);
    hdEnable(HD_MAX_FORCE_CLAMPING);
    hdEnable(HD_SOFTWARE_FORCE_LIMIT);

    // Open scheduler
    hdStartScheduler();
    is_scheduler_running = true;
    if (CatchHdError()) {
        std::cout << "Failed to start the scheduler.\n";
        return;
    }

    // Add Task for callback
    hdSchedulerHandle = hdScheduleAsynchronous(StateCallBack, this, HD_MAX_SCHEDULER_PRIORITY);
    hd_scheduler_handles.push_back(hdSchedulerHandle);

    hdSchedulerHandle = hdScheduleAsynchronous(CopyDeviceDataCallback, this, HD_MIN_SCHEDULER_PRIORITY);
    hd_scheduler_handles.push_back(hdSchedulerHandle);

    if (CatchHdError()) {
        std::cout << "Failed to set the scheduler task.\n";
        return;
    }

    if (max_input_fore_feedback <= 0.0) {
        std::cout << "max_input_fore_feedback: " << max_input_fore_feedback << " must be > 0\n";
        max_input_fore_feedback = 0.0;
    }


    // It will take a few microseconds for the scheduler to turn on better
    // and retrieve the correct device information before update()
    std::this_thread::sleep_for(std::chrono::milliseconds(42));
    is_init_done = true;
}

Geomagic::~Geomagic() {

}

void Geomagic::SetHapticDeviceStatus(int *hapticStatus) {
    haptic_status = hapticStatus;
    if (haptic_status && is_init_done) {
        *haptic_status = 0;
    }
}

void Geomagic::Initialize() {
    if (is_left_hand_frame) {
        device_base_pos.x = -device_base_pos.x;
    }
    UpdatePosition();
    loop_thread = new std::thread(&Geomagic::Loop, this);
}

void Geomagic::UpdatePosition() {
    if (is_using_DH_method) {
        // TODO:DH method
    } else {
        // TODO:Fixed this problem
        // device_angle_1[0] = simulate_data.angle1[0];

    }
}

void Geomagic::Loop() const {
    while (true) {
        if (!is_looping)break;
        // TODO:There has some coupling with sofa scene,analyze how to solve it
    }
};
