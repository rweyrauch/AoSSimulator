/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include "bonesplitterz/WurrgogProphet.h"

namespace Bonesplitterz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    bool WurrgogProphet::s_registered = false;

    Unit *WurrgogProphet::Create(const ParameterList &parameters) {
        auto unit = new WurrgogProphet();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, Bonegrinz);
        unit->setWarclan(warclan);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WurrgogProphet::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };
            s_registered = UnitFactory::Register("Wurrgog Prophet", factoryMethod);
        }
    }

    WurrgogProphet::WurrgogProphet() :
            Bonesplitterz("Wurrgog Prophet", 5, WOUNDS, 8, 5, false),
            m_staffAndShiv(Weapon::Type::Melee, "Wurrgog Staff and Bone Shiv", 1, 4, 4, 3, 0, RAND_D3),
            m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 1, RAND_D3, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, WIZARD, WURRGOG_PROPHET};
        m_weapons = {&m_staffAndShiv, &m_fangedMaw};
        m_totalUnbinds = 2;
        m_totalSpells = 2;
    }

    bool WurrgogProphet::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staffAndShiv);
        model->addMeleeWeapon(&m_fangedMaw);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int WurrgogProphet::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        // Beast Mask
        if (!weapon->isMissile()) {
            mod -= 1;
        }
        return mod;
    }

    void WurrgogProphet::onStartHero(PlayerId playerId) {
        Unit::onStartHero(playerId);

        if (owningPlayer() == playerId) {
            // Prophet of Da Waaagh!
            if (Dice::rollD6() >= 4) {
                m_roster->addCommandPoints(1);
            }
        }
    }

    int WurrgogProphet::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Bonesplitterz
