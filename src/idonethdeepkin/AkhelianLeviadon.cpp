/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <idonethdeepkin/AkhelianLeviadon.h>
#include <UnitFactory.h>
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 340;

    struct TableEntry {
        int m_save;
        int m_finDamage;
        int m_jawsToHit;
    };

    const size_t g_numTableEntries = 4;
    const int g_woundThresholds[g_numTableEntries] = {8, 11, 14, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {2, 4, 2},
                    {3, 3, 3},
                    {4, 2, 4},
                    {5, 1, 5}
            };

    bool AkhelianLeviadon::s_registered = false;

    AkhelianLeviadon::AkhelianLeviadon(Enclave enclave, MountTrait mountTrait) :
            IdonethDeepkinBase(enclave, "Akhelian Leviadon", 10, g_wounds, 6, 2, true, g_pointsPerUnit),
            m_harpoonLauncher(Weapon::Type::Missile, "Harpoon Launchers", 24, 8, 3, 3, -1, 1),
            m_crushingJaws(Weapon::Type::Melee, "Crushing Jaws", 1, 2, 2, 2, -2, 3),
            m_scythedFins(Weapon::Type::Melee, "Massive Scythed Fins", 2, 4, 2, 3, -1, 4),
            m_spearAndHarpoons(Weapon::Type::Melee, "Twin-pronged Spear and Razorshell Harpoons", 1, 6, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, MONSTER, AKHELIAN, LEVIADON};
        m_weapons = {&m_harpoonLauncher, &m_crushingJaws, &m_scythedFins, &m_spearAndHarpoons};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_scythedFins.setMount(true);
        m_crushingJaws.setMount(true);

        s_globalSaveMod.connect(this, &AkhelianLeviadon::voidDrumSaveMod, &m_voidDrumSaveSlot);
        s_globalToHitMod.connect(this, &AkhelianLeviadon::voidDrumToHitMod, &m_voidDrumHitSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_harpoonLauncher);
        model->addMeleeWeapon(&m_crushingJaws);
        model->addMeleeWeapon(&m_scythedFins);
        model->addMeleeWeapon(&m_spearAndHarpoons);

        addModel(model);
    }

    AkhelianLeviadon::~AkhelianLeviadon() {
        m_voidDrumHitSlot.disconnect();
        m_voidDrumSaveSlot.disconnect();
    }

    Unit *AkhelianLeviadon::Create(const ParameterList &parameters) {
        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        auto trait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_leviadonTrait[0]);
        return new AkhelianLeviadon(enclave, trait);
    }

    void AkhelianLeviadon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                            EnumParameter("Mount Trait", g_leviadonTrait[0], g_leviadonTrait)
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Akhelian Leviadon", factoryMethod);
        }
    }

    Wounds AkhelianLeviadon::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Jaws of Death
        if ((hitRoll >= 6) && (weapon->name() == m_crushingJaws.name())) {
            return {0, 6};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void AkhelianLeviadon::onWounded() {
        Unit::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_scythedFins.setDamage(g_damageTable[damageIndex].m_finDamage);
        m_crushingJaws.setToHit(g_damageTable[damageIndex].m_jawsToHit);
        m_save = g_damageTable[damageIndex].m_save;
    }

    void AkhelianLeviadon::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t AkhelianLeviadon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int AkhelianLeviadon::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int AkhelianLeviadon::voidDrumSaveMod(const Unit *unit, const Weapon *weapon) {
        auto mod = 0;
        if (unit->hasKeyword(IDONETH_DEEPKIN) && isFriendly(unit) && unit->wounds() <= 8) {
            if (unit->distanceTo(this) <= 12.0) mod++;
        }
        return mod;
    }

    int AkhelianLeviadon::voidDrumToHitMod(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        auto mod = 0;
        if (attacker->hasKeyword(NAMARTI) && isFriendly(attacker)) {
            if (target->distanceTo(this) <= 12.0f) mod++;
        }
        return mod;
    }

    void AkhelianLeviadon::onCharged() {
        // Crushing Charge
        if (meleeTarget() && (distanceTo(meleeTarget()) <= 1.0)) {
            if (Dice::RollD6() >= 2) {
                Wounds wounds = {0, Dice::RollD3(), Wounds::Source::Ability};
                if (meleeTarget()->wounds() == 1) {
                    wounds.mortal = Dice::RollD6();
                }
                meleeTarget()->applyDamage(wounds, this);
            }
        }
        IdonethDeepkinBase::onCharged();
    }

} //namespace IdonethDeepkin
