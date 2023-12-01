# IMGUI

## TODO

| issue                             | todo                   | status |
|-----------------------------------|------------------------|--------|
| InteractiveScene::showAboutWindow | modify to myself info. |        |
| ecal's loop +glfw's loop          |                        |        |
|                                   |                        |        |
|                                   |                        |        |


```C++
// Geometry
if (ImGui::TreeNodeEx("Geometry", ImGuiTreeNodeFlags_DefaultOpen)) {
  // Position
  glm::vec3 value = model->Model::getPosition();
  if (ImGui::DragFloat3("Position", &value.x, 0.01F, 0.0F, 0.0F, "%.4f"))
  {
    model->Model::setPosition(value);
  }
```