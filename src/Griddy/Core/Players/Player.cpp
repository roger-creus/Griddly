#include "Player.hpp"

#include <spdlog/spdlog.h>

#include "../GameProcess.hpp"

namespace griddy {

Player::Player(uint32_t id, std::string name, std::shared_ptr<Observer> observer)
    : id_(id), name_(name), observer_(observer) {
  score_ = std::make_shared<int32_t>(0);
}

Player::~Player() {}

std::string Player::getName() const {
  return name_;
}

uint32_t Player::getId() const {
  return id_;
}

std::shared_ptr<int32_t> Player::getScore() const {
  return score_;
}

void Player::init(uint32_t gridWidth, uint32_t gridHeight, std::shared_ptr<GameProcess> gameProcess) {
  spdlog::debug("Initializing player: {0}, name: {1}", id_, name_);
  if (observer_ != nullptr) {
    observer_->init(gridWidth, gridHeight);
  }
  this->gameProcess_ = gameProcess;

  *score_ = 0;
}

void Player::reset() {
  if (observer_ != nullptr) {
    observer_->reset();
  }

  *score_ = 0;
}

void Player::setAvatar(std::shared_ptr<Object> avatarObject) {
  avatar_ = avatarObject;
}

std::shared_ptr<Object> Player::getAvatar() {
  return avatar_;
}

std::shared_ptr<GameProcess> Player::getGameProcess() const {
  return gameProcess_;
}

std::shared_ptr<Observer> Player::getObserver() const {
  return observer_;
}

std::vector<int> Player::performActions(std::vector<std::shared_ptr<Action>> actions) {
  auto rewards = gameProcess_->performActions(id_, actions);

  // Update the player's score
  for (auto r : rewards) {
    *score_ += r;
  }

  return rewards;
}

std::unique_ptr<uint8_t[]> Player::observe() {
  if (observer_ == nullptr) {
    return nullptr;
  }
  return observer_->update(id_);
}

}  // namespace griddy