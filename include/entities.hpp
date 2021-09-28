#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

/************************************************************************************************
ENUMS
*************************************************************************************************/

enum class Race
{
    Repsoris,
    Ursobac,
    Rodanto,
    Felidae,
    Strigifor,
    Vulpotis,
};

enum class ItemType
{
    Melee_OneHanded, // Sword, Hatched, Mace, Scimitar, Sabre, Katana
    Melee_TwoHanded, // Hammer, Long Sword, Pike, Quarterstaff, Battle Axe

    Ranged_OneHanded, // Wand, Pistol, Hand crossbow
    Ranged_TwoHanded, // Bow, Crossbow, Long Bow, Musket

    Shield,        // Round Shield, Buckler, Large Shield
    Dual_Wielding, // Dagger,

    Armor_Head,
    Armor_Torso,
    Armor_Legs,
    Armor_Gloves,

    Consumable,
    Throwable, // Grenades, Throwing Knifes, Shuriken, Molotov, Oil Flask
    Scroll
};

enum class EquipmentSlot
{
    MainHand,
    Offhand,
    Torso,
    Head,
    Legs,
    Gloves
};

const std::vector<std::string> equipmentSlotNames{"Main Hand", "Offhand", "Torso", "Head", "Legs", "Gloves"};

enum class Controller
{
    Player,
    AI_Friendly,
    AI_Enemy,
};

/************************************************************************************************
CLASSES
*************************************************************************************************/

struct Attributes
{
    int strength;
    int dexterity;
    int vitality;
    int intelligence;
};

struct RaceDetail
{
    Race id;
    std::string name;
    std::string description;
    Attributes attributes;
};

struct Item
{
    std::string name;
    std::string description;
    ItemType type;

    Attributes requirements;
    int damage;
    int armor;
};

struct Inventory
{
    std::vector<Item> backpack;
    std::unordered_map<std::string, Item> equipped;
};

struct Hero;

struct Ability
{
    std::string name;
    std::string description;
    std::function<void(Hero &, Hero &)> action;
};

struct StatusEffect
{
    std::string name;
    int expireInTurns;
    std::function<void(Hero &)> execute;
};

struct Hero
{
    std::string name;
    int health;
    int xp;

    int maxHealth;
    int level;
    int xpToLevelUp;
    int unspentPoints;

    Race race;
    Controller controller;

    Attributes attributes;
    std::vector<Ability> abilities;
    std::vector<StatusEffect> statusEffects;
    Inventory inventory;
};

/************************************************************************************************
GLOBAL LISTS
*************************************************************************************************/

const std::unordered_map<Race, RaceDetail> g_AllRaces{
    {Race::Repsoris, {Race::Repsoris, "Repsoris", "Identified by their reptilian features and hatred for cold.", {8, 6, 2, 1}}},
    {Race::Ursobac, {Race::Ursobac, "Ursobac", "Their large intimidating physique makes them valuable protectors.", {12, 1, 6, 1}}},
    {Race::Rodanto, {Race::Rodanto, "Rodanto", "They are especially proud of their prominent incisors, although they like to lurk in the shadows.", {1, 14, 2, 3}}},
    {Race::Felidae, {Race::Felidae, "Felidae", "Their grace and finesse is matched by none. Some believe that they have more than one life.", {1, 11, 4, 4}}},
    {Race::Strigifor, {Race::Strigifor, "Strigifor", "Their huge eyes are full of wisdom and understanding, but the feathery crown on their head provides little protection.", {1, 1, 1, 15}}},
    {Race::Vulpotis, {Race::Vulpotis, "Vulpotis", "Slick, cunning and opportunistic. They hide a lot of wits under their dense fur.", {1, 4, 2, 13}}},
};

const std::string introText = R"--( 
                                     )DMGI:                                                       
                                     rZ}v^                                                        
                                     !`                                                           
                                     T.                    .X}*.                                  
                                    =Bx                    :#duTr:                                
                           `x?!.  `IYQ\V_                  TL                                     
                           `K@@#QMsQ8@w}-                 _wk-                                    
                            -Q@@@@@@@@#Qdecx*!`          _)Ku*                                    
           .-      -=        x@@@@@@@#vM3rixk#r           _Ei                                     
           `wv`    ~#U:      =@@@@@@@@@B~   -BV      ``    K~                                     
            .RR!   uU`       ,#@@@@@@@@m*Yr` w#:     .ZDHyTQVv}XX}.                               
             ,Q#Y ,9Z}='     =@@@@@@@@@Bg@@Q*}@$!,,,":w@@@@@@@@@@@6.                              
               Y@@@@@z`  '~;!g@##@@@@@@@@@@@@@3VQ~     j@@@@@@@@@@@@W`                            
    ,i`        `d@@@@|     `6O:`.#@@@@@@@@@@@#^ Tv    `6@@@@@@@@@@@@@y                            
    _QT.        ~@@@@Z`   `eM- =O@@@@@@@@@@@@@#R#>    "#@@@@@@@@@@@@@#:                           
     d@#V'      -@@@@@b- ^9e`  `^#Gv0@M}M@gID@@@B      V@@@@@@@@@@@@@U                            
     j@@@Q.     !m@@@@@#B@ZwTxr<:~W?G#>xw"   `=}*>`    _0@@@@@@@@@@By`                            
     x@@@Q_    *_;@@@@@@@@QzkL^LdB##@@@@@@@@@@@@@@@@##Ds$QB@Q00Edmx~                              
     x##E<    ): =@@@@@@@@@@j`   .)MB@$,         *)"  w@@@@@@@@@@@#B#=                            
     `g@@BQ0GDDVu@$B* _P~v =T>  ~]M=8QPe}      `))`    M@@@@@@@@#hl#@G                            
      z@P_T0.=8vvQuruQg#dQu`   ~UR},Q@edy~     *^:    `Q@@@@@@#V!,k@@}              .____:
       o@@@@@@@@@@@@@@@@@@@@@g#3dz**#M!usuM` .rv:  'xQ@@@@@@B*~rLG@8_ ',~r(vIPx)^~_`              
       -Q@@@@@@@@@@@@@@@@@@@@@@@@@@B@G:rQ^Bi r)\ ^TKM##B@@@@@QQ888#0x?)*!__^$@#D.                 
        ~B@@@@@@@@@@@@@@@@@@@@@@@@@@@#gD#EB#GQQPggMO#@@@@@@@@@@@@@@#MuTwq$#@@QY:                  
         "Q@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@gx!uDx9@@@@@@@#=                    
          *@@@@@@B8g8B@@@@@@@@@@@##@@@@@@@@@@@@@#Qg8#@@@@@@@@@@BQB@@@@@Q'                         
          .wGT*:-  `  _^Ym0BQGi=-``-~vcXPGGayi*"` ` `~}G0$$GY=_.'.~iaE0|

+--------------------------------------------------------------------------------------------------------------------------------+

You have woken up aboard an old smelly vessel called The Lords Wisdom not quite remembering how long you were sailing the great 
seas. Each time you close your eyes you are haunted by the nightmares you saw back home. All those people... your family... 
everyone you knew is gone.

The unstoppable wrath of the plague left many people without their loved ones. You are one of the few desperate survivors who were
lucky enough to flee. This ship was your last attempt to escape to a place hopefully untouched by the disease. As the days went by, 
everyone started to realize that the ship was not spared from the epidemic. One by one people began falling sick, food rations were 
running out. The only way to survive was to reach shore as soon as possible. The captain decided to head towards the shores of a nearby 
island called Damaranzel. 

You remember your mother reading you some stories about this place. In your mind you imagine the lush forests and majestic mountains 
in the north. Maybe the island could be a new beginning for you, free of the disease and full of fascinating places to explore.

Unfortunatelly, the early morning mist obscured the captain's vision, causing the ship to run ashore. You overhear murmurs about a city
called Alaxis nearby. You decided to disembark and head there by foot...

+--------------------------------------------------------------------------------------------------------------------------------+

)--";

const std::string t01_shore = R"--(
+--------------------------------------------------------------------------------------------------------------------------------+
TODO
+--------------------------------------------------------------------------------------------------------------------------------+
)--";

const std::unordered_map<std::string, std::string> g_AllText{
    {"intro", introText},
    {"t01_shore", t01_shore}};
