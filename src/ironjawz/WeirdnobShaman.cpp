/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "ironjawz/WeirdnobShaman.h"
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 120;

    bool OrrukWeirdnobShaman::s_registered = false;

    Unit *OrrukWeirdnobShaman::Create(const ParameterList &parameters) {
        auto unit = new OrrukWeirdnobShaman();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_shamanCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_shamanArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        unit->configure(lore);
        return unit;
    }

    int OrrukWeirdnobShaman::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void OrrukWeirdnobShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukWeirdnobShaman::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    OrrukWeirdnobShaman::ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_shamanCommandTraits[0], g_shamanCommandTraits),
                            EnumParameter("Artefact", g_shamanArtefacts[0], g_shamanArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Weirdnob Shaman", factoryMethod);
        }
    }

    OrrukWeirdnobShaman::OrrukWeirdnobShaman() :
            Ironjawz("Orruk Weirdnob Shaman", 4, g_wounds, 6, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Waaagh! Staff", 1, 3, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, WIZARD, WEIRDNOB_SHAMAN};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;
        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    void OrrukWeirdnobShaman::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<LineOfEffectSpell>(this, "Green Puke", 6, RAND_2D6, RAND_D3, 0));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    void OrrukWeirdnobShaman::onEndHero(PlayerId player) {
        Ironjawz::onEndHero(player);

        // Brutal Power
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
            for (auto unit : units) {
                if (unit->hasKeyword(IRONJAWZ) && (unit->remainingModels() >= 10)) {
                    auto matchName = [](std::unique_ptr<Spell> &spell) -> bool {
                        return (spell->name() == "Green Puke");
                    };
                    auto sip = std::find_if(m_knownSpells.begin(), m_knownSpells.end(), matchName);
                    if (sip != m_knownSpells.end()) {
                        auto target = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
                        auto spell = sip->get();
                        auto successful = spell->cast(target, m_battleRound);
                        if (successful == Spell::Result::Success) {
                            PLOG_INFO.printf("%s successfully cast %s", m_name.c_str(), spell->name().c_str());
                            onCastSpell(spell, target);
                        }
                    }
                }
            }
        }
    }

} // namespace Ironjawz