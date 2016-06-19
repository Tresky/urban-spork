#ifndef MAP_EVENTS_HPP
#define MAP_EVENTS_HPP

namespace rpg_map_mode
{

namespace private_map_mode
{

class MapEvent
{
  friend class EventSupervisor;
public:
  MapEvent(const std::string& _id);

  ~MapEvent() {}

  std::string GetEventId() const
  {
    return id;
  }

  bool IsFinished() const
  {
    return !active;
  }

protected:
  virtual void Launch() = 0;

  virtual bool Update() = 0;

  bool active;

private:
  std::string id;
};

class MapTransitionEvent : public MapEvent
{
public:
  MapTransitionEvent(const std::string& _id,
                     const std::string& _map_name,
                     const std::string& _origin);

  virtual ~MapTransitionEvent() {};

  static MapTransitionEvent* Create(const std::string& _id,
                                    const std::string& _map_name,
                                    const std::string& _origin);

protected:
  void Launch();

  bool Update();

  std::string new_map_name;
  std::string origin;

  bool done;
};

class EventSupervisor
{
  friend class MapEvent;
public:
  EventSupervisor();

  ~EventSupervisor();

  void LaunchEventById(const std::string& _id);

  void LaunchEvent(MapEvent* _event);

  void StopEvent(const std::string& _id) {}

  void Update();

  MapEvent* GetEvent(const std::string& _id) const;

private:
  bool RegisterEvent(MapEvent* _event);

  std::map<std::string, MapEvent*> events;

  std::vector<MapEvent*> active_events;

  bool updating;
};

}

}

#endif // MAP_EVENTS_HPP
