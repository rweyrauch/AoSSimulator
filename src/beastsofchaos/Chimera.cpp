/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Chimera.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 220;

    bool Chimera::s_registered = false;

    struct TableEntry {
        int m_fieryBreath;
        int m_avianHeadRend;
        int m_leonineHeadDamage;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {RAND_D6, -3, RAND_D6},
                    {RAND_D3, -2, RAND_D3},
                    {RAND_D3, -2, RAND_D3},
                    {1,       -1, 1},
                    {1,       -1, 1}
            };

    Chimera::Chimera(Greatfray fray) :
            BeastsOfChaosBase("Chimera", 10, g_wounds, 6, 5, true, g_pointsPerUnit),
            m_fieryBreath(Weapon::Type::Missile, "Fiery Breath", 14, 1, 0, 0, 0, RAND_D6),
            m_avianHead(Weapon::Type::Melee, "Avian Head", 1, 3, 3, 4, -3, RAND_D3),
            m_draconicHead(Weapon::Type::Melee, "Draconic Head", 1, 3, 4, 4, -1, 2),
            m_leonineHead(Weapon::Type::Melee, "Leonine Head", 1, 3, 4, 3, -1, RAND_D6),
            m_maulingClaws(Weapon::Type::Melee, "Mauling Claws", 2, 6, 4, 3, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, CHIMERA};
        m_weapons = {&m_fieryBreath, &m_avianHead, &m_draconicHead, &m_leonineHead, &m_maulingClaws};
        m_battleFieldRole = Role::Behemoth;

        setGreatfray(fray);

        auto model = new Model(g_basesize, wounds());

        // NOTE: Fiery Breath attack is special, do not treat it as a weapon

        model->addMeleeWeapon(&m_avianHead);
        model->addMeleeWeapon(&m_draconicHead);
        model->addMeleeWeapon(&m_leonineHead);
        model->addMeleeWeapon(&m_maulingClaws);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Chimera::Create(const ParameterList &parameters) {
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);

        return new Chimera(fray);
    }

    void Chimera::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Chimera", factoryMethod);
        }
    }

    void Chimera::onWounded() {
        const auto damageIndex = getDamageTableIndex();

        m_avianHead.setRend(g_damageTable[damageIndex].m_avianHeadRend);
        m_leonineHead.setDamage(g_damageTable[damageIndex].m_leonineHeadDamage);

        BeastsOfChaosBase::onWounded();
    }

    size_t Chimera::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int Chimera::chargeModifier() const {
        int modifier = BeastsOfChaosBase::chargeModifier();

        // Vicious Charge
        modifier += 2;

        return modifier;
    }

    void Chimera::onStartShooting(PlayerId player) {
        BeastsOfChaosBase::onStartShooting(player);

        if (player == owningPlayer()) {
            // Fiery Breath
            if (m_shootingTarget) {
                if (distanceTo(m_shootingTarget) <= (double) m_fieryBreath.range()) {
                    // Auto-hit and inflict mortal wounds.
                    Wounds breathDamage = {0, Dice::RollSpecial(g_damageTable[getDamageTableIndex()].m_fieryBreath)};
                    m_shootingTarget->applyDamage(breathDamage, this);
                }
            }
        }
    }

    int Chimera::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace BeastsOfChaos
