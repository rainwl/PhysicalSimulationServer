#include "iostream"
#include "../include/ecal/ecal.h"
#include "../include/ecal/msg/protobuf/publisher.h"
#include "../protobuf/fusion.pb.h"
#include "random"
#include "iomanip"

void PrintAligned(const std::string &key, const auto &value);

void PrintAligned(const std::string &key, const auto &value) { std::cout << std::left << std::setw(20) << key << ": " << value << "\n"; }

void PrintAligned(const std::string &key, const auto &value1, const auto &value2, const auto &value3);

void PrintAligned(const std::string &key, const auto &value1, const auto &value2, const auto &value3) { std::cout << std::left << std::setw(20) << key << ": " << value1 << "," << value2 << "," << value3 << "\n"; }

void PrintAligned(const std::string &key, const auto &v1, const auto &v2, const auto &v3, const auto &v4);

void PrintAligned(const std::string &key, const auto &v1, const auto &v2, const auto &v3, const auto &v4) { std::cout << std::left << std::setw(20) << key << ": " << v1 << "," << v2 << "," << v3 << "," << v4 << "\n"; }

void output(const pb::FusionData::FusionData &fusion_data);

void output(const pb::FusionData::FusionData &fusion_data) {
  PrintAligned("endoscope_pos", fusion_data.endoscope_pos().x(), fusion_data.endoscope_pos().y(),
               fusion_data.endoscope_pos().z());
  PrintAligned("endoscope_euler", fusion_data.endoscope_euler().x(), fusion_data.endoscope_euler().y(),
               fusion_data.endoscope_euler().z());
  PrintAligned("tube_pos", fusion_data.tube_pos().x(), fusion_data.tube_pos().y(), fusion_data.tube_pos().z());
  PrintAligned("tube_euler", fusion_data.tube_euler().x(), fusion_data.tube_euler().y(),
               fusion_data.tube_euler().z());
  PrintAligned("endoscope_offset", fusion_data.offset().endoscope_offset());
  PrintAligned("tube_offset", fusion_data.offset().tube_offset());
  PrintAligned("instrument_switch", fusion_data.offset().instrument_switch());
  PrintAligned("animation_value", fusion_data.offset().animation_value());
  PrintAligned("pivot_offset", fusion_data.offset().pivot_offset());
  PrintAligned("rot_coord", fusion_data.rot_coord().x(), fusion_data.rot_coord().y(),
               fusion_data.rot_coord().z(), fusion_data.rot_coord().w());
  PrintAligned("pivot_pos", fusion_data.pivot_pos().x(), fusion_data.pivot_pos().y(), fusion_data.pivot_pos().z());
  PrintAligned("ablation_count", fusion_data.ablation_count());
  PrintAligned("haptic_state", fusion_data.haptic().haptic_state());
  PrintAligned("haptic_offset", fusion_data.haptic().haptic_offset());
  PrintAligned("haptic_force", fusion_data.haptic().haptic_force());
  PrintAligned("hemostasis_count", fusion_data.hemostasis_count());
  PrintAligned("hemostasis_index", fusion_data.hemostasis_index());
  PrintAligned("liga_flavum", fusion_data.soft_tissue().liga_flavum());
  PrintAligned("disc_yellow_space", fusion_data.soft_tissue().disc_yellow_space());
  PrintAligned("veutro_vessel", fusion_data.soft_tissue().veutro_vessel());
  PrintAligned("fat", fusion_data.soft_tissue().fat());
  PrintAligned("fibrous_rings", fusion_data.soft_tissue().fibrous_rings());
  PrintAligned("nucleus_pulposus", fusion_data.soft_tissue().nucleus_pulposus());
  PrintAligned("p_longitudinal_liga", fusion_data.soft_tissue().p_longitudinal_liga());
  PrintAligned("dura_mater", fusion_data.soft_tissue().dura_mater());
  PrintAligned("nerve_root", fusion_data.soft_tissue().nerve_root());
  PrintAligned("nerve_root_dance", fusion_data.nerve_root_dance());
  std::cout << "\n";
}

int main(const int argc, char **argv) {
#pragma region eCAL init
  eCAL::Initialize(argc, argv, "Fusion Publisher");
  eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, "healthy");
  eCAL::protobuf::CPublisher<pb::FusionData::FusionData> pub("FusionData");
  pb::FusionData::FusionData fusion_data;
#pragma endregion
#pragma region random
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis_0_10(0.0, 10.0);
  std::uniform_real_distribution<float> dis_330_360(330.0, 360.0);
  std::uniform_real_distribution<float> dis_10_15(10.0, 15.0);
#pragma endregion
  while (eCAL::Ok()) {
#pragma region Send Data

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
    pub.Send(fusion_data);
#pragma endregion
    output(fusion_data);
    //eCAL::Process::SleepMS(500);
  }

  eCAL::Finalize();

  return 0;
}
