#define TERMINAL "Alacritty"
#define TERMCLASS "Alacritty"
#define BROWSER "firefox"

#define STR_ARR(...) (const char*[]){ __VA_ARGS__, NULL}
#define CMD(...) { .v = STR_ARR(__VA_ARGS__) }
#define SHCMD(...) CMD("/usr/bin/zsh", "-c", __VA_ARGS__)

#define Mod(K) MODKEY, XK_##K
#define ModSh(K) (MODKEY|ShiftMask), XK_##K
#define ModShCtrl(K) (MODKEY|ShiftMask|ControlMask), XK_##K

#define POPTERM_ROWS "10"
#define POPTERM_COLS "150"

#define POPTERM_KITTY(cmd) \
  SHCMD( \
    "kitty " \
    "--name popterm " \
    "-o initial_window_width=150c " \
    "-o initial_window_height=10c " \
    "--detach "\
    cmd \
  )

#define POPTERM_ALACRITTY(cmd) \
  SHCMD( \
    "alacritty " \
    "--title popterm " \
    "--class Alacritty,popterm " \
    "--option window.dimensions.columns=150 " \
    "--option window.dimensions.lines=10 " \
    "--command " \
    cmd \
  )

#define POPTERM(...) POPTERM_ALACRITTY(__VA_ARGS__)

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
static int focusonwheel       = 0;
static char *fonts[]          = { "Iosevka Nerd Font:size=14", "Fira Sans Compressed:size=14", "NotoColorEmoji:pixelsize=14:antialias=true:autohint=true"  };
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

static Sp scratchpads[] = {
  /* name      cmd  */
  /* {"spterm",  STR_ARR("st", "-n", "spterm", "-g", "120x34", "-e", "tmuxdd")}, */
  /* {"spterm", STR_ARR("st", "-n", "spterm", "-g", "120x34", "-e", "tmuxdd")}, */
  {
    "spterm",
    STR_ARR(
      "alacritty",
      "--class", "Alacritty,spterm",
      "--title", "spterm",
      "--option", "window.dimensions.columns=80",
      "--option", "window.dimensions.lines=25",
      "--command", "tmuxdd"
    )
  },
  {
    "spkitty",
    STR_ARR(
      "kitty",
      "--override", "initial_window_width=80c",
      "--override", "initial_window_height=25c",
      "--name", "spkitty",
      "tmuxdd"
    )
  }
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
  */
  /* class    instance      title          tags mask    isfloating   isterminal  noswallow  monitor */
  /* { "Gimp",     NULL,       NULL,             1 << 8,       0,           0,         0,        -1 }, */
  /* { TERMCLASS,  "bg",       NULL,             1 << 7,       0,           1,         0,        -1 }, */
  /* { TERMCLASS,  "popterm",  NULL,             0,            1,           1,         1,        -1 }, */
  { TERMCLASS,  NULL,       NULL,             0,            0,           1,         0,        -1 },
  { NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
  { TERMCLASS,  "popterm",  NULL,             0,            1,           1,         1,        -1 },
  { "kitty",    "popterm",  NULL,             0,            1,           1,         1,        -1 },
  { TERMCLASS,  "spterm",   NULL,             SPTAG(0),     1,           1,         1,        -1 },
  { "kitty",    "spkitty",  NULL,             SPTAG(1),     1,           1,         1,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
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

// Xresources preferences to load at startup
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
  TAGKEYS(XK_1, 0)
  TAGKEYS(XK_2, 1)
  TAGKEYS(XK_3, 2)
  TAGKEYS(XK_4, 3)
  TAGKEYS(XK_5, 4)
  TAGKEYS(XK_6, 5)
  TAGKEYS(XK_7, 6)
  TAGKEYS(XK_8, 7)
  TAGKEYS(XK_9, 8)
  {ModSh(h),     moveresize, {.v = (int []){ -25, 0, 0, 0 }}},
  {ModSh(l),     moveresize, {.v = (int []){ 25, 0, 0, 0 }}},
  {ModSh(j),     moveresize, {.v = (int []){ 0, 25, 0, 0 }}},
  {ModSh(k),     moveresize, {.v = (int []){ 0, -25, 0, 0 }}},
  {ModShCtrl(h), moveresize, {.v = (int []){ 0, 0, -25, 0 }}},
  {ModShCtrl(l), moveresize, {.v = (int []){ 0, 0, 25, 0 }}},
  {ModShCtrl(j), moveresize, {.v = (int []){ 0, 0, 0, 25 }}},
  {ModShCtrl(k), moveresize, {.v = (int []){ 0, 0, 0, -25 }}},
  {Mod(Escape),         spawn,           SHCMD("slock") },
  {ModSh(Escape),       spawn,           SHCMD("systemctl suspend") },
  {Mod(grave),          spawn,           POPTERM("emoji") },
  {ModSh(grave),        togglescratch,   SHCMD("") },
  {Mod(0),              view,            {.ui = ~0 } },
  {ModSh(0),            tag,             {.ui = ~0 } },
  {Mod(minus),          spawn,           SHCMD("pamixer -d 5") },
  {ModSh(minus),        spawn,           SHCMD("pamixer -d 15") },
  {Mod(equal),          spawn,           SHCMD("pamixer -i 5") },
  {ModSh(equal),        spawn,           SHCMD("pamixer -i 15") },
  {Mod(BackSpace),      spawn,           CMD("sysact") },
  {ModSh(BackSpace),    spawn,           CMD("sysact") },
  {Mod(Tab),            view,            {0} },
  {ModSh(Tab),          spawn,           SHCMD("") },
  {Mod(v),              spawn,           POPTERM("open") },
  {Mod(q),              killclient,      {0} },
  {ModSh(q),            spawn,           CMD("sysact") },
  {Mod(w),              spawn,           CMD(BROWSER) },
  /* {ModSh(w),            spawn,           CMD(TERMINAL, "-e", "sudo", "nmtui") }, */
  /* {Mod(e),              spawn,           SHCMD(TERMINAL " -e neomutt; rmdir ~/.abook") }, */
  /* {ModSh(e),            spawn,           SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") }, */
  {Mod(r),              spawn,           POPTERM("run")},
  {ModSh(r),            spawn,           POPTERM("sudo systemctl disable --now udevmon && remaps")},
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
  /* {Mod(p),              spawn,           CMD("mpc", "toggle") }, */
  /* {ModSh(p),            spawn,           SHCMD("mpc pause; pauseallmpv") }, */
  /* {Mod(bracketleft),    spawn,           CMD("mpc", "seek", "-10") }, */
  /* {ModSh(bracketleft),  spawn,           CMD("mpc", "seek", "-60") }, */
  /* {Mod(bracketright),   spawn,           CMD("mpc", "seek", "+10") }, */
  /* {ModSh(bracketright), spawn,           CMD("mpc", "seek", "+60") }, */
  /* {Mod(backslash),      view,            {0} }, */
  /* {ModSh(backslash),    spawn,           SHCMD("") }, */
  {Mod(a),              spawn,           CMD("alacritty", "--command", "pulsemixer") },
  /* {Mod(a),              defaultgaps,      {0} }, // TODO: I don't want gaps! */
  {ModSh(a),            togglegaps,     {0} },
  {Mod(s),              swapfocus,    {0} },
  {ModSh(s),            togglesticky,    {0} },
  {Mod(d),              spawn,           CMD("dmenu_run") },
  {Mod(f),              togglefullscr,   {0} },
  /* {ModSh(f),            setlayout,       {.v = &layouts[8]} }, */
  {ModSh(f),            togglefloating,  {0} },
  {Mod(g),              setmfact,        {.f = -0.05} },
  {Mod(semicolon),      setmfact,        {.f = +0.05} },
  {Mod(h),              shiftview,       { .i = -1 } },
  {Mod(l),              shiftview,       { .i = 1 } },
  {ModSh(g),            shifttag,        {.i = -1 } },
  {ModSh(semicolon),    shifttag,        {.i = 1 } },
  {Mod(apostrophe),     togglescratch,   {.ui = 0} },
  {ModSh(apostrophe),   spawn,           SHCMD("st -e run") },
  {ModSh(apostrophe),   togglesmartgaps, {0} },
  {Mod(Return),         spawn,           CMD("kitty") },
  {ModSh(Return),       togglescratch,   {.ui = 0} },
  {Mod(z),              incrgaps,        {.i = +3 } },
  {Mod(x),              incrgaps,        {.i = -3 } },
  /* {Mod(c),              spawn,           CMD(TERMINAL, "-e", "profanity") }, */
  {Mod(b),              togglebar,       {0} },
  /* {Mod(n),              spawn,           {.v = (const char*[]){ TERMINAL, "-e", "nvim", "-c", "VimwikiIndex", NULL } } }, */
  /* {ModSh(n),            spawn,           SHCMD(TERMINAL " -e newsboat") }, */
  {Mod(m),              spawn,           CMD(TERMINAL, "-e", "ncmpcpp") },
  {ModSh(m),            spawn,           CMD("pamixer", "-t") },
  {Mod(comma),          spawn,           CMD("mpc", "prev") },
  {ModSh(comma),        spawn,           CMD("mpc", "seek", "0%") },
  {Mod(period),         spawn,           CMD("mpc", "next") },
  {ModSh(period),       spawn,           CMD("mpc", "repeat") },
  {Mod(Left),           focusmon,        {.i = -1 } },
  {ModSh(Left),         tagmon,          {.i = -1 } },
  {Mod(Right),          focusmon,        {.i = +1 } },
  {ModSh(Right),        tagmon,          {.i = +1 } },
  /* {Mod(Insert),         spawn,           SHCMD("xdotool type $(grep -v '^#' ~/.local/share/larbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") }, */
  {ModSh(F1),           spawn,           CMD("kitty", "pulsemixer")}, // SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -") },
  {Mod(F1),             spawn,           CMD("pamixer", "-t")}, // SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -") },
  {Mod(F2),             spawn,           CMD("pamixer", "-d", "3")},
  {Mod(F3),             spawn,           CMD("pamixer", "-i", "3")},
  {Mod(F4),             spawn,           SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
  {Mod(F5),             spawn,           CMD("backlight", "-dec", "10") },
  {Mod(F6),             spawn,           CMD("backlight", "-inc", "10") },
  {Mod(F7),             spawn,           CMD("dock")},
  {Mod(F11),            spawn,           SHCMD("webcam") },
  /* {Mod(space),          zoom,            {0} }, */
  {Mod(space),          togglescratch,   {.ui = 0} },
  {ModSh(space),        togglescratch,   {.ui = 1} },
  /* {ModSh(space),        togglefloating,  {0} }, */
  {Mod(Print),          spawn,           CMD("dmenurecord") },
  {ModSh(Print),        spawn,           CMD("dmenurecord", "kill") },
  {Mod(Delete),         spawn,           CMD("dmenurecord", "kill") },
  {Mod(Scroll_Lock),    spawn,           SHCMD("killall screenkey || screenkey &") },
  {ShiftMask,           XK_Print,                 spawn,           CMD("maimpick") },
  {0,                   XK_Print,                 spawn,           SHCMD("screenshot f") },
  {0,                   XF86XK_AudioMute,         spawn,           SHCMD("pamixer -t") },
  {0,                   XF86XK_AudioRaiseVolume,  spawn,           SHCMD("pamixer --allow-boost -i 3") },
  {0,                   XF86XK_AudioLowerVolume,  spawn,           SHCMD("pamixer --allow-boost -d 3") },
  {0,                   XF86XK_AudioPrev,         spawn,           CMD("mpc", "prev") },
  {0,                   XF86XK_AudioNext,         spawn,           CMD("mpc",  "next") },
  {0,                   XF86XK_AudioPause,        spawn,           CMD("mpc", "pause") },
  {0,                   XF86XK_AudioPlay,         spawn,           CMD("mpc", "play") },
  {0,                   XF86XK_AudioStop,         spawn,           CMD("mpc", "stop") },
  {0,                   XF86XK_AudioRewind,       spawn,           CMD("mpc", "seek", "-10") },
  {0,                   XF86XK_AudioForward,      spawn,           CMD("mpc", "seek", "+10") },
  {0,                   XF86XK_AudioMedia,        spawn,           CMD(TERMINAL, "-e", "ncmpcpp") },
  {0,                   XF86XK_AudioMicMute,      spawn,           SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
  {0,                   XF86XK_PowerOff,          spawn,           CMD("sysact") },
  {0,                   XF86XK_Calculator,        spawn,           CMD(TERMINAL, "-e", "bc", "-l") },
  {0,                   XF86XK_Sleep,             spawn,           CMD("sudo", "-A", "zzz") },
  {0,                   XF86XK_WWW,               spawn,           CMD(BROWSER) },
  {0,                   XF86XK_DOS,               spawn,           CMD("kitty") },
  {0,                   XF86XK_ScreenSaver,       spawn,           SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
  {0,                   XF86XK_TaskPane,          spawn,           CMD(TERMINAL, "-e", "htop") },
  {0,                   XF86XK_Mail,              spawn,           SHCMD(TERMINAL " -e neomutt") },
  {0,                   XF86XK_MyComputer,        spawn,           CMD(TERMINAL, "-e",  "lfub",  "/") },
  {0,                   XF86XK_Battery,           spawn,           SHCMD("") },
  {0,                   XF86XK_Launch1,           spawn,           CMD("xset", "dpms", "force", "off") },
  {0,                   XF86XK_TouchpadToggle,    spawn,           SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
  {0,                   XF86XK_TouchpadOff,       spawn,           CMD("synclient", "TouchpadOff=1") },
  {0,                   XF86XK_TouchpadOn,        spawn,           CMD("synclient", "TouchpadOff=0") },
  {0,                   XF86XK_MonBrightnessUp,   spawn,           CMD("xbacklight", "-inc", "15") },
  {0,                   XF86XK_MonBrightnessDown, spawn,           CMD("xbacklight", "-dec", "15") },

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
  /* { ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} }, */
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
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
