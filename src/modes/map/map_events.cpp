#include "../../utils/globals.hpp"

#include "../../core/video_manager.hpp"

#include "map_mode.hpp"
#include "map_events.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

MapEvent::MapEvent(const std::string& _id)
  : id(_id)
  , active(false)
{
  MapMode* mm = MapMode::CurrentInstance();
  if (!mm)
  {
    PRINT_ERROR << "No MapMode to register event with." << endl;
    return;
  }

  EventSupervisor* events = mm->GetEventSupervisor();
  if (!events)
  {
    PRINT_ERROR << "No EventSupervisor to register event with." << endl;
    return;
  }

  if (!events->RegisterEvent(this))
  {
    PRINT_ERROR << "Failed to register event: " << _id << endl;
    return;
  }
}





MapTransitionEvent::MapTransitionEvent(const std::string& _id,
                                       const std::string& _map_name,
                                       const std::string& _origin)
 : MapEvent(_id)
 , new_map_name(_map_name)
 , origin(_origin)
{}

MapTransitionEvent* MapTransitionEvent::Create(const std::string& _id,
                                               const std::string& _map_name,
                                               const std::string& _origin)
{
    return new MapTransitionEvent(_id, _map_name, _origin);
}

void MapTransitionEvent::Launch()
{
  rpg_video::VideoManager->StartTransitionFadeOut(sf::Color::Black, 2000);
  active = true;
}

void MapTransitionEvent::Update()
{
    if (rpg_video::VideoManager->IsFading())
      return;

    if (!IsFinished())
    {
      // rpg_global::GlobalManager->SetPreviousLocation(origin);
      MapMode* mode = new MapMode(new_map_name);
      rpg_mode_manager::ModeManager->Pop();
      rpg_mode_manager::ModeManager->Push(mode, false, true);
      active = false;
    }
}





EventSupervisor::EventSupervisor()
  : updating(false)
{}

EventSupervisor::~EventSupervisor()
{
  // Need to delete all MapEvent*
}

void EventSupervisor::LaunchEvent(const std::string& _id)
{
  MapEvent* event = GetEvent(_id);
  if (!event)
    PRINT_WARNING << "Event not found: " << _id << endl;
  else
    LaunchEvent(event);
}

void EventSupervisor::LaunchEvent(MapEvent* _event)
{
  if (!_event)
  {
    PRINT_WARNING << "Event object null" << endl;
    return;
  }
  if (!updating && _event->IsFinished())
  {
    active_events.push_back(_event);
    _event->Launch();
  }
}

void EventSupervisor::Update()
{
  updating = true;

  std::vector<MapEvent*>::iterator it;
  for (it = active_events.begin(); it != active_events.end();)
  {
    (*it)->Update();
    if ((*it)->IsFinished())
      it = active_events.erase(it);
    else
      ++it;
  }

  updating = false;
}

MapEvent* EventSupervisor::GetEvent(const std::string& _id) const
{
  if (events.find(_id) == events.end())
    return nullptr;
  return events.at(_id);
}

bool EventSupervisor::RegisterEvent(MapEvent* _event)
{
  if (!_event)
  {
    PRINT_WARNING << "Event object null" << endl;
    return false;
  }

  if (GetEvent(_event->id))
  {
    PRINT_WARNING << "Attempting to register event that exists: " << _event->id << endl;
    return false;
  }

  events[_event->id] = _event;
  return true;
}

}

}
