/*

# Starting Guild #

### Description ###
------------------------------------------------------------------------------------------------------------------
This module auto-assigns new players to specific guilds.

- New characters are added to a specified guild on first login.
- Config: Set ID for specific Horde and Alliance guilds


### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: Player/Server
- Script: StartGuild
- Config: Yes
- SQL: Yes


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2017.08.01
- v2017.08.02 - Fix crash prevention when no guilds exist



### Credits ###
------------------------------------------------------------------------------------------------------------------
#### A module for AzerothCore by StygianTheBest ([stygianthebest.github.io](http://stygianthebest.github.io)) ####

###### Additional Credits include:
- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [AzerothCore Discord](https://discord.gg/gkt4y2x)
- [EMUDevs](https://youtube.com/user/EmuDevs)
- [AC-Web](http://ac-web.org/)
- [ModCraft.io](http://modcraft.io/)
- [OwnedCore](http://ownedcore.com/)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [AoWoW](https://wotlk.evowow.com/)


### License ###
------------------------------------------------------------------------------------------------------------------
- This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/


#include "ScriptMgr.h"
#include "Player.h"
#include "GuildMgr.h"
#include "Config.h"
#include "Chat.h"

#define Welcome_Name "Notice"

bool StartGuildEnable = 1;
bool StartGuildAnnounceModule = 1;
uint32 HordeGuild = 1;
uint32 AllianceGuild = 2;

class StartGuildConfig : public WorldScript
{
public:
    StartGuildConfig() : WorldScript("StartGuildConfig_conf") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/mod_startguild.conf";
#ifdef WIN32
            cfg_file = "mod_startguild.conf";
#endif
            std::string cfg_def_file = cfg_file + ".dist";
            sConfigMgr->LoadMore(cfg_def_file.c_str());
            sConfigMgr->LoadMore(cfg_file.c_str());

            StartGuildEnable = sConfigMgr->GetBoolDefault("StartGuild.Enable", 1);
            StartGuildAnnounceModule = sConfigMgr->GetBoolDefault("StartGuild.Announce", 1);
            HordeGuild = sConfigMgr->GetIntDefault("StartGuild.Horde", 1);
            AllianceGuild = sConfigMgr->GetIntDefault("StartGuild.Alliance", 2);
        }
    }
};

class StartGuildAnnounce : public PlayerScript
{

public:

    StartGuildAnnounce() : PlayerScript("StartGuildAnnounce") {}

    void OnLogin(Player* player)
    {
        // Announce Module
        if (StartGuildEnable = true)
        {
            if (StartGuildAnnounceModule = true)
            {
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00StartGuild |rmodule.");
            }
        }
    }
};

class StartGuild : public PlayerScript
{

public:
	StartGuild() : PlayerScript("StartGuild") { }

	// Start Guild
	void OnFirstLogin(Player* player)
	{
		// If enabled...
		if (StartGuildEnable = true)
		{
            const uint32 GUILD_ID_ALLIANCE = AllianceGuild; //sConfigMgr->GetIntDefault("StartGuild.Alliance", 0);
            const uint32 GUILD_ID_HORDE = HordeGuild; //sConfigMgr->GetIntDefault("StartGuild.Horde", 0);

			Guild* guild = sGuildMgr->GetGuildById(player->GetTeamId() == TEAM_ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE);

			// If a guild is present, assign the character to the guild; otherwise skip assignment.
			if (guild)
			{
				guild->AddMember(player->GetGUID());

				// Inform the player they have joined the guild	
				std::ostringstream ss;
				ss << "Welcome to the " << player->GetGuildName() << " guild " << player->GetName() << "!";
				ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
			}
		}
	}
};

void AddStartGuildScripts()
{
    new StartGuildConfig();
    new StartGuildAnnounce();
    new StartGuild();
}
