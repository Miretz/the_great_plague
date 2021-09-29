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
You have woken up aboard an old, smelly vessel called The Lord's Wisdom, not quite remembering how long you have been sailing 
the great seas. Each time you close your eyes, you are haunted by the nightmares you saw back home. Your friends... your family... 
most of the people you knew are gone.

The unstoppable wrath of the plague left many people without their loved ones. You are one of the few desperate survivors lucky 
enough to flee. This was your last attempt to escape to a place hopefully untouched by the disease. As the days went by, 
everyone started to realize that the ship was not spared from the epidemic. One by one, people began falling sick. Food rations 
were running out. The only way to survive was to reach the shore as soon as possible. The captain decided to head towards the 
shores of a nearby island called Damaranzel.

You remember your mother reading you some stories about this place. In your mind, you imagine the lush forests and majestic 
mountains in the north. Maybe the island could be a new beginning for you, free of the disease and full of fascinating places 
to explore. Unfortunatelly, the early morning mist obscured the captain's vision, causing the ship to run ashore. You overhear 
murmurs about a city called Alaxis nearby. You decided to disembark and head there by foot...
+--------------------------------------------------------------------------------------------------------------------------------+

)--";

    const std::string danseaPicture = R"--(:g#z:8@M-- --wb$r-r8QQBBBBQ8$dc!:rTY*:!ybwL=  !:               
 `O#m'K@#G* ~`-xKV<rTdZdMhywsZgQ$bz=!*za}!`_r}> `Y:              
 Y#d`=B@Q:-  ^u,!_:}Oq,`      '*wOEWV=_*cay*, -` s8-             
'd#~`M@@Qr  -`!, _|GM3.          `!TZP]!^vxuYx^,`:3di`           
dy  !B#Xk`  .uWMZZi`                  *8EYvxx!_-`` 'YZT.         
#~`E@#}xD~`-OQgH`                       :lEQQw^oy"    `rx        
:.`Q@$i!9QT!$8i        '`                `-vB##BGVZZT:  :x_      
'.D@@#0^^Gr*DZ_  _vYx}KMe3bMdZX~       *MQ#BOa0#BQ$mkqu: !V_     
 -Q@@MuKm.rK0*   _:_   Tk  O88d!     rqB# #   #@##QQQH)!=-'V*    
-#@@#V_`_8B6R'    _rzv  j  =0i-     *Q8d  #  Q#@@@8xMBQWZy']M   
(@@@Q- -b#B66,       "*I##UL:       _gD >xywv: xQ@@#3(}g6r^!!8`  
Z@@@w `6#BQRqr                       yQ"        W##Mua$Q#BRi"P`  
8@@D :##QQEg$II=                      wm       rB@BQ#######V`3:  
g@@d `D#}=K$Q9oe|`                    `DW`    -D##Q(B##B##BBE#^  
y@@0` mB!`UP6Q0KIk,           ^^*=`  `~6}`   -M@@#g*}BQBBQQQB@^  
xQjmY_*#E^ZOadQg5XV-            !*zXVx*`    ~9@@##w__}9v$Q8Q##<  
wdr#B} P@#Q#BQgg8$dj=                  `='`uB@@##Bv*^.^ !>xB#B_  
#B:*lP*~@@@###BQQQgO5u_  >r_` `.__-,~^rvv,=B@@@##BL,~.   `e@@O   
cG|qR|`V@@@@##B##QQg9qP^`:-~vx~~vv)?xz\ .LB@@@##BB*  -  !v6@@*   
V>~qdu`)g@@#@##B@#BBQ065!    `;}uLv(r! |Q@@@@@##QbT`',=}9Q@@u    
 - _*rB#MB$>^^Y$#@@@##BB8*          `V#@@@@@@###QQ=   ,r(Bx      
    '$@I`z<    `x@@@@####QVx::=)v)xIB@@@@@@@@###8QO"   -;*mr!`   
    `g@hVq=-_   :@@@@@#########@@@@@@@@@@@@@####QMQq      "mUV}. 
)--";

    const std::string danseaIntro = R"--(+--------------------------------------------------------------------------------------------------------------------------------+
As you step out of the ship you feel relieved. Finally stable ground under your feet after such a long time. You look around 
trying to figure out the way to the city. After a few steps towards the trees, you suddenly hear a female voice calling out 
your name. You recognize this voice. It is Dansea, the butcher's daughter, a Felidae girl you know from your home village. 
Before the plague started, she used to be a hunter. She approaches you with a bow in her hand.
+--------------------------------------------------------------------------------------------------------------------------------+

)--";

    const std::string t01_shore = R"--(VjviMPix!*ogy_!LE#dO8go?ZiYbBz:_~,=vj###0"`,`!~`!:_'                                                                
,_<)3Kvy^>:LZv^wI##0c3GPQmiw#Bz}xv*:.*g##x~!_```  -:` ``                                                            
r:!rMd\^ijVumBgLxd#QMZVyR}vcQ#RQ~.iv,:_Z#D~,`  `  `._,'`                                                            
)x*k$yLxTlO#DuQaPWQ##ZcZR(`!ZQTZIrkZ|^^K#Bv=!_.`   `._.                                                             
`-=vK^`*y?}##3ZBB3Q#BB#Gc3^!3Qc>oB]dd\-)#Q) ~:`     ``' ``                                                          
_`:P5V_;x:rOBB5gei$QR3EaIc??m#EKvoOQBY=qB#$c:`                                                                      
-;uB5\*Vh=vgB9H:rxg#c`)y*.-yuQ#Vvv0##3~}B##^ `'`                                        _r_                         
``(QV-=MGu}Q#ddBKV$#Qmgu"._i^mQ^`?ZQ#}`'^Q#i                                     _r       "                         
 .P#l`,u^__a#PPQ<-5#ILI~` _T*H8x^_ x#m` ,$#c'``                                  `-`     -l^*!                      
._uBc`.i!`!3#U-,~.I#u````_*xVQBj,. ?Bd~ `o#c`                                     .-   r6Q#@#!`!. `                 
 .uBM!*iv<)8#s``  x#l`````'`,MB(` `uQd,  xQv                                    =OZ9x`"8@@@@B-`:***                 
  vQB*_-__'3#K`   x#x`      _R#x -r0#Z*'`,L:                                   :B@@@x k@@@@@#k`=#e  ,'              
  `U#k"  `.c#V``  xBv   `'  -d#w^vqB#s:_..,_`                                  !QBggZ<^TxyMIYUHg#* "R`              
   v#E: .:_m#u`  _ZBr     ``_0#PiK$#Bm!_-uRxrv*"                               `HQ8BZ}xLLT8}!x#@Q"`-R::             
  'I#D>-;uPQ8!   .O$_       `u#y}D##g*_:K#Dxxg@#Rm*LVVv*uucVTy}TlTTLiLYYL}}}}LLo#@@@bVIcVMQZPRQQ#0EE8GKwkwoIhIIwcTyI
``,G#O\<xb8BU-`  ^Qd'        uBxrz6Qy:~dQg#QYlY8@#KY}})!jm=......_!:-.-_,,:rIPXUQQQ@BMGqZ$8g8Q#@@@@@DL.```.---.`  :h
:)}Z#BDdyW#QIrT):D#d,`      .zQ~-,rkwY3##BQW8@Qm$8#@@8x=L* `!~~^<r^^>^*^~=~)V$BQ#@@@@#BBB##@@@@@@@@QYx^!"":"",,:!=*x
IB##@#######OzV6k#Bdv,'      v3!`_<rU#Qd8IaUZ@@#E#@@QdUr!"!````  ...._...___,"*Q@@@@@@@@@@@@@@@@@@@U::,`      ````  
8###########gGmIu0DT*``      _>,  ``=OB#QPZgka##QQQDecTvr==_----_,-''.         .___________=r~,_,::_```         ```'
Q###@#######BOdyyIx,`              )#@@BRQ#D(~TY)xckePb0QBBQgd3ky}!_-.````                 `:^=!-`````````````````':
EB###@#####B$Zy!=HMy*`            x@@@#QQ#@@Br\QgMwyY?*rvrxxkchqR0QQBBBQQQDdOMaKmjyVVullTLxv}lllluuT}ix?)))rr*r))?vx
MD6RggDabQ$QPx:_-LZdy"`` `-,=~=::,#@@#BQ8#@@BQMQ@@@BB6V\^=^*;:!,---_,!~>^*\xxlwjedddZOPPHGZOR99DED$000QBB#BBBBBBBBBQ
LxTPbKr!*ywec^=:^``_---:TjVx*~r*<:!=***vv}L}x>~::)V}jzx*r=~!,*mT=..--:*rrv())IMwx*=r*!,,-..-_=*rrv>*>^",~khu?^=r*!::
ZgQ$IL~"vk}*=:!<:_:_!*vvzV>:":~~:^VVeXT)~=)\~!=!=>(}i^~iTr=~!^*_---::*zoiL}yzMai*=:;!_<Vv:-:!Lsc)>=~*vicYsu~:":~!,rK
uV}~=vTLww)!=,:!_rGurruuViiui~;~rzPVa}=:,"!::iKX}~=i*!_"^=,,^L|!--^iTcsVr)vwoXTGDHr~=:vv,~?T]yzx;^!~~^ygbwvxVL===:vr
+--------------------------------------------------------------------------------------------------------------------------------+
You're standing on a sandy beach lined by a lush forest. You can see the beached vessel from here. There's also a path
between the trees.
+--------------------------------------------------------------------------------------------------------------------------------+

)--";

    const std::string t02_forest = R"--(
+--------------------------------------------------------------------------------------------------------------------------------+
TODO
+--------------------------------------------------------------------------------------------------------------------------------+

)--";

    struct Area
    {
        std::string name;
        std::string body;
        std::vector<uint32_t> connections;
        std::function<void(GameState &)> auxFunction;
    };
    
    void navigateToArea(uint32_t areaId, GameState &game);

    void t01_shore_DanseaConversation(GameState &game);

    const std::vector<Area> allAreas{
        {"Introduction", t00_introduction, {1}, nullptr},
        {"Shore", t01_shore, {2}, t01_shore_DanseaConversation},
        {"Forest", t02_forest, {1}, nullptr}};
}