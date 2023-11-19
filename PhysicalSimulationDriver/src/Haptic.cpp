//
// Created by 22153 on 11/19/2023.
//

#include "Haptic.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <HD/hdDefines.h>
#include <HD/hdDevice.h>
#include <HD/hdScheduler.h>

Haptic::Haptic(const char *name)
  : device_name(std::string("Default")), device_base_pos(Vec3(0, 0, 0)) { InitDevice(); }

Haptic::~Haptic()
= default;

void Haptic::InitDevice() {
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

void Haptic::SetHapticDeviceStatus(int *hapticStatus) {
  haptic_status = hapticStatus;
  if (haptic_status && is_init_done) { *haptic_status = 0; }
}

void Haptic::Initialize() {
  if (is_left_hand_frame) { device_base_pos.x = -device_base_pos.x; }
  UpdatePosition();
  loop_thread = new std::thread(&Haptic::Loop, this);
}

void Haptic::UpdatePosition() {
  if (is_using_DH_method) {
    // TODO:DH method
  } else {
    // TODO:Fixed this problem
    // device_angle_1[0] = simulate_data.angle1[0];
  }
}

void Haptic::Loop() const {
  while (true) {
    if (!is_looping) break;
    // TODO:There has some coupling with sofa scene,analyze how to solve it
  }
}

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

HDCallbackCode HDCALLBACK StateCallBack(void *userData) {}

HDCallbackCode HDCALLBACK CopyDeviceDataCallback(void *userData) {
  const auto driver = static_cast<Haptic *>(userData);
  driver->simulate_data = driver->haptic_data;
  return HD_CALLBACK_CONTINUE;
}

bool Haptic::is_scheduler_running = false;
