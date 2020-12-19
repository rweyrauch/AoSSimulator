/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/FrostlordThundertusk.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 350;

    struct TableEntry {
        int m_move;
        int m_ice;
        int m_tusksToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 5, 8, 10, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 12, 2},
                    {7, 10, 3},
                    {6, 8,  3},
                    {5, 6,  4},
                    {4, 4,  4}
            };

    bool FrostlordOnThundertusk::s_registered = false;

    Unit *FrostlordOnThundertusk::Create(const ParameterList &parameters) {
        auto unit = new FrostlordOnThundertusk();

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_frostlordTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_frostlordArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_thundertuskTraits[0]);

        bool ok = unit->configure(mountTrait);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FrostlordOnThundertusk::ValueToString(const Parameter &parameter) {
        return MawtribesBase::ValueToString(parameter);
    }

    int FrostlordOnThundertusk::EnumStringToInt(const std::string &enumString) {
        return MawtribesBase::EnumStringToInt(enumString);
    }

    void FrostlordOnThundertusk::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FrostlordOnThundertusk::Create,
                    FrostlordOnThundertusk::ValueToString,
                    FrostlordOnThundertusk::EnumStringToInt,
                    FrostlordOnThundertusk::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_frostlordTraits[0], g_frostlordTraits),
                            EnumParameter("Artefact", g_frostlordArtefacts[0], g_frostlordArtefacts),
                            EnumParameter("Mount Trait", g_thundertuskTraits[0], g_thundertuskTraits),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Frostlord on Thundertusk", factoryMethod);
        }
    }

    FrostlordOnThundertusk::FrostlordOnThundertusk() :
            MawtribesBase("Frostlord on Thundertusk", 8, g_wounds, 9, 3, false),
            m_ice(Weapon::Type::Missile, "Frost-wreathed Ice", 18, 0, 0, 0, 0, 0),
            m_spear(Weapon::Type::Melee, "Frost Spear", 2, 4, 3, 3, -1, 3),
            m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 3, 0, 1),
            m_tusks(Weapon::Type::Melee, "Colossal Tusks", 2, 4, 3, 2, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, OGOR, THUNDERTUSK, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, FROSTLORD};
        m_weapons = {&m_ice, &m_spear, &m_kicks, &m_tusks};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
    }

    bool FrostlordOnThundertusk::configure(MountTrait mountTrait) {
        auto model = new Model(g_basesize, wounds());

        m_mountTrait = mountTrait;

        model->addMissileWeapon(&m_ice);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_tusks);

        addModel(model);

        m_points = FrostlordOnThundertusk::ComputePoints(1);

        return true;
    }

    void FrostlordOnThundertusk::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int FrostlordOnThundertusk::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void FrostlordOnThundertusk::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_tusks.setToWound(g_damageTable[damageIndex].m_tusksToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    int FrostlordOnThundertusk::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        // Numbing Chill
        if (!weapon->isMissile()) mod--;

        return mod;
    }

    void FrostlordOnThundertusk::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (player == owningPlayer()) {
            if (m_meleeTarget) {
                Dice::RollResult result;
                Dice::RollD6(g_damageTable[getDamageTableIndex()].m_ice, result);
                int toWound = 6;
                if (m_meleeTarget->remainingModels() >= 20) toWound -= 2;
                else if (m_meleeTarget->remainingModels() >= 10) toWound -= 1;

                Wounds wounds = {0, result.rollsGE(toWound)};
                m_meleeTarget->applyDamage(wounds, this);
            }
        }
    }

    int FrostlordOnThundertusk::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
