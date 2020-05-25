#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanObserver.hpp"

namespace griddly {

struct BlockDefinition {
  float color[3];
  std::string shape;
  float scale;
};

struct BlockConfig {
  glm::vec3 color;
  vk::ShapeBuffer shapeBuffer;
  float scale;
};

class BlockObserver : public VulkanObserver {
 public:
  BlockObserver(std::shared_ptr<Grid> grid, uint32_t tileSize, std::unordered_map<std::string, BlockDefinition> blockDefinitions, std::string resourcePath);
  ~BlockObserver();

  void init(uint32_t gridWidth, uint32_t gridHeight) override;

  std::shared_ptr<uint8_t> update(int playerId) const override;
  std::shared_ptr<uint8_t> reset() const override;

 private:
  void render(vk::VulkanRenderContext& ctx) const;

  std::unordered_map<std::string, BlockConfig> blockConfigs_;
  const std::unordered_map<std::string, BlockDefinition> blockDefinitions_;
};

}  // namespace griddly