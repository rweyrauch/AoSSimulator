/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
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

    bool FrostlordOnThundertusk::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *FrostlordOnThundertusk::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_frostlordTraits[0]);
            auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_frostlordArtefacts[0]);
            auto general = GetBoolParam("General", parameters, false);
            auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_thundertuskTraits[0]);
            return new FrostlordOnThundertusk(tribe, trait, artefact, general, mountTrait);
        }
        return nullptr;
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

    FrostlordOnThundertusk::FrostlordOnThundertusk(Mawtribe tribe, CommandTrait trait, Artefact artefact,
                                                   bool isGeneral, MountTrait mountTrait) :
            MawtribesBase(tribe, "Frostlord on Thundertusk", 8, g_wounds, 9, 3, false) {
        m_keywords = {DESTRUCTION, OGOR, THUNDERTUSK, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, FROSTLORD};
        m_weapons = {&m_ice, &m_spear, &m_kicks, &m_tusks};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_tusks.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        m_mountTrait = mountTrait;

        model->addMissileWeapon(&m_ice);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_kicks);
        model->addMeleeWeapon(&m_tusks);

        addModel(model);

        if (trait == CommandTrait::Master_Of_The_Mournfangs) {
            s_globalBraveryMod.connect(this, &FrostlordOnThundertusk::masterOfMournfangs, &m_masterOfMournfangs);
        }

        m_points = FrostlordOnThundertusk::ComputePoints(1);
    }

    void FrostlordOnThundertusk::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    size_t FrostlordOnThundertusk::getDamageTableIndex() const {
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

    void FrostlordOnThundertusk::onStartHero(PlayerId player) {
        MawtribesBase::onStartHero(player);

        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        const bool isFeeding = !units.empty();

        if (isFeeding && (m_mountTrait == MountTrait::Fleshgreed)) {
            heal(1);
        }
    }

    int FrostlordOnThundertusk::woundModifier() const {
        auto mod = UnitModifierInterface::woundModifier();
        if (m_mountTrait == MountTrait::Gvarnak) {
            mod++;
        }
        return mod;
    }

} // namespace OgorMawtribes
