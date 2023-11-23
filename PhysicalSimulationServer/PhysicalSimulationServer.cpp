#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <fusion.pb.h>
#include <iomanip>
#include <iostream>
#include <random>

int count{0};
std::vector<float> data;

int main(const int argc, char **argv) {
#pragma region eCAL init
  eCAL::Initialize(argc, argv, "Fusion Publisher");
  eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, "healthy");
  const eCAL::CPublisher publisher("fusion");
#pragma endregion

#pragma region Random
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis_0_10(0.0, 10.0);
  std::uniform_real_distribution<float> dis_330_360(330.0, 360.0);
  std::uniform_real_distribution<float> dis_10_15(10.0, 15.0);
#pragma endregion

  while (eCAL::Ok()) {
    count++;

    data.clear();
    data.push_back(dis_0_10(gen));
    data.push_back(dis_0_10(gen));
    data.push_back(dis_0_10(gen));
    data.push_back(dis_330_360(gen));
    data.push_back(dis_10_15(gen));
    data.push_back(dis_10_15(gen));
    data.push_back(dis_0_10(gen));
    data.push_back(dis_0_10(gen));
    data.push_back(dis_0_10(gen));
    data.push_back(dis_330_360(gen));
    data.push_back(dis_10_15(gen));
    data.push_back(dis_10_15(gen));
    data.push_back(-1);
    data.push_back(-3);
    data.push_back(60);
    data.push_back(0.5);
    data.push_back(2);
    data.push_back(0);
    data.push_back(0.707106f);
    data.push_back(0);
    data.push_back(0.707106f);
    data.push_back(-10);
    data.push_back(4.9f);
    data.push_back(-0.9f);
    data.push_back(0);
    data.push_back(3);
    data.push_back(-1);
    data.push_back(2);
    data.push_back(0);
    data.push_back(0);
    data.push_back(1);
    data.push_back(1);
    data.push_back(1);
    data.push_back(1);
    data.push_back(1);
    data.push_back(1);
    data.push_back(1);
    data.push_back(1);
    data.push_back(1);
    data.push_back(0);

    publisher.Send(data.data(), data.size());

    std::cout << count << "\n";
    // eCAL::Process::SleepMS(20);
  }

  eCAL::Finalize();

  return 0;
}