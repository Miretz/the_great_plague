#pragma once

#include "entities.hpp"

namespace Areas
{

    const std::string t00_introduction = R"--( 
                                     )DMGI:                                                       
                                     rZ}v^                                                        
                                     T.                    .X}*.                                  
                           `x?!.  `IYQ\V_                  TL                                     
                           `K@@#QMsQ8@w}-                 _wk-                                    
           .-      -=        x@@@@@@@#vM3rixk#r           _Ei                                     
           `wv`    ~#U:      =@@@@@@@@@B~   -BV      ``    K~                                     
            .RR!   uU`       ,#@@@@@@@@m*Yr` w#:     .ZDHyTQVv}XX}.                               
             ,Q#Y ,9Z}='     =@@@@@@@@@Bg@@Q*}@$!,,,":w@@@@@@@@@@@6.                              
               Y@@@@@z`  '~;!g@##@@@@@@@@@@@@@3VQ~     j@@@@@@@@@@@@W`                            
    ,i`        `d@@@@|     `6O:`.#@@@@@@@@@@@#^ Tv    `6@@@@@@@@@@@@@y                            
     d@#V'      -@@@@@b- ^9e`  `^#Gv0@M}M@gID@@@B      V@@@@@@@@@@@@@U                            
     j@@@Q.     !m@@@@@#B@ZwTxr<:~W?G#>xw"   `=}*>`    _0@@@@@@@@@@By`                            
     x##E<    ): =@@@@@@@@@@j`   .)MB@$,         *)"  w@@@@@@@@@@@#B#=                            
     `g@@BQ0GDDVu@$B* _P~v =T>  ~]M=8QPe}      `))`    M@@@@@@@@#hl#@G                            
      z@P_T0.=8vvQuruQg#dQu`   ~UR},Q@edy~     *^:    `Q@@@@@@#V!,k@@}              .____:
       o@@@@@@@@@@@@@@@@@@@@@g#3dz**#M!usuM` .rv:  'xQ@@@@@@B*~rLG@8_ ',~r(vIPx)^~_`              
       -Q@@@@@@@@@@@@@@@@@@@@@@@@@@B@G:rQ^Bi r)\ ^TKM##B@@@@@QQ888#0x?)*!__^$@#D.                 
        ~B@@@@@@@@@@@@@@@@@@@@@@@@@@@#gD#EB#GQQPggMO#@@@@@@@@@@@@@@#MuTwq$#@@QY:                  
          *@@@@@@B8g8B@@@@@@@@@@@##@@@@@@@@@@@@@#Qg8#@@@@@@@@@@BQB@@@@@Q'                         
    .     .wGT*:-  `  _^Ym0BQGi=-``-~vcXPGGayi*"` ` `~}G0$$GY=_.'.~iaE0|     .   .    .~iaE0ayi*~ . ~ai0e~  .  -~vcXPGGayi*~-.
+--------------------------------------------------------------------------------------------------------------------------------+
You have woken up aboard an old smelly vessel called The Lords Wisdom not quite remembering how long you were sailing the great 
seas. Each time you close your eyes you are haunted by the nightmares you saw back home. All those people... your family... 
everyone you knew is gone.

The unstoppable wrath of the plague left many people without their loved ones. You are one of the few desperate survivors who were
lucky enough to flee. This ship was your last attempt to escape to a place hopefully untouched by the disease. As the days went by, 
everyone started to realize that the ship was not spared from the epidemic. One by one people began falling sick, food rations were 
running out. The only way to survive was to reach shore as soon as possible. The captain decided to head towards the shores of 
a nearby island called Damaranzel. 

You remember your mother reading you some stories about this place. In your mind you imagine the lush forests and majestic mountains 
in the north. Maybe the island could be a new beginning for you, free of the disease and full of fascinating places to explore.

Unfortunatelly, the early morning mist obscured the captain's vision, causing the ship to run ashore. You overhear murmurs about 
a city called Alaxis nearby. You decided to disembark and head there by foot...
+--------------------------------------------------------------------------------------------------------------------------------+

)--";

    const std::string t01_shore = R"--(
+--------------------------------------------------------------------------------------------------------------------------------+
Area text to be defined ...
+--------------------------------------------------------------------------------------------------------------------------------+
)--";

    struct Area
    {
        std::string name;
        std::string body;
        std::vector<int> connections;
        std::function<void(std::vector<Hero> &)> auxFunction;
    };

    void navigateToArea(int areaId, std::vector<Hero> &heroes);

    const std::vector<Area> areas{
        {"Introduction", t00_introduction, {1}, nullptr},
        {"Shore", t01_shore, {}, nullptr}};
}