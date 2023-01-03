#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* appearance */
static unsigned int borderpx  = 0;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 0;       /* horiz inner gap between windows */
static unsigned int gappiv    = 0;       /* vert inner gap between windows */
static unsigned int gappoh    = 0;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 0;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 0;        /* 0 means bottom bar */
static char *fonts[]          = { "monospace:size=14", "NotoColorEmoji:pixelsize=14:antialias=true:autohint=true"  };
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#770000";
static char selbgcolor[]      = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", "-e", "tmuxdd", NULL };
/* const char *spcmd1[] = {"st", "-n", "spkitty", "-g", "120x34", "-e", "tmuxdd", NULL }; */
/* const char *spcmd1[] = {"kitty", "--name", "spterm", "tmuxdd", NULL }; */
/* const char *spcmd2[] = {"st", "--title", "spkitty", "-g", "120x34", "-e", "tmuxdd", NULL }; */
const char *spcmd2[] = {"kitty", "--name", "spkitty", "tmuxdd", NULL };
static Sp scratchpads[] = {
  /* name          cmd  */
  {"spterm",      spcmd1},
  {"spkitty",     spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
  */
  /* class    instance      title          tags mask    isfloating   isterminal  noswallow  monitor */
  { "Gimp",     NULL,       NULL,             1 << 8,       0,           0,         0,        -1 },
  { TERMCLASS,  NULL,       NULL,             0,            0,           1,         0,        -1 },
  { NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
  { TERMCLASS,  "bg",       NULL,             1 << 7,       0,           1,         0,        -1 },
  { TERMCLASS,  "spterm",   NULL,             SPTAG(0),     1,           1,         1,        -1 },
  { TERMCLASS,  "popterm",  NULL,             0,            1,           1,         1,        -1 },
  { "kitty",    "spkitty",  NULL,             SPTAG(1),     1,           1,         1,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[M]",  monocle },    /* All windows on top of eachother */

  { "[]=",  tile },     /* Default: Master on left, slaves on right */
  { "TTT",  bstack },   /* Master on top, slaves on bottom */

  { "[@]",  spiral },   /* Fibonacci spiral */
  { "[\\]", dwindle },    /* Decreasing in size right and leftward */

  { "[D]",  deck },     /* Master on left, slaves in monocle-like mode on right */

  { "|M|",  centeredmaster },   /* Master in middle, slaves on sides */
  { ">M>",  centeredfloatingmaster }, /* Same but master floats */

  { "><>",  NULL },     /* no layout function means floating behavior */
  { NULL,   NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
  { MOD,  XK_j, ACTION##stack,  {.i = INC(+1) } }, \
  { MOD,  XK_k, ACTION##stack,  {.i = INC(-1) } },
  /* { MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, */
  /* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
  /* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
  /* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
  /* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "color0",          STRING,  &normbordercolor },
    { "color8",          STRING,  &selbordercolor },
    { "color0",          STRING,  &normbgcolor },
    { "color4",          STRING,  &normfgcolor },
    { "color0",          STRING,  &selfgcolor },
    { "color4",          STRING,  &selbgcolor },
    { "borderpx",        INTEGER, &borderpx },
    { "snap",            INTEGER, &snap },
    { "showbar",         INTEGER, &showbar },
    { "topbar",          INTEGER, &topbar },
    { "nmaster",         INTEGER, &nmaster },
    { "resizehints",     INTEGER, &resizehints },
    { "mfact",           FLOAT,   &mfact },
    { "gappih",          INTEGER, &gappih },
    { "gappiv",          INTEGER, &gappiv },
    { "gappoh",          INTEGER, &gappoh },
    { "gappov",          INTEGER, &gappov },
    { "swallowfloating", INTEGER, &swallowfloating },
    { "smartgaps",       INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
  /* modifier                     key        function        argument */
  STACKKEYS(MODKEY,                          focus)
  STACKKEYS(MODKEY|ShiftMask,                push)
  TAGKEYS(      XK_1,   0)
  TAGKEYS(      XK_2,   1)
  TAGKEYS(      XK_3,   2)
  TAGKEYS(      XK_4,   3)
  TAGKEYS(      XK_5,   4)
  TAGKEYS(      XK_6,   5)
  TAGKEYS(      XK_7,   6)
  TAGKEYS(      XK_8,   7)
  TAGKEYS(      XK_9,   8)
#define Mod(K) MODKEY, XK_##K
#define ModSh(K) (MODKEY|ShiftMask), XK_##K
#define POPTERM(cmd) SHCMD("st -n popterm -g 80x12 -e " cmd)
  {ModSh(Escape),       spawn,           SHCMD("") },
  {Mod(grave),          spawn,           {.v = (const char*[]){ "dmenuunicode", NULL } } },
  {ModSh(grave),        togglescratch,   SHCMD("") },
  {Mod(0),              view,            {.ui = ~0 } },
  {ModSh(0),            tag,             {.ui = ~0 } },
  {Mod(minus),          spawn,           SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
  {ModSh(minus),        spawn,           SHCMD("pamixer --allow-boost -d 15; kill -44 $(pidof dwmblocks)") },
  {Mod(equal),          spawn,           SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
  {ModSh(equal),        spawn,           SHCMD("pamixer --allow-boost -i 15; kill -44 $(pidof dwmblocks)") },
  {Mod(BackSpace),      spawn,           {.v = (const char*[]){ "sysact", NULL } } },
  {ModSh(BackSpace),    spawn,           {.v = (const char*[]){ "sysact", NULL } } },
  {Mod(Tab),            view,            {0} },
  {ModSh(Tab),          spawn,           SHCMD("") },
  {Mod(v),              spawn,           POPTERM("open") },
  {Mod(q),              killclient,      {0} },
  {ModSh(q),            spawn,           {.v = (const char*[]){ "sysact", NULL } } },
  {Mod(w),              spawn,           {.v = (const char*[]){ BROWSER, NULL } } },
  {ModSh(w),            spawn,           {.v = (const char*[]){ TERMINAL, "-e", "sudo", "nmtui", NULL } } },
  {Mod(e),              spawn,           SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") },
  {ModSh(e),            spawn,           SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
  {Mod(r),              spawn,           POPTERM("run")},
  {ModSh(r),            spawn,           POPTERM("run")},
  {Mod(t),              setlayout,       {.v = &layouts[0]} }, /* tile */
  {ModSh(t),            setlayout,       {.v = &layouts[1]} }, /* bstack */
  {Mod(y),              setlayout,       {.v = &layouts[2]} }, /* spiral */
  {ModSh(y),            setlayout,       {.v = &layouts[3]} }, /* dwindle */
  {Mod(u),              setlayout,       {.v = &layouts[4]} }, /* deck */
  {ModSh(u),            setlayout,       {.v = &layouts[5]} }, /* monocle */
  {Mod(i),              setlayout,       {.v = &layouts[6]} }, /* centeredmaster */
  {ModSh(i),            setlayout,       {.v = &layouts[7]} }, /* centeredfloatingmaster */
  {Mod(o),              incnmaster,      {.i = +1 } },
  {ModSh(o),            incnmaster,      {.i = -1 } },
  {Mod(p),              spawn,           {.v = (const char*[]){ "mpc", "toggle", NULL } } },
  {ModSh(p),            spawn,           SHCMD("mpc pause; pauseallmpv") },
  {Mod(bracketleft),    spawn,           {.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
  {ModSh(bracketleft),  spawn,           {.v = (const char*[]){ "mpc", "seek", "-60", NULL } } },
  {Mod(bracketright),   spawn,           {.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
  {ModSh(bracketright), spawn,           {.v = (const char*[]){ "mpc", "seek", "+60", NULL } } },
  {Mod(backslash),      view,            {0} },
  {ModSh(backslash),    spawn,           SHCMD("") },
  {Mod(a),              togglegaps,      {0} },
  {ModSh(a),            defaultgaps,     {0} },
  {Mod(s),              togglesticky,    {0} },
  {ModSh(s),            spawn,           SHCMD("") },
  {Mod(d),              spawn,           {.v = (const char*[]){ "dmenu_run", NULL } } },
  {ModSh(d),            spawn,           {.v = (const char*[]){ "passmenu", NULL } } },

  {Mod(f),              togglefullscr,   {0} },
  {ModSh(f),            setlayout,       {.v = &layouts[8]} },

  {Mod(g),              setmfact,        {.f = -0.05} },
  {Mod(semicolon),      setmfact,        {.f = +0.05} },

  {Mod(h),              shiftview,       { .i = -1 } },
  {Mod(l),              shiftview,       { .i = 1 } },

  {ModSh(g),            shifttag,        {.i = -1 } },
  {ModSh(semicolon),    shifttag,        {.i = 1 } },

  {Mod(apostrophe),     togglescratch,   {.ui = 1} },

  {ModSh(apostrophe),   spawn,           SHCMD("st -e run") },
  {ModSh(apostrophe),   togglesmartgaps, {0} },

  {Mod(Return),         spawn,           {.v = termcmd } },
  {ModSh(Return),       togglescratch,   {.ui = 0} },

  {Mod(z),              incrgaps,        {.i = +3 } },
  {ModSh(z),            spawn,           SHCMD("") },
  {Mod(x),              incrgaps,        {.i = -3 } },
  {ModSh(x),            spawn,           SHCMD("") },
  {Mod(c),              spawn,           {.v = (const char*[]){ TERMINAL, "-e", "profanity", NULL } } },
  {ModSh(c),            spawn,           SHCMD("") },
  {Mod(b),              togglebar,       {0} },
  {ModSh(b),            spawn,           SHCMD("") },
  /* {Mod(n),              spawn,           {.v = (const char*[]){ TERMINAL, "-e", "nvim", "-c", "VimwikiIndex", NULL } } }, */
  /* {ModSh(n),            spawn,           SHCMD(TERMINAL " -e newsboat ; pkill -RTMIN+6 dwmblocks") }, */
  {Mod(m),              spawn,           {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
  {ModSh(m),            spawn,           SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
  {Mod(comma),          spawn,           {.v = (const char*[]){ "mpc", "prev", NULL } } },
  {ModSh(comma),        spawn,           {.v = (const char*[]){ "mpc", "seek", "0%", NULL } } },
  {Mod(period),         spawn,           {.v = (const char*[]){ "mpc", "next", NULL } } },
  {ModSh(period),       spawn,           {.v = (const char*[]){ "mpc", "repeat", NULL } } },
  {Mod(Left),           focusmon,        {.i = -1 } },
  {ModSh(Left),         tagmon,          {.i = -1 } },
  {Mod(Right),          focusmon,        {.i = +1 } },
  {ModSh(Right),        tagmon,          {.i = +1 } },
  {Mod(Page_Up),        shiftview,       { .i = -1 } },
  {ModSh(Page_Up),      shifttag,        {.i = -1 } },
  {Mod(Page_Down),      shiftview,       { .i = +1 } },
  {ModSh(Page_Down),    shifttag,        {.i = +1 } },
  {Mod(Insert),         spawn,           SHCMD("xdotool type $(grep -v '^#' ~/.local/share/larbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") },
  {Mod(F1),             spawn,           SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -") },
  {Mod(F2),             spawn,           {.v = (const char*[]){ "tutorialvids", NULL } } },
  {Mod(F3),             spawn,           {.v = (const char*[]){ "displayselect", NULL } } },
  {Mod(F4),             spawn,           SHCMD("st -e pulsemixer; kill -44 $(pidof dwmblocks)") },
  {Mod(F6),             spawn,           {.v = (const char*[]){ "torwrap", NULL } } },
  {Mod(F7),             spawn,           {.v = (const char*[]){ "td-toggle", NULL } } },
  {Mod(F8),             spawn,           {.v = (const char*[]){ "mw", "-Y", NULL } } },
  {Mod(F9),             spawn,           {.v = (const char*[]){ "dmenumount", NULL } } },
  {Mod(F10),            spawn,           {.v = (const char*[]){ "dmenuumount", NULL } } },
  {Mod(F11),            spawn,           SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
  {Mod(F12),            spawn,           SHCMD("remaps & notify-send \"⌨️ Keyboard remapping...\" \"Re-running keyboard defaults for any newly plugged-in keyboards.\"") },
  /* {Mod(space),          zoom,            {0} }, */
  {Mod(space),          togglescratch,   {.ui = 1} },
  {ModSh(space),        togglescratch,   {.ui = 0} },
  /* {ModSh(space),        togglefloating,  {0} }, */
  {Mod(Print),          spawn,           {.v = (const char*[]){ "dmenurecord", NULL } } },
  {ModSh(Print),        spawn,           {.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
  {Mod(Delete),         spawn,           {.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
  {Mod(Scroll_Lock),    spawn,           SHCMD("killall screenkey || screenkey &") },
  {ShiftMask,           XK_Print,                 spawn,           {.v = (const char*[]){ "maimpick", NULL } } },
  {0,                   XK_Print,                 spawn,           SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
  {0,                   XF86XK_AudioMute,         spawn,           SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
  {0,                   XF86XK_AudioRaiseVolume,  spawn,           SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
  {0,                   XF86XK_AudioLowerVolume,  spawn,           SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
  {0,                   XF86XK_AudioPrev,         spawn,           {.v = (const char*[]){ "mpc", "prev", NULL } } },
  {0,                   XF86XK_AudioNext,         spawn,           {.v = (const char*[]){ "mpc",  "next", NULL } } },
  {0,                   XF86XK_AudioPause,        spawn,           {.v = (const char*[]){ "mpc", "pause", NULL } } },
  {0,                   XF86XK_AudioPlay,         spawn,           {.v = (const char*[]){ "mpc", "play", NULL } } },
  {0,                   XF86XK_AudioStop,         spawn,           {.v = (const char*[]){ "mpc", "stop", NULL } } },
  {0,                   XF86XK_AudioRewind,       spawn,           {.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
  {0,                   XF86XK_AudioForward,      spawn,           {.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
  {0,                   XF86XK_AudioMedia,        spawn,           {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
  {0,                   XF86XK_AudioMicMute,      spawn,           SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
  {0,                   XF86XK_PowerOff,          spawn,           {.v = (const char*[]){ "sysact", NULL } } },
  {0,                   XF86XK_Calculator,        spawn,           {.v = (const char*[]){ TERMINAL, "-e", "bc", "-l", NULL } } },
  {0,                   XF86XK_Sleep,             spawn,           {.v = (const char*[]){ "sudo", "-A", "zzz", NULL } } },
  {0,                   XF86XK_WWW,               spawn,           {.v = (const char*[]){ BROWSER, NULL } } },
  {0,                   XF86XK_DOS,               spawn,           {.v = termcmd } },
  {0,                   XF86XK_ScreenSaver,       spawn,           SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
  {0,                   XF86XK_TaskPane,          spawn,           {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
  {0,                   XF86XK_Mail,              spawn,           SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
  {0,                   XF86XK_MyComputer,        spawn,           {.v = (const char*[]){ TERMINAL, "-e",  "lfub",  "/", NULL } } },
  {0,                   XF86XK_Battery,           spawn,           SHCMD("") },
  {0,                   XF86XK_Launch1,           spawn,           {.v = (const char*[]){ "xset", "dpms", "force", "off", NULL } } },
  {0,                   XF86XK_TouchpadToggle,    spawn,           SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
  {0,                   XF86XK_TouchpadOff,       spawn,           {.v = (const char*[]){ "synclient", "TouchpadOff=1", NULL } } },
  {0,                   XF86XK_TouchpadOn,        spawn,           {.v = (const char*[]){ "synclient", "TouchpadOff=0", NULL } } },
  {0,                   XF86XK_MonBrightnessUp,   spawn,           {.v = (const char*[]){ "xbacklight", "-inc", "15", NULL } } },
  {0,                   XF86XK_MonBrightnessDown, spawn,           {.v = (const char*[]){ "xbacklight", "-dec", "15", NULL } } },

  // TODO: do not work:
  /* { MODKEY,           XK_F5,    xrdb,   {.v = NULL } }, */
  /* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
  /* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
  /* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
  /* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
  /* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
  /* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
  /* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
  /* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
  /* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
  /* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
  /* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
#ifndef __OpenBSD__
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
  { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
  { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
  { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
  { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
  { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
  { ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkClientWin,         MODKEY,         Button4,        incrgaps,       {.i = +1} },
  { ClkClientWin,         MODKEY,         Button5,        incrgaps,       {.i = -1} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
  { ClkTagBar,    0,    Button4,  shiftview,  {.i = -1} },
  { ClkTagBar,    0,    Button5,  shiftview,  {.i = 1} },
  { ClkRootWin,   0,    Button2,  togglebar,  {0} },
};
