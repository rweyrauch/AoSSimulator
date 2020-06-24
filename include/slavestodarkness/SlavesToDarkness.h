/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAVESTODARKNESS_H
#define SLAVESTODARKNESS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace SlavesToDarkness {

    enum class DamnedLegion : int {
        Ravagers,
        Cabalists,
        Despoilers,
        Host_of_the_Everchosen
    };

    enum class MarkOfChaos : int {
        Undivided,
        Nurgle,
        Khorne,
        Slaanesh,
        Tzeentch
    };

    enum class Lore : int {
        None,

        // Lore of the Damned
        Binding_Damnation,
        Spite_Tongue_Curse,
        Whispers_of_Chaos,
        Mask_of_Darkness,
        Call_to_Glory,
        Ruinous_Vigour
    };

    enum class CommandTrait : int {
        None,

        // Ravagers
        Bolstered_by_Hate,
        Unquestioned_Resolve,
        Favoured_of_the_Pantheon,
        Eternal_Vendetta,
        Flames_of_Spite,
        Master_of_Deception,

        // Cabalists
        //Bolstered_by_Hate,
        Lord_of_Terror,
        //Favoured_of_the_Pantheon,
        Mighty_Ritualist,
        Blasphemous_Influence,
        All_for_One,

        // Despoilers
        //Bolstered_by_Hate,
        //Lord_of_Terror,
        Lightning_Reflexes,
        Radiance_of_Dark_Glory,
        Distorting_Miasma,
        Paragon_of_Ruin,
    };

    enum class Artefact : int {
        None,

        // Ravagers
        Hellfire_Sword,
        Blasphemous_Cuirass,
        Helm_of_the_Oppressor,
        Cloak_of_the_Relentless_Conqueror,
        Mark_of_the_High_Favoured,
        Desecrator_Gauntlets,

        // Cabalists
        Soul_Feeder,
        Black_Athame,
        Infernal_Puppet,
        Spelleater_Pendant,
        Scroll_of_Dark_Unravelling,
        Spell_Familiar,

        // Despoilers
        Crown_of_Hellish_Adoration,
        Helm_of_Many_Eyes,
        Armour_of_Tortured_Souls,
        Diabolic_Mantle,
        Doombringer_Blade,
        Realmwarpers_Twist_Rune
    };

    class SlavesToDarknessBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SlavesToDarknessBase() = default;

        ~SlavesToDarknessBase() override = default;

        void setDamnedLegion(DamnedLegion legion);

        void setMarkOfChaos(MarkOfChaos mark);

    protected:

        SlavesToDarknessBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon) const override;

    private:

        DamnedLegion m_legion = DamnedLegion::Ravagers;
        MarkOfChaos m_markOfChaos = MarkOfChaos::Undivided;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Chaos
//    Aura of Khorne                Yes
//    Aura of Tzeentch              Partial/TODO
//    Aura of Nurgle                TODO
//    Aura of Slaanesh              TODO
//    Aura of Chaos Undivided       TODO
// Eye of the Gods                  TODO
// Ravagers
//    Glory for the Taking          TODO
//    Rally the Tribes              TODO
// Cabalists
//    Ritual of Sorcerous Might     TODO
//    Ritual of Corruption          TODO
// Despoilers
//    Sacrilegious Might            TODO
//    Blessed by the Unholy         TODO
//    Twisted Dominion              TODO
// Host of the Everchosen
//    Fearless in His Presence      TODO
//    The Will of the Everchosen    TODO
//    Dark Prophecy                 TODO
//

} // SlavesToDarkness

#endif //SLAVESTODARKNESS_H