#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 10;	/* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "hack:size=12", "fontawesome:size=12" };
static const char dmenufont[]       = "hack:size=12";
static const char col_gray1[]       = "#282828"; // background colour
static const char col_gray2[]       = "#444444"; // inactive window border colour
static const char col_gray3[]       = "#ebdbb2"; // font colour
static const char col_gray4[]       = "#7aeb9a"; // current tag/window font colour
static const char col_cyan[]        = "#504945"; // top bar second colour and active window border colour

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const char ptagf[] = "[%s %s]";	/* format of a tag label */
static const char etagf[] = "[%s]";	/* format of an empty tag */
static const int lcaselbl = 0;		/* 1 means make tag label lowercase */	

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "TIL",      tile },    /* first entry is default */
	{ "FLT",      NULL },    /* no layout function means floating behavior */
	{ "MON",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };	// set dmenu settings
static const char *termcmd[] = { "st", NULL }; // set terminal
static const char *monoff[] = { "xset", "dpms", "force", "off", NULL }; // monitor backlight off
static const char *monbrightup[] = { "light", "-A", "5", NULL }; // monitor brightness up
static const char *monbrightdown[] = { "light", "-U", "5", NULL }; // monitor brightness down
static const char *upvol[] = { "pactl", "set-sink-volume", "0", "+5%",     NULL }; // vol up
static const char *downvol[] = { "pactl", "set-sink-volume", "0", "-5%",     NULL }; // vol down
static const char *mutevol[] = { "pactl", "set-sink-mute",   "0", "toggle",  NULL }; // vol off

static Key keys[] = {
	/* modifier                     key        	function        argument				explanation */
	// Program launching
	{ MODKEY,                       XK_d,		spawn,		{.v = dmenucmd } },			// Run dmenu
	{ MODKEY,             		XK_Return,	spawn,		{.v = termcmd } },			// Run terminal
	{ MODKEY,			XK_w,		spawn,		SHCMD("librewolf") },			// Run Librewolf
	{ MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD("brave-beta") },			// Run Brave beta build
	{ MODKEY,			XK_p,	   	spawn,		SHCMD("bitwarden-desktop") },		// Run Bitwarden
	{ MODKEY|ShiftMask,		XK_e,	   	spawn,		SHCMD("element-desktop") },		// Run Element
	{ MODKEY,			XK_e,	   	spawn,		SHCMD("st lf") },			// Run lf
	{ MODKEY|ShiftMask,		XK_l,      	spawn,		SHCMD("slock") },			// Lock sceen
	{ MODKEY,			XK_n,		spawn,		SHCMD("networkmanager_dmenu") },	// Run dmenu network mngr

	// Scripts
	{ MODKEY,			XK_BackSpace,	spawn,		SHCMD("power-off_menu") },		// Select a power off option
	{ 0,				XK_Print,	spawn,		SHCMD("dscreenshot") },			// Open screenshot menu
	{ MODKEY,			XK_Print,	spawn,		SHCMD("dmenurecord") },			// Select a record option
	{ MODKEY|ShiftMask,		XK_Print,	spawn,		SHCMD("dmenurecord kill") },		// Kill recording
		
	// Layouts
	{ MODKEY,                       XK_t,      	setlayout,	{.v = &layouts[0]} },			// Tiled layout
	{ MODKEY,                       XK_f,      	setlayout,	{.v = &layouts[1]} },			// Floating layout
	{ MODKEY,                       XK_m,      	setlayout,	{.v = &layouts[2]} },			// Monocle layout
	{ MODKEY,                       XK_space,  	setlayout,	{0} },					// Switch to previous layout
	
	// Hardware control
	{ 0,                       	XF86XK_AudioLowerVolume, spawn, {.v = downvol } },			// Decrease volume
	{ 0,                       	XF86XK_AudioMute, spawn, {.v = mutevol } },				// Mute volume
	{ 0,                       	XF86XK_AudioRaiseVolume, spawn, {.v = upvol } },			// Increase volume
	{ 0,				XF86XK_MonBrightnessUp, spawn, {.v = monbrightup } },			// Increase backlight
	{ 0,				XF86XK_MonBrightnessDown, spawn, {.v = monbrightdown } },		// Decrease backlight
	{ 0,				XF86XK_ScreenSaver, spawn, {.v = monoff } },				// Turn backlight off

	// Window management
	{ MODKEY|ShiftMask,             XK_space,  	togglefloating,	{0} },					// Toggle floating window
	{ MODKEY,                       XK_0,      	view,		{.ui = ~0 } },				// View all open windows
	{ MODKEY|ShiftMask,             XK_0,      	tag,		{.ui = ~0 } },				// View active window on all workspaces
	{ MODKEY,             		XK_q,      	killclient,	{0} },					// Close window
	{ MODKEY|ShiftMask,             XK_Return, 	zoom,		{0} },					// Set current window as master
	{ MODKEY,                       XK_h,      	setmfact,	{.f = -0.05} },				// Shrink master screen proportion
	{ MODKEY,                       XK_l,      	setmfact,	{.f = +0.05} },				// Increase master screen proportion
	{ MODKEY,                       XK_o,      	incnmaster,	{.i = +1 } },				// Increase number of windows in master stack
	{ MODKEY|ShiftMask,             XK_o,      	incnmaster,	{.i = -1 } },				// Decrease number of windows in master stack

	// Focus (active window)
	{ MODKEY,                       XK_j,      	focusstack,	{.i = +1 } },				// Increase focused window
	{ MODKEY,                       XK_k,      	focusstack,	{.i = -1 } },				// Decrease focused window

	// Gaps
	{ MODKEY,			XK_minus,  	setgaps,	{.i = -1 } },				// Decrease gaps
	{ MODKEY,			XK_equal,  	setgaps,	{.i = +1 } },				// Increase gaps
	{ MODKEY|ShiftMask,		XK_equal,  	setgaps,	{.i = 0  } },				// Remove gaps
	
	// Tags
	{ MODKEY|ShiftMask,             XK_comma,  	tagmon,		{.i = -1 } },				// Decrement tag by 1
	{ MODKEY|ShiftMask,             XK_period, 	tagmon,		{.i = +1 } },				// Increment tag by 1
	TAGKEYS(                        XK_1,		0)							// Tag 1
	TAGKEYS(                        XK_2,           1)							// Tag 2
	TAGKEYS(                        XK_3,           2)							// ...
	TAGKEYS(                        XK_4,           3)
	TAGKEYS(                        XK_5,           4)
	TAGKEYS(                        XK_6,           5)
	TAGKEYS(                        XK_7,           6)							// ...
	TAGKEYS(                        XK_8,           7)							// Tag 8
	TAGKEYS(                        XK_9,           8)							// Tag 9
	{ MODKEY,                       XK_Tab,    	view,		{0} },					// Switch to most recently active tag

	// Multiple monitors
	{ MODKEY,                       XK_comma,  	focusmon,	{.i = -1 } },				// Decrease focused monitor number
	{ MODKEY,                       XK_period, 	focusmon,	{.i = +1 } },				// Increase focused monitor number

	// Misc.
	{ MODKEY,                       XK_b,      	togglebar,	{0} },					// Toggle bar

	// Quit/Restart
	{ MODKEY|ShiftMask,             XK_q,      	quit,		{0} },					// Quit DWM
	{ MODKEY|ControlMask|ShiftMask, XK_q,     	quit,		{1} },					// Restart DWM
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

