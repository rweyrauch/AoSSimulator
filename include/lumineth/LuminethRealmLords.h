/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LUMINETHREALMLORDS_H
#define LUMINETHREALMLORDS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace LuminethRealmLords {

    enum class GreatNation : int {
        None,
        Syar,
        Iliatha,
        Zaitrec,
        Ymetrica,
    };

    enum class CommandTrait : int {
        None,
        Lords_of_Brilliance,
        Lords_of_Stone,

        // Nation specific

    };

    enum class Artefact : int {
        None,
    };

    enum class Lore : int {
        None,

        // Lore of Hysh
        Speed_of_Hysh,
        Solar_Flare,
        Lambent_Light,
        Etheral_Blessing,
        Total_Eclipse,
        Protection_of_Hysh,

        // Lore of the Mountains
        Unyielding_Calm,
        Crippling_Vertigo,
        Voice_of_the_Mountains,
        Living_Fissue,
        Entomb,
        Assault_of_Stone
    };

    class LuminethBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        LuminethBase() = default;

        ~LuminethBase() override = default;

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);
    protected:

        LuminethBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aetherquartz Reserve             TODO
// Lightning Reactions              TODO
// Shining Company                  TODO
// Enduring as Rock                 TODO
// Tectonic Force                   TODO
// Absorb Despair                   TODO
//

    void Init();

} // namespace LuminethRealmLords

#endif // LUMINETHREALMLORDS_H
