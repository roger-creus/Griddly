#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "GridLocation.hpp"

#define BehaviourFunction std::function<BehaviourResult(std::shared_ptr<Action>)>

namespace griddy {

class Grid;
class Action;

struct BehaviourResult {
  bool abortAction = false;
  int32_t reward = 0;
};

class Object : public std::enable_shared_from_this<Object> {
 public:
  virtual GridLocation getLocation() const;

  // playerId of 0 means the object does not belong to any player in particular, (walls etc)
  virtual void init(uint32_t playerId, GridLocation location, std::shared_ptr<Grid> grid_);

  virtual std::string getObjectName() const;

  virtual uint32_t getObjectId() const;

  virtual std::string getDescription() const;

  virtual uint32_t getPlayerId() const;

  virtual bool canPerformAction(std::string actionName) const;

  virtual BehaviourResult onActionSrc(std::shared_ptr<Object> destinationObject, std::shared_ptr<Action> action);

  virtual BehaviourResult onActionDst(std::shared_ptr<Object> sourceObject, std::shared_ptr<Action> action);

  virtual void addActionSrcBehaviour(std::string action, std::string destinationObjectName, std::string commandName, std::vector<std::string> commandParameters, std::unordered_map<std::string, std::vector<std::string>> nestedCommands);

  virtual void addActionDstBehaviour(std::string action, std::string sourceObjectName, std::string commandName, std::vector<std::string> commandParameters, std::unordered_map<std::string, std::vector<std::string>> nestedCommands);

  Object(std::string objectName, uint32_t id, std::unordered_map<std::string, std::shared_ptr<int32_t>> availableParameters);

  ~Object();

 private:
  uint32_t x_;
  uint32_t y_;
  uint32_t playerId_;
  const std::string objectName_;
  const uint32_t id_;

  std::unordered_map<std::string, std::string> actionMap_;


  // action -> destination -> [behaviour functions]
  std::unordered_map<std::string, std::unordered_map<std::string, std::vector<BehaviourFunction>>> srcBehaviours_;
  
  // action -> source -> [behaviour functions]
  std::unordered_map<std::string, std::unordered_map<std::string, std::vector<BehaviourFunction>>> dstBehaviours_;
  
  // The parameters that are available in the object for behaviour commands to interact with
  std::unordered_map<std::string, std::shared_ptr<int32_t>> availableParameters_;

  std::shared_ptr<Grid> grid_;

  virtual void moveObject(GridLocation newLocation);

  virtual void removeObject();

  std::vector<std::shared_ptr<int32_t>> findParameters(std::vector<std::string> parameters);

  BehaviourFunction instantiateBehaviour(std::string commandName, std::vector<std::string> commandParameters);
  BehaviourFunction instantiateConditionalBehaviour(std::string commandName, std::vector<std::string> commandParameters, std::unordered_map<std::string, std::vector<std::string>> subCommands);
};

}  // namespace griddy