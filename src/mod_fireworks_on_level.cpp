/*
 * Fireworks on Level:
 * - Announces the module on login (configurable).
 * - Casts a celebratory firework on specific level-ups.
 */

#include "Chat.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"

#include <array>

namespace
{
    constexpr auto CONF_ENABLE = "FireworksOnLevel.Enable";
    constexpr auto CONF_ANNOUNCE = "FireworksOnLevel.Announce";

    constexpr std::array<uint32, 10> FIREWORK_SPELLS{
        11540, // Blue Firework
        11541, // Green Firework
        11542, // Streamers
        11543, // USA
        11544, // Yellow Rose Firework
        25465, // Ground
        30161, // Purple Firework
        47004, // Fire
        55420, // Lavender
        64885  // Gold
    };
}

class FireworksOnLevel : public PlayerScript
{
public:
    FireworksOnLevel()
        : PlayerScript("FireworksOnLevel", { PLAYERHOOK_ON_LOGIN, PLAYERHOOK_ON_LEVEL_CHANGED })
    {
    }

    void OnPlayerLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>(CONF_ANNOUNCE, true))
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Fireworks on Level|r module.");
    }

    void OnPlayerLevelChanged(Player* player, uint8 /*oldLevel*/) override
    {
        if (!sConfigMgr->GetOption<bool>(CONF_ENABLE, true))
            return;

        // Cast three random fireworks from the list. Sampling without replacement for variety.
        uint8 remaining = FIREWORK_SPELLS.size();
        std::array<uint32, FIREWORK_SPELLS.size()> pool = FIREWORK_SPELLS;

        for (uint8 i = 0; i < 3 && remaining > 0; ++i)
        {
            uint8 index = urand(0, remaining - 1);
            uint32 spellId = pool[index];

            player->CastSpell(player, spellId, true);

            // Move used entry to the end of the active pool.
            std::swap(pool[index], pool[remaining - 1]);
            --remaining;
        }
    }
};

void AddFireworksOnLevelScripts()
{
    new FireworksOnLevel();
}
