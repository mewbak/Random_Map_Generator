/*
Copyright © 2013 Igor Paliychuk

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

#include "EventManager.h"
#include "UtilsParsing.h"
#ifndef MAP_GENERATOR
#include "SharedGameResources.h"
#else
#include "Settings.h"
std::map<std::string, std::vector<Event_Component> > EventManager::loot_tables;
#endif
#include "UtilsFileSystem.h"
#include "UtilsMath.h"

/**
 * Class: Event
 */
Event::Event()
	: type("")
	, components(std::vector<Event_Component>())
	, location(Rect())
	, hotspot(Rect())
	, cooldown(0)
	, cooldown_ticks(0)
	, keep_after_trigger(true)
	, center(FPoint(-1, -1))
	, reachable_from(Rect()) {
}

void EventManager::getLootTable(const std::string &filename, std::vector<Event_Component> *ec_list) {
    if (!ec_list)
        return;

    std::map<std::string, std::vector<Event_Component> >::iterator it;
    for (it = loot_tables.begin(); it != loot_tables.end(); ++it) {
        if (it->first == filename) {
            std::vector<Event_Component> *loot_defs = &it->second;
            for (unsigned i=0; i<loot_defs->size(); ++i) {
                ec_list->push_back((*loot_defs)[i]);
            }
            break;
        }
    }
}

void EventManager::parseLoot(FileParser &infile, Event_Component *e, std::vector<Event_Component> *ec_list) {
    if (e == NULL) return;

    std::string chance;
    bool first_is_filename = false;
    e->s = infile.nextValue();

    if (e->s == "currency")
        e->c = CURRENCY_ID;
    else if (toInt(e->s, -1) != -1)
        e->c = toInt(e->s);
    else if (ec_list) {
        // load entire loot table
        std::string filename = e->s;

        // remove the last event component, since getLootTable() will create a new one
        if (e == &ec_list->back())
            ec_list->pop_back();

        getLootTable(filename, ec_list);
        first_is_filename = true;
    }

    if (!first_is_filename) {
        // make sure the type is "loot"
        e->type = EC_LOOT;

        // drop chance
        chance = infile.nextValue();
        if (chance == "fixed") e->z = 0;
        else e->z = toInt(chance);

        // quantity min/max
        e->a = toInt(infile.nextValue());
        clampFloor(e->a, 1);
        e->b = toInt(infile.nextValue());
        clampFloor(e->b, e->a);
    }

    // add repeating loot
    if (ec_list) {
        std::string repeat_val = infile.nextValue();
        while (repeat_val != "") {
            ec_list->push_back(Event_Component());
            Event_Component *ec = &ec_list->back();
            ec->type = EC_LOOT;

            ec->s = repeat_val;
            if (ec->s == "currency")
                ec->c = CURRENCY_ID;
            else if (toInt(ec->s, -1) != -1)
                ec->c = toInt(ec->s);
            else {
                // remove the last event component, since getLootTable() will create a new one
                ec_list->pop_back();

                getLootTable(repeat_val, ec_list);

                repeat_val = infile.nextValue();
                continue;
            }

            chance = infile.nextValue();
            if (chance == "fixed") ec->z = 0;
            else ec->z = toInt(chance);

            ec->a = toInt(infile.nextValue());
            clampFloor(ec->a, 1);
            ec->b = toInt(infile.nextValue());
            clampFloor(ec->b, ec->a);

            repeat_val = infile.nextValue();
        }
    }
}

Event::~Event() {
}

/**
 * returns a pointer to the event component within the components list
 * no need to free the pointer by caller
 * NULL will be returned if no such event is found
 */
Event_Component* Event::getComponent(const EVENT_COMPONENT_TYPE &_type) {
	std::vector<Event_Component>::iterator it;
	for (it = components.begin(); it != components.end(); ++it)
		if (it->type == _type)
			return &(*it);
	return NULL;
}

void Event::deleteAllComponents(const EVENT_COMPONENT_TYPE &_type) {
	std::vector<Event_Component>::iterator it;
	for (it = components.begin(); it != components.end(); ++it)
		if (it->type == _type)
			it = components.erase(it);
}


/**
 * Class: EventManager
 */
EventManager::EventManager() {
}

EventManager::~EventManager() {
}

void EventManager::loadEvent(FileParser &infile, Event* evnt) {
	if (!evnt) return;
	// @CLASS EventManager|Description of events in maps/ and npcs/

	if (infile.key == "type") {
		// @ATTR event.type|[on_trigger:on_mapexit:on_leave:on_load:on_clear]|Type of map event.
		std::string type = infile.val;
		evnt->type = type;

		if      (type == "on_trigger");
		else if (type == "on_mapexit"); // no need to set keep_after_trigger to false correctly, it's ignored anyway
		else if (type == "on_leave");
		else if (type == "on_load") {
			evnt->keep_after_trigger = false;
		}
		else if (type == "on_clear") {
			evnt->keep_after_trigger = false;
		}
		else {
			infile.error("EventManager: Event type '%s' unknown, change to \"on_trigger\" to suppress this warning.", type.c_str());
		}
	}
	else if (infile.key == "location") {
		// @ATTR event.location|[x,y,w,h]|Defines the location area for the event.
		evnt->location.x = toInt(infile.nextValue());
		evnt->location.y = toInt(infile.nextValue());
		evnt->location.w = toInt(infile.nextValue());
		evnt->location.h = toInt(infile.nextValue());

		if (evnt->center.x == -1 && evnt->center.y == -1) {
			evnt->center.x = static_cast<float>(evnt->location.x) + static_cast<float>(evnt->location.w)/2;
			evnt->center.y = static_cast<float>(evnt->location.y) + static_cast<float>(evnt->location.h)/2;
		}
	}
	else if (infile.key == "hotspot") {
		//  @ATTR event.hotspot|[ [x, y, w, h] : location ]|Event uses location as hotspot or defined by rect.
		if (infile.val == "location") {
			evnt->hotspot.x = evnt->location.x;
			evnt->hotspot.y = evnt->location.y;
			evnt->hotspot.w = evnt->location.w;
			evnt->hotspot.h = evnt->location.h;
		}
		else {
			evnt->hotspot.x = toInt(infile.nextValue());
			evnt->hotspot.y = toInt(infile.nextValue());
			evnt->hotspot.w = toInt(infile.nextValue());
			evnt->hotspot.h = toInt(infile.nextValue());
		}

		evnt->center.x = static_cast<float>(evnt->hotspot.x) + static_cast<float>(evnt->hotspot.w)/2;
		evnt->center.y = static_cast<float>(evnt->hotspot.y) + static_cast<float>(evnt->hotspot.h)/2;
	}
	else if (infile.key == "cooldown") {
		// @ATTR event.cooldown|duration|Duration for event cooldown in 'ms' or 's'.
		evnt->cooldown = parse_duration(infile.val);
	}
	else if (infile.key == "reachable_from") {
		// @ATTR event.reachable_from|[x,y,w,h]|If the hero is inside this rectangle, they can activate the event.
		evnt->reachable_from.x = toInt(infile.nextValue());
		evnt->reachable_from.y = toInt(infile.nextValue());
		evnt->reachable_from.w = toInt(infile.nextValue());
		evnt->reachable_from.h = toInt(infile.nextValue());
	}
	else {
		loadEventComponent(infile, evnt, NULL);
	}
}

void EventManager::loadEventComponent(FileParser &infile, Event* evnt, Event_Component* ec) {
	Event_Component *e = NULL;
	if (evnt) {
		evnt->components.push_back(Event_Component());
		e = &evnt->components.back();
	}
	else if (ec) {
		e = ec;
	}

	if (!e) return;

	e->type = EC_NONE;
	if (infile.key == "tooltip") {
		// @ATTR event.tooltip|string|Tooltip for event
		e->type = EC_TOOLTIP;
#ifndef MAP_GENERATOR
		e->s = msg->get(infile.val);
#else
        e->s = infile.val;
#endif
    }
	else if (infile.key == "power_path") {
		// @ATTR event.power_path|[hero:[x,y]]|Event power path
		e->type = EC_POWER_PATH;

		// x,y are src, if s=="hero" we target the hero,
		// else we'll use values in a,b as coordinates
		e->x = toInt(infile.nextValue());
		e->y = toInt(infile.nextValue());

		std::string dest = infile.nextValue();
		if (dest == "hero") {
			e->s = "hero";
		}
		else {
			e->a = toInt(dest);
			e->b = toInt(infile.nextValue());
		}
	}
	else if (infile.key == "power_damage") {
		// @ATTR event.power_damage|min(integer), max(integer)|Range of power damage
		e->type = EC_POWER_DAMAGE;

		e->a = toInt(infile.nextValue());
		e->b = toInt(infile.nextValue());
	}
	else if (infile.key == "intermap") {
		// @ATTR event.intermap|[map(string),x(integer),y(integer)]|Jump to specific map at location specified.
		e->type = EC_INTERMAP;

		e->s = infile.nextValue();
		e->x = toInt(infile.nextValue());
		e->y = toInt(infile.nextValue());
	}
	else if (infile.key == "intramap") {
		// @ATTR event.intramap|[x(integer),y(integer)]|Jump to specific position within current map.
		e->type = EC_INTRAMAP;

		e->x = toInt(infile.nextValue());
		e->y = toInt(infile.nextValue());
	}
	else if (infile.key == "mapmod") {
		// @ATTR event.mapmod|[string,int,int,int],..|Modify map tiles
		e->type = EC_MAPMOD;

		e->s = infile.nextValue();
		e->x = toInt(infile.nextValue());
		e->y = toInt(infile.nextValue());
		e->z = toInt(infile.nextValue());

		// add repeating mapmods
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_MAPMOD;
				e->s = repeat_val;
				e->x = toInt(infile.nextValue());
				e->y = toInt(infile.nextValue());
				e->z = toInt(infile.nextValue());

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "soundfx") {
		// @ATTR event.soundfx|[soundfile(string),x(integer),y(integer),loop(boolean)]|Filename of a sound to play. Optionally, it can be played at a specific location and/or looped.
		e->type = EC_SOUNDFX;

		e->s = infile.nextValue();
		e->x = e->y = -1;
		e->z = static_cast<int>(false);

		std::string s = infile.nextValue();
		if (s != "") e->x = toInt(s);

		s = infile.nextValue();
		if (s != "") e->y = toInt(s);

		s = infile.nextValue();
		if (s != "") e->z = static_cast<int>(toBool(s));
	}
	else if (infile.key == "loot") {
		// @ATTR event.loot|[string,drop_chance([fixed:chance(integer)]),quantity_min(integer),quantity_max(integer)],...|Add loot to the event; either a filename or an inline definition.
		e->type = EC_LOOT;
#ifndef MAP_GENERATOR
		loot->parseLoot(infile, e, &evnt->components);
#endif
	}
	else if (infile.key == "loot_count") {
		// @ATTR event.loot_count|min (integer), max (integer)|Sets the minimum (and optionally, the maximum) amount of loot this event can drop. Overrides the global drop_max setting.
		e->type = EC_LOOT_COUNT;

		e->x = toInt(infile.nextValue());
		e->y = toInt(infile.nextValue());
		if (e->x != 0 || e->y != 0) {
			clampFloor(e->x, 1);
			clampFloor(e->y, e->x);
		}
	}
#ifndef MAP_GENERATOR
	else if (infile.key == "msg") {
		// @ATTR event.msg|string|Adds a message to be displayed for the event.
		e->type = EC_MSG;

		e->s = msg->get(infile.val);
	}
#endif
	else if (infile.key == "shakycam") {
		// @ATTR event.shakycam|duration|Makes the camera shake for this duration in 'ms' or 's'.
		e->type = EC_SHAKYCAM;

		e->x = parse_duration(infile.val);
	}
	else if (infile.key == "requires_status") {
		// @ATTR event.requires_status|string,...|Event requires list of statuses
		e->type = EC_REQUIRES_STATUS;

		e->s = infile.nextValue();

		// add repeating requires_status
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_REQUIRES_STATUS;
				e->s = repeat_val;

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "requires_not_status") {
		// @ATTR event.requires_not_status|string,...|Event requires not list of statuses
		e->type = EC_REQUIRES_NOT_STATUS;

		e->s = infile.nextValue();

		// add repeating requires_not
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_REQUIRES_NOT_STATUS;
				e->s = repeat_val;

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "requires_level") {
		// @ATTR event.requires_level|integer|Event requires hero level
		e->type = EC_REQUIRES_LEVEL;

		e->x = toInt(infile.nextValue());
	}
	else if (infile.key == "requires_not_level") {
		// @ATTR event.requires_not_level|integer|Event requires not hero level
		e->type = EC_REQUIRES_NOT_LEVEL;

		e->x = toInt(infile.nextValue());
	}
	else if (infile.key == "requires_currency") {
		// @ATTR event.requires_currency|integer|Event requires atleast this much currency
		e->type = EC_REQUIRES_CURRENCY;

		e->x = toInt(infile.nextValue());
	}
	else if (infile.key == "requires_not_currency") {
		// @ATTR event.requires_not_currency|integer|Event requires no more than this much currency
		e->type = EC_REQUIRES_NOT_CURRENCY;

		e->x = toInt(infile.nextValue());
	}
	else if (infile.key == "requires_item") {
		// @ATTR event.requires_item|integer,...|Event requires specific item
		e->type = EC_REQUIRES_ITEM;

		e->x = toInt(infile.nextValue());

		// add repeating requires_item
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_REQUIRES_ITEM;
				e->x = toInt(repeat_val);

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "requires_not_item") {
		// @ATTR event.requires_not_item|integer,...|Event requires not having a specific item
		e->type = EC_REQUIRES_NOT_ITEM;

		e->x = toInt(infile.nextValue());

		// add repeating requires_not_item
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_REQUIRES_NOT_ITEM;
				e->x = toInt(repeat_val);

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "requires_class") {
		// @ATTR event.requires_class|string|Event requires this base class
		e->type = EC_REQUIRES_CLASS;

		e->s = infile.nextValue();
	}
	else if (infile.key == "requires_not_class") {
		// @ATTR event.requires_not_class|string|Event requires not this base class
		e->type = EC_REQUIRES_NOT_CLASS;

		e->s = infile.nextValue();
	}
	else if (infile.key == "set_status") {
		// @ATTR event.set_status|string,...|Sets specified statuses
		e->type = EC_SET_STATUS;

		e->s = infile.nextValue();

		// add repeating set_status
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_SET_STATUS;
				e->s = repeat_val;

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "unset_status") {
		// @ATTR event.unset_status|string,...|Unsets specified statuses
		e->type = EC_UNSET_STATUS;

		e->s = infile.nextValue();

		// add repeating unset_status
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_UNSET_STATUS;
				e->s = repeat_val;

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "remove_currency") {
		// @ATTR event.remove_currency|integer|Removes specified amount of currency from hero inventory
		e->type = EC_REMOVE_CURRENCY;

		e->x = toInt(infile.val);
		clampFloor(e->x, 0);
	}
	else if (infile.key == "remove_item") {
		// @ATTR event.remove_item|integer,...|Removes specified item from hero inventory
		e->type = EC_REMOVE_ITEM;

		e->x = toInt(infile.nextValue());

		// add repeating remove_item
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_REMOVE_ITEM;
				e->x = toInt(repeat_val);

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "reward_xp") {
		// @ATTR event.reward_xp|integer|Reward hero with specified amount of experience points.
		e->type = EC_REWARD_XP;

		e->x = toInt(infile.val);
		clampFloor(e->x, 0);
	}
	else if (infile.key == "reward_currency") {
		// @ATTR event.reward_currency|integer|Reward hero with specified amount of currency.
		e->type = EC_REWARD_CURRENCY;

		e->x = toInt(infile.val);
		clampFloor(e->x, 0);
	}
	else if (infile.key == "reward_item") {
		// @ATTR event.reward_item|x(integer),y(integer)|Reward hero with y number of item x.
		e->type = EC_REWARD_ITEM;

		e->x = toInt(infile.nextValue());
		e->y = toInt(infile.val);
		clampFloor(e->y, 0);
	}
	else if (infile.key == "restore") {
		// @ATTR event.restore|string|Restore the hero's HP, MP, and/or status.
		e->type = EC_RESTORE;

		e->s = infile.val;
	}
	else if (infile.key == "power") {
		// @ATTR event.power|power_id|Specify power coupled with event.
		e->type = EC_POWER;

		e->x = toInt(infile.val);
	}
	else if (infile.key == "spawn") {
		// @ATTR event.spawn|[string,x(integer),y(integer)], ...|Spawn an enemy from this category at location
		e->type = EC_SPAWN;

		e->s = infile.nextValue();
		e->x = toInt(infile.nextValue());
		e->y = toInt(infile.nextValue());

		// add repeating spawn
		if (evnt) {
			std::string repeat_val = infile.nextValue();
			while (repeat_val != "") {
				evnt->components.push_back(Event_Component());
				e = &evnt->components.back();
				e->type = EC_SPAWN;

				e->s = repeat_val;
				e->x = toInt(infile.nextValue());
				e->y = toInt(infile.nextValue());

				repeat_val = infile.nextValue();
			}
		}
	}
	else if (infile.key == "stash") {
		// @ATTR event.stash|boolean|Opens the Stash menu.
		e->type = EC_STASH;

		e->s = infile.val;
	}
	else if (infile.key == "npc") {
		// @ATTR event.npc|string|Filename of an NPC to start dialog with.
		e->type = EC_NPC;

		e->s = infile.val;
	}
	else if (infile.key == "music") {
		// @ATTR event.music|string|Change background music to specified file.
		e->type = EC_MUSIC;

		e->s = infile.val;
	}
	else if (infile.key == "cutscene") {
		// @ATTR event.cutscene|string|Show specified cutscene by filename.
		e->type = EC_CUTSCENE;

		e->s = infile.val;
	}
	else if (infile.key == "repeat") {
		// @ATTR event.repeat|boolean|Allows the event to be triggered again.
		e->type = EC_REPEAT;

		e->s = infile.val;
	}
	else if (infile.key == "save_game") {
		// @ATTR event.save_game|boolean|Saves the game when the event is triggered. The respawn position is set to where the player is standing.
		e->type = EC_SAVE_GAME;

		e->s = infile.val;
	}
	else if (infile.key == "book") {
		// @ATTR event.book|string|Opens a book by filename.
		e->type = EC_BOOK;

		e->s = infile.val;
	}
	else {
		infile.error("EventManager: '%s' is not a valid key.", infile.key.c_str());
	}
}

/**
 * A particular event has been triggered.
 * Process all of this events components.
 *
 * @param The triggered event
 * @return Returns true if the event shall not be run again.
 */
bool EventManager::executeEvent(Event &ev) {
	if(&ev == NULL) return false;

	// skip executing events that are on cooldown
	if (ev.cooldown_ticks > 0) return false;

	// set cooldown
	ev.cooldown_ticks = ev.cooldown;

	Event_Component *ec;

#ifndef MAP_GENERATOR
	for (unsigned i = 0; i < ev.components.size(); ++i) {
		ec = &ev.components[i];

		if (ec->type == EC_SET_STATUS) {
			camp->setStatus(ec->s);
		}
		else if (ec->type == EC_UNSET_STATUS) {
			camp->unsetStatus(ec->s);
		}
		else if (ec->type == EC_INTERMAP) {

			if (fileExists(mods->locate(ec->s))) {
				mapr->teleportation = true;
				mapr->teleport_mapname = ec->s;
				mapr->teleport_destination.x = static_cast<float>(ec->x) + 0.5f;
				mapr->teleport_destination.y = static_cast<float>(ec->y) + 0.5f;
			}
			else {
				ev.keep_after_trigger = false;
				mapr->log_msg = msg->get("Unknown destination");
			}
		}
		else if (ec->type == EC_INTRAMAP) {
			mapr->teleportation = true;
			mapr->teleport_mapname = "";
			mapr->teleport_destination.x = static_cast<float>(ec->x) + 0.5f;
			mapr->teleport_destination.y = static_cast<float>(ec->y) + 0.5f;
		}
		else if (ec->type == EC_MAPMOD) {
			if (ec->s == "collision") {
				if (ec->x >= 0 && ec->x < mapr->w && ec->y >= 0 && ec->y < mapr->h) {
					mapr->collider.colmap[ec->x][ec->y] = static_cast<unsigned short>(ec->z);
					mapr->map_change = true;
				}
				else
					logError("EventManager: Mapmod at position (%d, %d) is out of bounds 0-255.", ec->x, ec->y);
			}
			else {
				size_t index = static_cast<size_t>(distance(mapr->layernames.begin(), find(mapr->layernames.begin(), mapr->layernames.end(), ec->s)));
				if (!mapr->isValidTile(ec->z))
					logError("EventManager: Mapmod at position (%d, %d) contains invalid tile id (%d).", ec->x, ec->y, ec->z);
				else if (index >= mapr->layers.size())
					logError("EventManager: Mapmod at position (%d, %d) is on an invalid layer.", ec->x, ec->y);
				else if (ec->x >= 0 && ec->x < mapr->w && ec->y >= 0 && ec->y < mapr->h)
					mapr->layers[index][ec->x][ec->y] = static_cast<unsigned short>(ec->z);
				else
					logError("EventManager: Mapmod at position (%d, %d) is out of bounds 0-255.", ec->x, ec->y);
			}
		}
		else if (ec->type == EC_SOUNDFX) {
			FPoint pos(0,0);
			bool loop = false;

			if (ec->x != -1 && ec->y != -1) {
				if (ec->x != 0 && ec->y != 0) {
					pos.x = static_cast<float>(ec->x) + 0.5f;
					pos.y = static_cast<float>(ec->y) + 0.5f;
				}
			}
			else if (ev.location.x != 0 && ev.location.y != 0) {
				pos.x = static_cast<float>(ev.location.x) + 0.5f;
				pos.y = static_cast<float>(ev.location.y) + 0.5f;
			}

			if (ev.type == "on_load" || static_cast<bool>(ec->z) == true)
				loop = true;

			SoundManager::SoundID sid = snd->load(ec->s, "MapRenderer background soundfx");

			snd->play(sid, GLOBAL_VIRTUAL_CHANNEL, pos, loop);
			mapr->sids.push_back(sid);
		}
		else if (ec->type == EC_LOOT) {
			Event_Component *ec_lootcount = ev.getComponent(EC_LOOT_COUNT);
			if (ec_lootcount) {
				mapr->loot_count.x = ec_lootcount->x;
				mapr->loot_count.y = ec_lootcount->y;
			}
			else {
				mapr->loot_count.x = 0;
				mapr->loot_count.y = 0;
			}

			ec->x = ev.hotspot.x;
			ec->y = ev.hotspot.y;
			mapr->loot.push_back(*ec);
		}
		else if (ec->type == EC_MSG) {
			mapr->log_msg = ec->s;
		}
		else if (ec->type == EC_SHAKYCAM) {
			mapr->shaky_cam_ticks = ec->x;
		}
		else if (ec->type == EC_REMOVE_CURRENCY) {
			camp->removeCurrency(ec->x);
		}
		else if (ec->type == EC_REMOVE_ITEM) {
			camp->removeItem(ec->x);
		}
		else if (ec->type == EC_REWARD_XP) {
			camp->rewardXP(ec->x, true);
		}
		else if (ec->type == EC_REWARD_CURRENCY) {
			camp->rewardCurrency(ec->x);
		}
		else if (ec->type == EC_REWARD_ITEM) {
			ItemStack istack;
			istack.item = ec->x;
			istack.quantity = ec->y;
			camp->rewardItem(istack);
		}
		else if (ec->type == EC_RESTORE) {
			camp->restoreHPMP(ec->s);
		}
		else if (ec->type == EC_SPAWN) {
			Point spawn_pos;
			spawn_pos.x = ec->x;
			spawn_pos.y = ec->y;
			powers->spawn(ec->s, spawn_pos);
		}
		else if (ec->type == EC_POWER) {
			Event_Component *ec_path = ev.getComponent(EC_POWER_PATH);
			FPoint target;

			if (ec_path) {
				// targets hero option
				if (ec_path->s == "hero") {
					target.x = mapr->cam.x;
					target.y = mapr->cam.y;
				}
				// targets fixed path option
				else {
					target.x = static_cast<float>(ec_path->a) + 0.5f;
					target.y = static_cast<float>(ec_path->b) + 0.5f;
				}
			}
			// no path specified, targets self location
			else {
				target.x = static_cast<float>(ev.location.x) + 0.5f;
				target.y = static_cast<float>(ev.location.y) + 0.5f;
			}

			// ec->x is power id
			// ec->y is statblock index
			mapr->activatePower(ec->x, ec->y, target);
		}
		else if (ec->type == EC_STASH) {
			mapr->stash = toBool(ec->s);
			if (mapr->stash) {
				mapr->stash_pos.x = static_cast<float>(ev.location.x) + 0.5f;
				mapr->stash_pos.y = static_cast<float>(ev.location.y) + 0.5f;
			}
		}
		else if (ec->type == EC_NPC) {
			mapr->event_npc = ec->s;
		}
		else if (ec->type == EC_MUSIC) {
			mapr->music_filename = ec->s;
			mapr->loadMusic();
		}
		else if (ec->type == EC_CUTSCENE) {
			mapr->cutscene = true;
			mapr->cutscene_file = ec->s;
		}
		else if (ec->type == EC_REPEAT) {
			ev.keep_after_trigger = toBool(ec->s);
		}
		else if (ec->type == EC_SAVE_GAME) {
			mapr->save_game = toBool(ec->s);
		}
		else if (ec->type == EC_NPC_ID) {
			mapr->npc_id = ec->x;
		}
		else if (ec->type == EC_BOOK) {
			mapr->show_book = ec->s;
		}
	}
#endif
	return !ev.keep_after_trigger;
}


bool EventManager::isActive(const Event &e) {
#ifndef MAP_GENERATOR
	for (unsigned i=0; i < e.components.size(); i++) {
		if (e.components[i].type == EC_REQUIRES_NOT_STATUS) {
			if (camp->checkStatus(e.components[i].s)) {
				return false;
			}
		}
		else if (e.components[i].type == EC_REQUIRES_STATUS) {
			if (!camp->checkStatus(e.components[i].s)) {
				return false;
			}
		}
		else if (e.components[i].type == EC_REQUIRES_CURRENCY) {
			if (!camp->checkCurrency(e.components[i].x)) {
				return false;
			}
		}
		else if (e.components[i].type == EC_REQUIRES_NOT_CURRENCY) {
			if (camp->checkCurrency(e.components[i].x)) {
				return false;
			}
		}
		else if (e.components[i].type == EC_REQUIRES_ITEM) {
			if (!camp->checkItem(e.components[i].x)) {
				return false;
			}
		}
		else if (e.components[i].type == EC_REQUIRES_NOT_ITEM) {
			if (camp->checkItem(e.components[i].x)) {
				return false;
			}
		}
		else if (e.components[i].type == EC_REQUIRES_LEVEL) {
			if (camp->hero->level < e.components[i].x) {
				return false;
			}
		}
		else if (e.components[i].type == EC_REQUIRES_NOT_LEVEL) {
			if (camp->hero->level >= e.components[i].x) {
				return false;
			}
		}
		else if (e.components[i].type == EC_REQUIRES_CLASS) {
			if (camp->hero->character_class != e.components[i].s)
				return false;
		}
		else if (e.components[i].type == EC_REQUIRES_NOT_CLASS) {
			if (camp->hero->character_class == e.components[i].s)
				return false;
		}
	}
#endif
	return true;
}

