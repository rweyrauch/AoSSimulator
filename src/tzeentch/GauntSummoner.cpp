/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Roster.h>
#include "tzeentch/GauntSummoner.h"
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 240;

    bool GauntSummonerOfTzeentch::s_registered = false;

    Unit *GauntSummonerOfTzeentch::Create(const ParameterList &parameters) {
        auto unit = new GauntSummonerOfTzeentch();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GauntSummonerOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GauntSummonerOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    GauntSummonerOfTzeentch::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange),
                            BoolParameter("General")

                    },
                    CHAOS,
                    {TZEENTCH, SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Gaunt Summoner of Tzeentch", factoryMethod);
        }
    }

    GauntSummonerOfTzeentch::GauntSummonerOfTzeentch() :
            TzeentchBase("Gaunt Summoner of Tzeentch", 5, WOUNDS, 8, 6, false),
            m_staff(Weapon::Type::Missile, "Changestaff", 18, 1, 3, 4, 0, RAND_D3),
            m_blade(Weapon::Type::Melee, "Warptongue Blade", 1, 1, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, MORTAL, TZEENTCH, ARCANITE, SLAVES_TO_DARKNESS, EVERCHOSEN, HERO, WIZARD,
                      GAUNT_SUMMONER};
        m_weapons = {&m_staff, &m_blade};
        m_battleFieldRole = Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool GauntSummonerOfTzeentch::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_staff);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds
    GauntSummonerOfTzeentch::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Warptongue Blade
        if ((woundRoll == 6) && (weapon->name() == m_blade.name())) {
            return {0, Dice::rollD6()};
        }
        return TzeentchBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void GauntSummonerOfTzeentch::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            // Book of Profane Secrets
            if (!m_usedBookOfSecrets) {
                // Summon chaos daemon
                static const std::string SummonedUnitNames[6] = {
                        "Horrors of Tzeentch",
                        "Bloodletters",
                        "Plaguebearers",
                        "Daemonettes",
                        "Furies",
                        "Horrors of Tzeentch"
                };

                const int which = Dice::rollD6() - 1;

                auto factory = UnitFactory::LookupUnit(SummonedUnitNames[which]);
                if (factory) {
                    if (m_roster) {
                        auto unit = UnitFactory::Create(SummonedUnitNames[which], factory->m_parameters);
                        unit->deploy(position(), m_orientation);
                        m_roster->addUnit(unit);
                    }
                }
                m_usedBookOfSecrets = true;
            }
        }
    }

    void GauntSummonerOfTzeentch::onRestore() {
        Unit::onRestore();

        m_usedBookOfSecrets = false;
    }

    int GauntSummonerOfTzeentch::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Tzeentch