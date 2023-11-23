#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <fusion.pb.h>
#include <iomanip>
#include <iostream>
#include <random>

int count{0};
std::vector<float> data;
pb::FusionData::FusionData fusion_data;

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
#pragma region mutable_ set_
    fusion_data.mutable_endoscope_pos()->set_x(dis_0_10(gen));
    fusion_data.mutable_endoscope_pos()->set_y(dis_0_10(gen));
    fusion_data.mutable_endoscope_pos()->set_z(dis_0_10(gen));

    fusion_data.mutable_endoscope_euler()->set_x(dis_330_360(gen));
    fusion_data.mutable_endoscope_euler()->set_y(dis_10_15(gen));
    fusion_data.mutable_endoscope_euler()->set_z(dis_10_15(gen));

    fusion_data.mutable_tube_pos()->set_x(dis_0_10(gen));
    fusion_data.mutable_tube_pos()->set_y(dis_0_10(gen));
    fusion_data.mutable_tube_pos()->set_z(dis_0_10(gen));

    fusion_data.mutable_tube_euler()->set_x(dis_330_360(gen));
    fusion_data.mutable_tube_euler()->set_y(dis_10_15(gen));
    fusion_data.mutable_tube_euler()->set_x(dis_10_15(gen));

    fusion_data.mutable_offset()->set_endoscope_offset(-1);
    fusion_data.mutable_offset()->set_tube_offset(-3);
    fusion_data.mutable_offset()->set_instrument_switch(60);
    fusion_data.mutable_offset()->set_animation_value(0.5);
    fusion_data.mutable_offset()->set_pivot_offset(2);

    fusion_data.mutable_rot_coord()->set_x(0);
    fusion_data.mutable_rot_coord()->set_y(0.7071068f);
    fusion_data.mutable_rot_coord()->set_z(0);
    fusion_data.mutable_rot_coord()->set_w(0.7071068f);

    fusion_data.mutable_pivot_pos()->set_x(-10);
    fusion_data.mutable_pivot_pos()->set_y(4.9f);
    fusion_data.mutable_pivot_pos()->set_z(-0.9f);

    fusion_data.set_ablation_count(0);

    fusion_data.mutable_haptic()->set_haptic_state(3);
    fusion_data.mutable_haptic()->set_haptic_offset(-1);
    fusion_data.mutable_haptic()->set_haptic_force(2);

    fusion_data.set_hemostasis_count(0);
    fusion_data.set_hemostasis_index(0);

    fusion_data.mutable_soft_tissue()->set_liga_flavum(1);
    fusion_data.mutable_soft_tissue()->set_disc_yellow_space(1);
    fusion_data.mutable_soft_tissue()->set_veutro_vessel(1);
    fusion_data.mutable_soft_tissue()->set_fat(1);
    fusion_data.mutable_soft_tissue()->set_fibrous_rings(1);
    fusion_data.mutable_soft_tissue()->set_nucleus_pulposus(1);
    fusion_data.mutable_soft_tissue()->set_p_longitudinal_liga(1);
    fusion_data.mutable_soft_tissue()->set_dura_mater(1);
    fusion_data.mutable_soft_tissue()->set_nerve_root(1);

    fusion_data.set_nerve_root_dance(0);
#pragma endregion
    const int data_size = fusion_data.ByteSizeLong();// NOLINT(clang-diagnostic-shorten-64-to-32, bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    auto data = std::make_unique<uint8_t[]>(data_size);  // NOLINT(clang-diagnostic-shadow)
    fusion_data.SerializePartialToArray(data.get(), data_size);

    const int code = publisher.Send(data.get(), data_size);// NOLINT(clang-diagnostic-shorten-64-to-32, bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    if (code != data_size) { std::cout << "failure\n"; }

    // eCAL::Process::SleepMS(20);
  }

  eCAL::Finalize();

  return 0;
}