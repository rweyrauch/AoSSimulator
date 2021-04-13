/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/FrostlordStonehorn.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 400;

    struct TableEntry {
        int m_move;
        int m_hornsAttacks;
        int m_hoovesToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 5, 8, 10, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 6, 2},
                    {10, 5, 3},
                    {8,  4, 3},
                    {6,  3, 4},
                    {4,  2, 4}
            };

    bool FrostlordOnStonehorn::s_registered = false;

    bool FrostlordOnStonehorn::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *FrostlordOnStonehorn::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_frostlordTraits[0]);
            auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_frostlordArtefacts[0]);
            auto general = GetBoolParam("General", parameters, false);
            auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_stonehornTraits[0]);
            return new FrostlordOnStonehorn(tribe, trait, artefact, general, mountTrait);
        }
        return nullptr;
    }

    std::string FrostlordOnStonehorn::ValueToString(const Parameter &parameter) {
        return MawtribesBase::ValueToString(parameter);
    }

    int FrostlordOnStonehorn::EnumStringToInt(const std::string &enumString) {
        return MawtribesBase::EnumStringToInt(enumString);
    }

    void FrostlordOnStonehorn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FrostlordOnStonehorn::Create,
                    FrostlordOnStonehorn::ValueToString,
                    FrostlordOnStonehorn::EnumStringToInt,
                    FrostlordOnStonehorn::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_frostlordTraits[0], g_frostlordTraits),
                            EnumParameter("Artefact", g_frostlordArtefacts[0], g_frostlordArtefacts),
                            EnumParameter("Mount Trait", g_stonehornTraits[0], g_stonehornTraits),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Frostlord on Stonehorn", factoryMethod);
        }
    }

    FrostlordOnStonehorn::FrostlordOnStonehorn(Mawtribe tribe, CommandTrait trait, Artefact artefact,
                                               bool isGeneral, MountTrait mountTrait) :
            MawtribesBase(tribe, "Frostlord on Stonehorn", 12, g_wounds, 9, 3, false) {
        m_keywords = {DESTRUCTION, OGOR, STONEHORN, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, FROSTLORD};
        m_weapons = {&m_spear, &m_kicks, &m_horns, &m_hooves};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_hooves.setMount(true);
        m_horns.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        m_mountTrait = mountTrait;

        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_hooves);

        addModel(model);

        if (trait == CommandTrait::Master_Of_The_Mournfangs) {
            s_globalBraveryMod.connect(this, &FrostlordOnStonehorn::masterOfMournfangs, &m_masterOfMournfangs);
        }

        m_points = FrostlordOnStonehorn::ComputePoints(1);
    }

    void FrostlordOnStonehorn::onRestore() {
        MawtribesBase::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    size_t FrostlordOnStonehorn::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        if (m_commandTrait == CommandTrait::Skilled_Rider) {
            woundsInflicted /= 2;
        }
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void FrostlordOnStonehorn::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_horns.setAttacks(g_damageTable[damageIndex].m_hornsAttacks);
        m_hooves.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        MawtribesBase::onWounded();
    }

    Wounds
    FrostlordOnStonehorn::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Earth-shattering Charge
        if (m_charged && (weapon->name() == m_horns.name() || weapon->name() == m_hooves.name())) {
            return {weapon->damage() + 1, 0};
        }
        return MawtribesBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Wounds FrostlordOnStonehorn::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Stone Skeleton
        return ignoreWounds(wounds, 5);
    }

    int FrostlordOnStonehorn::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int FrostlordOnStonehorn::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = MawtribesBase::toHitModifier(weapon, target);
        if ((weapon->name() == m_horns.name()) && (m_mountTrait == MountTrait::Black_Clatterhorn)) {
            mod++;
        }
        return mod;
    }

    int FrostlordOnStonehorn::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = MawtribesBase::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
        if ((weapon->name() == m_hooves.name()) && (m_mountTrait == MountTrait::Frosthoof_Bull)) {
            rend--;
        }
        return rend;
    }

    int FrostlordOnStonehorn::woundModifier() const {
        auto mod = MawtribesBase::woundModifier();
        if (m_mountTrait == MountTrait::Rockmane_Elder) {
            mod++;
        }
        return mod;
    }

} // namespace OgorMawtribes
