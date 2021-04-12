/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/KlaqTrok.h>
#include <UnitFactory.h>

namespace Seraphon {
    static const int g_basesize = 32;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 0;

    bool KlaqTrok::s_registered = false;

    KlaqTrok::KlaqTrok(bool isGeneral) :
            SeraphonBase("Klaq-Trok", 6, g_wounds, 8, 4, false),
            m_warblade(Weapon::Type::Melee, "Celestite Warblade", 1, 5, 3, 3, 0, 1),
            m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1) {
        m_keywords = {ORDER, SERAPHON, COALESCED, THUNDER_LIZARD, SAURUS, HERO, OLDBLOOD, KLAQ_TROK};
        m_weapons = {&m_warblade, &m_jaws};
        m_battleFieldRole = Role::Leader;

        setWayOfTheSeraphon(WayOfTheSeraphon::Coalesced, Constellation::Thunder_Lizard);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_warblade);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);

        m_points = ComputePoints(1);
    }

    Unit *KlaqTrok::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new KlaqTrok(general);
    }

    void KlaqTrok::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Klaq-Trok", factoryMethod);
        }
    }

    int KlaqTrok::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Cold Ferocity
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_warblade.name())) {
            return 2;
        }
        return SeraphonBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int KlaqTrok::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
