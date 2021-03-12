/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/ArachnarokSpiderWithFlinger.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz {
    static const int g_basesize = 160;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 220;

    bool ArachnarokSpiderWithFlinger::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_legsAttacks;
        int m_fangsToHit;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 8, 2},
                    {7, 7, 3},
                    {6, 6, 3},
                    {5, 5, 4},
                    {4, 4, 4}
            };

    ArachnarokSpiderWithFlinger::ArachnarokSpiderWithFlinger() :
            GloomspiteGitzBase("Arachnarok Spider with Flinger", 8, g_wounds, 6, 4, true),
            m_spiderBows(Weapon::Type::Missile, "Spider-bows", 16, 8, 5, 5, 0, 1),
            m_flinger(Weapon::Type::Missile, "Flinger", 36, 1, 0, 0, 0, 0),
            m_chitinousLegs(Weapon::Type::Melee, "Chitinous Legs", 3, 8, 4, 3, -1, 1),
            m_monstrousFangs(Weapon::Type::Melee, "Monstrous Fangs", 1, 4, 2, 3, -1, RAND_D3),
            m_crookedSpears(Weapon::Type::Melee, "Crooked Spears", 1, 8, 5, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ARACHNAROK_SPIDER, GLOOMSPITE_GITZ, SPIDERFANG, MONSTER};
        m_weapons = {&m_spiderBows, &m_flinger, &m_chitinousLegs, &m_monstrousFangs, &m_crookedSpears};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_chitinousLegs.setMount(true);
        m_monstrousFangs.setMount(true);
    }

    bool ArachnarokSpiderWithFlinger::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_spiderBows);
        model->addMissileWeapon(&m_flinger);
        model->addMeleeWeapon(&m_chitinousLegs);
        model->addMeleeWeapon(&m_monstrousFangs);
        model->addMeleeWeapon(&m_crookedSpears);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void ArachnarokSpiderWithFlinger::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    void ArachnarokSpiderWithFlinger::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_monstrousFangs.setToHit(g_damageTable[damageIndex].m_fangsToHit);
        m_chitinousLegs.setAttacks(g_damageTable[damageIndex].m_legsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int ArachnarokSpiderWithFlinger::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *ArachnarokSpiderWithFlinger::Create(const ParameterList &parameters) {
        auto unit = new ArachnarokSpiderWithFlinger();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ArachnarokSpiderWithFlinger::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ArachnarokSpiderWithFlinger::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ArachnarokSpiderWithFlinger::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Arachnarok Spider with Flinger", factoryMethod);
        }
    }

    Wounds ArachnarokSpiderWithFlinger::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                     int woundRoll) const {
        // Spider Venom
        int threshold = inLightOfTheBadMoon() ? 5 : 6;
        if ((hitRoll >= threshold) && (weapon->name() == m_monstrousFangs.name())) {
            return {0, Dice::RollD3()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ArachnarokSpiderWithFlinger::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
