//
// Created by 22153 on 11/19/2023.
//

#ifndef HAPTIC_H
#define HAPTIC_H
#include <string>
#include <thread>
#include <vector>
#include <HD/hdDefines.h>
#include <HD/hdScheduler.h>

#include "vec3.h"

class Haptic {
public:
  explicit Haptic(const char *name);

  ~Haptic();

  // Securely initializes the Geomagic device and handles errors that may occur during initialization.
  void InitDevice();

  void SetHapticDeviceStatus(int *hapticStatus);

  void Initialize();

  void UpdatePosition();

  void Loop() const;

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
  Vec3 device_offset_base;    // Some dof angular offset of the device base
  Vec3 device_offset_base_c;  // Some Angular offset factor of the device base
  Vec3 device_offset_stylus;  // Some dof angular offset of the stylus
  Vec3 device_offset_stylus_c;// Some Angular offset factor of the stylus;
};

HDerror CatchHdError(bool logError = true);

HDCallbackCode HDCALLBACK StateCallBack(void *userData);

HDCallbackCode HDCALLBACK CopyDeviceDataCallback(void *userData);
#endif //HAPTIC_H
