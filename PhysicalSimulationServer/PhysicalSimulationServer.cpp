#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <fusion.pb.h>
#include <iomanip>
#include <iostream>
#include <random>

void output(const pb::FusionData::FusionData &fusion_data);

void output(const pb::FusionData::FusionData &fusion_data) {
  std::cout << std::left << std::setw(20) << "endoscope_pos"
      << ": " << fusion_data.endoscope_pos().x() << ",";
  std::cout << fusion_data.endoscope_pos().y() << ",";
  std::cout << fusion_data.endoscope_pos().z() << "\n";
  std::cout << std::left << std::setw(20) << "endoscope_euler"
      << ": " << fusion_data.endoscope_euler().x() << ",";
  std::cout << fusion_data.endoscope_euler().y() << ",";
  std::cout << fusion_data.endoscope_euler().z() << "\n";
  std::cout << std::left << std::setw(20) << "tube_pos"
      << ": " << fusion_data.tube_pos().x() << ",";
  std::cout << fusion_data.tube_pos().y() << ",";
  std::cout << fusion_data.tube_pos().z() << "\n";
  std::cout << std::left << std::setw(20) << "tube_euler"
      << ": " << fusion_data.tube_euler().x() << ",";
  std::cout << fusion_data.tube_euler().y() << ",";
  std::cout << fusion_data.tube_euler().z() << "\n";
  std::cout << std::left << std::setw(20) << "endoscope_offset"
      << ": " << fusion_data.offset().endoscope_offset() << "\n";
  std::cout << std::left << std::setw(20) << "tube_offset"
      << ": " << fusion_data.offset().tube_offset() << "\n";
  std::cout << std::left << std::setw(20) << "instrument_switch"
      << ": " << fusion_data.offset().instrument_switch() << "\n";
  std::cout << std::left << std::setw(20) << "animation_value"
      << ": " << fusion_data.offset().animation_value() << "\n";
  std::cout << std::left << std::setw(20) << "pivot_offset"
      << ": " << fusion_data.offset().pivot_offset() << "\n";
  std::cout << std::left << std::setw(20) << "rot_coordinate"
      << ": " << fusion_data.rot_coord().x() << ",";
  std::cout << fusion_data.rot_coord().y() << ",";
  std::cout << fusion_data.rot_coord().z() << ",";
  std::cout << fusion_data.rot_coord().w() << "\n";
  std::cout << std::left << std::setw(20) << "pivot_pos"
      << ": " << fusion_data.pivot_pos().x() << ",";
  std::cout << fusion_data.pivot_pos().y() << ",";
  std::cout << fusion_data.pivot_pos().z() << "\n";
  std::cout << std::left << std::setw(20) << "ablation_count"
      << ": " << fusion_data.ablation_count() << "\n";
  std::cout << std::left << std::setw(20) << "haptic_state"
      << ": " << fusion_data.haptic().haptic_state() << "\n";
  std::cout << std::left << std::setw(20) << "haptic_offset"
      << ": " << fusion_data.haptic().haptic_offset() << "\n";
  std::cout << std::left << std::setw(20) << "haptic_force"
      << ": " << fusion_data.haptic().haptic_force() << "\n";
  std::cout << std::left << std::setw(20) << "hemostasis_count"
      << ": " << fusion_data.hemostasis_count() << "\n";
  std::cout << std::left << std::setw(20) << "hemostasis_index"
      << ": " << fusion_data.hemostasis_index() << "\n";
  std::cout << std::left << std::setw(20) << "liga_flavum"
      << ": " << fusion_data.soft_tissue().liga_flavum() << "\n";
  std::cout << std::left << std::setw(20) << "disc_yellow_space"
      << ": " << fusion_data.soft_tissue().disc_yellow_space() << "\n";
  std::cout << std::left << std::setw(20) << "veutro_vessel"
      << ": " << fusion_data.soft_tissue().veutro_vessel() << "\n";
  std::cout << std::left << std::setw(20) << "fat"
      << ": " << fusion_data.soft_tissue().fat() << "\n";
  std::cout << std::left << std::setw(20) << "fibrous_rings"
      << ": " << fusion_data.soft_tissue().fibrous_rings() << "\n";
  std::cout << std::left << std::setw(20) << "nucleus_pulposus"
      << ": " << fusion_data.soft_tissue().nucleus_pulposus() << "\n";
  std::cout << std::left << std::setw(20) << "p_longitudinal_liga"
      << ": " << fusion_data.soft_tissue().p_longitudinal_liga() << "\n";
  std::cout << std::left << std::setw(20) << "dura_mater"
      << ": " << fusion_data.soft_tissue().dura_mater() << "\n";
  std::cout << std::left << std::setw(20) << "nerve_root"
      << ": " << fusion_data.soft_tissue().nerve_root() << "\n";
  std::cout << std::left << std::setw(20) << "nerve_root_dance"
      << ": " << fusion_data.nerve_root_dance() << "\n";
  std::cout << "\n";
}

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
  //pb::FusionData::FusionData fusion_data;

  while (eCAL::Ok()) {
    count++;
    //   fusion_data.mutable_endoscope_pos()->set_x(dis_0_10(gen));
    //   fusion_data.mutable_endoscope_pos()->set_y(dis_0_10(gen));
    //   fusion_data.mutable_endoscope_pos()->set_z(dis_0_10(gen));
    //   
    //   fusion_data.mutable_endoscope_euler()->set_x(dis_330_360(gen));
    //   fusion_data.mutable_endoscope_euler()->set_y(dis_10_15(gen));
    //   fusion_data.mutable_endoscope_euler()->set_z(dis_10_15(gen));
    //   
    //   fusion_data.mutable_tube_pos()->set_x(dis_0_10(gen));
    //   fusion_data.mutable_tube_pos()->set_y(dis_0_10(gen));
    //   fusion_data.mutable_tube_pos()->set_z(dis_0_10(gen));
    //   
    //   fusion_data.mutable_tube_euler()->set_x(dis_330_360(gen));
    //   fusion_data.mutable_tube_euler()->set_y(dis_10_15(gen));
    //   fusion_data.mutable_tube_euler()->set_x(dis_10_15(gen));
    //   
    //   fusion_data.mutable_offset()->set_endoscope_offset(-1);
    //   fusion_data.mutable_offset()->set_tube_offset(-3);
    //   fusion_data.mutable_offset()->set_instrument_switch(60);
    //   fusion_data.mutable_offset()->set_animation_value(0.5);
    //   fusion_data.mutable_offset()->set_pivot_offset(2);
    //   
    //   fusion_data.mutable_rot_coord()->set_x(0);
    //   fusion_data.mutable_rot_coord()->set_y(0.7071068f);
    //   fusion_data.mutable_rot_coord()->set_z(0);
    //   fusion_data.mutable_rot_coord()->set_w(0.7071068f);
    //   
    //   fusion_data.mutable_pivot_pos()->set_x(-10);
    //   fusion_data.mutable_pivot_pos()->set_y(4.9f);
    //   fusion_data.mutable_pivot_pos()->set_z(-0.9f);
    //   
    //   fusion_data.set_ablation_count(0);
    //   
    //   fusion_data.mutable_haptic()->set_haptic_state(3);
    //   fusion_data.mutable_haptic()->set_haptic_offset(-1);
    //   fusion_data.mutable_haptic()->set_haptic_force(2);
    //   
    //   fusion_data.set_hemostasis_count(0);
    //   fusion_data.set_hemostasis_index(0);
    //   
    //   fusion_data.mutable_soft_tissue()->set_liga_flavum(1);
    //   fusion_data.mutable_soft_tissue()->set_disc_yellow_space(1);
    //   fusion_data.mutable_soft_tissue()->set_veutro_vessel(1);
    //   fusion_data.mutable_soft_tissue()->set_fat(1);
    //   fusion_data.mutable_soft_tissue()->set_fibrous_rings(1);
    //   fusion_data.mutable_soft_tissue()->set_nucleus_pulposus(1);
    //   fusion_data.mutable_soft_tissue()->set_p_longitudinal_liga(1);
    //   fusion_data.mutable_soft_tissue()->set_dura_mater(1);
    //   fusion_data.mutable_soft_tissue()->set_nerve_root(1);
    //   
    //   fusion_data.set_nerve_root_dance(0);
    //   
    //   const int data_size = fusion_data.ByteSizeLong();// NOLINT(clang-diagnostic-shorten-64-to-32, bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    //   auto data = std::make_unique<uint8_t[]>(data_size);
    //   fusion_data.SerializePartialToArray(data.get(), data_size);
    //   
    //   const int code = publisher.Send(data.get(), data_size);// NOLINT(clang-diagnostic-shorten-64-to-32, bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    //   if (code != data_size) { std::cout << "failure\n"; }
    //   std::cout << count << std::endl;

    // #pragma region Send Data
    //
    //     fusion_data.mutable_endoscope_pos()->set_x(dis_0_10(gen));
    //     fusion_data.mutable_endoscope_pos()->set_y(dis_0_10(gen));
    //     fusion_data.mutable_endoscope_pos()->set_z(dis_0_10(gen));
    //
    //     fusion_data.mutable_endoscope_euler()->set_x(dis_330_360(gen));
    //     fusion_data.mutable_endoscope_euler()->set_y(dis_10_15(gen));
    //     fusion_data.mutable_endoscope_euler()->set_z(dis_10_15(gen));
    //
    //     fusion_data.mutable_tube_pos()->set_x(dis_0_10(gen));
    //     fusion_data.mutable_tube_pos()->set_y(dis_0_10(gen));
    //     fusion_data.mutable_tube_pos()->set_z(dis_0_10(gen));
    //
    //     fusion_data.mutable_tube_euler()->set_x(dis_330_360(gen));
    //     fusion_data.mutable_tube_euler()->set_y(dis_10_15(gen));
    //     fusion_data.mutable_tube_euler()->set_x(dis_10_15(gen));
    //
    //     fusion_data.mutable_offset()->set_endoscope_offset(-1);
    //     fusion_data.mutable_offset()->set_tube_offset(-3);
    //     fusion_data.mutable_offset()->set_instrument_switch(60);
    //     fusion_data.mutable_offset()->set_animation_value(0.5);
    //     fusion_data.mutable_offset()->set_pivot_offset(2);
    //
    //     fusion_data.mutable_rot_coord()->set_x(0);
    //     fusion_data.mutable_rot_coord()->set_y(0.7071068f);
    //     fusion_data.mutable_rot_coord()->set_z(0);
    //     fusion_data.mutable_rot_coord()->set_w(0.7071068f);
    //
    //     fusion_data.mutable_pivot_pos()->set_x(-10);
    //     fusion_data.mutable_pivot_pos()->set_y(4.9f);
    //     fusion_data.mutable_pivot_pos()->set_z(-0.9f);
    //
    //     fusion_data.set_ablation_count(0);
    //
    //     fusion_data.mutable_haptic()->set_haptic_state(3);
    //     fusion_data.mutable_haptic()->set_haptic_offset(-1);
    //     fusion_data.mutable_haptic()->set_haptic_force(2);
    //
    //     fusion_data.set_hemostasis_count(0);
    //     fusion_data.set_hemostasis_index(0);
    //
    //     fusion_data.mutable_soft_tissue()->set_liga_flavum(1);
    //     fusion_data.mutable_soft_tissue()->set_disc_yellow_space(1);
    //     fusion_data.mutable_soft_tissue()->set_veutro_vessel(1);
    //     fusion_data.mutable_soft_tissue()->set_fat(1);
    //     fusion_data.mutable_soft_tissue()->set_fibrous_rings(1);
    //     fusion_data.mutable_soft_tissue()->set_nucleus_pulposus(1);
    //     fusion_data.mutable_soft_tissue()->set_p_longitudinal_liga(1);
    //     fusion_data.mutable_soft_tissue()->set_dura_mater(1);
    //     fusion_data.mutable_soft_tissue()->set_nerve_root(1);
    //
    //     fusion_data.set_nerve_root_dance(0);
    //     pub.Send(fusion_data);
    // #pragma endregion
    //
    //     output(fusion_data);

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
