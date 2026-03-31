#if defined _INC_menu_floating
    #endinput
#endif
#define _INC_menu_floating

#include <a_samp>

#if !defined MENU_FLOATING_MAX_LISTITEM
    #define MENU_FLOATING_MAX_LISTITEM (10)
#endif

#if !defined _INC_open_mp
    #if !defined TextDrawColour
        #define TextDrawColour TextDrawColor
    #endif
    #if !defined TextDrawBackgroundColour
        #define TextDrawBackgroundColour TextDrawBackgroundColor
    #endif
    #if !defined PlayerTextDrawColour
        #define PlayerTextDrawColour PlayerTextDrawColor
    #endif
    #if !defined PlayerTextDrawBackgroundColour
        #define PlayerTextDrawBackgroundColour PlayerTextDrawBackgroundColor
    #endif
    #if !defined TEXT_DRAW_ALIGN_CENTER
        #define TEXT_DRAW_ALIGN_CENTER (2)
    #endif
    #if !defined TEXT_DRAW_FONT_SPRITE_DRAW
        #define TEXT_DRAW_FONT_SPRITE_DRAW (4)
    #endif
#endif

static
    PlayerText:g_tdPlayerMenuFloatingHeader[MAX_PLAYERS][2],
    PlayerText:g_tdPlayerMenuFloatingListItem[MAX_PLAYERS][MENU_FLOATING_MAX_LISTITEM][2];
    
new PlayerText:g_tdArrowRight[MAX_PLAYERS];
new PlayerText:g_tdArrowLeft[MAX_PLAYERS];

static
    g_PlayerMenuFloatingID[MAX_PLAYERS] = { -1, ... },
    g_PlayerMenuFloatingLength[MAX_PLAYERS];

forward OnPlayerMenuFloatingResponse(playerid, extraid, bool:response, listitem);

stock bool:IsPlayerViewingMenuFloating(playerid)
    return (g_PlayerMenuFloatingID[playerid] != -1);

stock bool:ShowMenuFloatingForPlayer(playerid, extraid, const header[], header_colour, const list_items[][], list_length = sizeof list_items)
{
    if (!IsPlayerConnected(playerid))
        return false;

    if (extraid < 0)
        return false;

    if (!list_length)
        return false;

    if (IsPlayerViewingMenuFloating(playerid))
        _destroy_menufloating(playerid, true);

    PlayerTextDrawShow(playerid, g_tdArrowRight[playerid]);
    PlayerTextDrawShow(playerid, g_tdArrowLeft[playerid]);
    
    PlayerTextDrawColour(playerid, g_tdPlayerMenuFloatingHeader[playerid][0], header_colour);
    PlayerTextDrawShow(playerid, g_tdPlayerMenuFloatingHeader[playerid][0]);

    PlayerTextDrawSetString(playerid, g_tdPlayerMenuFloatingHeader[playerid][1], _fix_text(header));
    PlayerTextDrawShow(playerid, g_tdPlayerMenuFloatingHeader[playerid][1]);

    if (list_length > MENU_FLOATING_MAX_LISTITEM)
        list_length = MENU_FLOATING_MAX_LISTITEM;

    g_PlayerMenuFloatingID[playerid] = extraid;
    g_PlayerMenuFloatingLength[playerid] = list_length;

    for (new i = 0; i < list_length; i++)
    {
        g_tdPlayerMenuFloatingListItem[playerid][i][0] = CreatePlayerTextDraw(playerid, 380.0, 175.0 + 26.0 * i, !"LD_SPAC:white");
        PlayerTextDrawTextSize(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][0], 100.0, 25.0);
        PlayerTextDrawColour(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][0], 255);
        PlayerTextDrawFont(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][0], TEXT_DRAW_FONT_SPRITE_DRAW);
        PlayerTextDrawSetSelectable(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][0], true);
        PlayerTextDrawShow(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][0]);

        g_tdPlayerMenuFloatingListItem[playerid][i][1] = CreatePlayerTextDraw(playerid, 386.0, 182.0 + 26.0 * i, _fix_text(list_items[i]));
        PlayerTextDrawLetterSize(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][1], 0.22, 1.15);
        PlayerTextDrawTextSize(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][1], 473.0, 98.0);
        PlayerTextDrawColour(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][1], 0xE1E1E1FF);
        PlayerTextDrawSetShadow(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][1], 0);
        PlayerTextDrawSetOutline(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][1], 1);
        PlayerTextDrawBackgroundColour(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][1], 0);
        PlayerTextDrawShow(playerid, g_tdPlayerMenuFloatingListItem[playerid][i][1]);
    }

    SelectTextDraw(playerid, 0x080808FF);
    return true;
}

stock bool:HideMenuFloatingForPlayer(playerid)
{
    if (_destroy_menufloating(playerid))
    {
        CancelSelectTextDraw(playerid);
        return true;
    }
    return false;
}

static stock _destroy_menufloating(playerid, bool:call = false)
{
    if (!IsPlayerViewingMenuFloating(playerid))
        return 0;

    PlayerTextDrawHide(playerid, g_tdArrowRight[playerid]);
    PlayerTextDrawHide(playerid, g_tdArrowLeft[playerid]);
    
    PlayerTextDrawHide(playerid, g_tdPlayerMenuFloatingHeader[playerid][0]);
    PlayerTextDrawHide(playerid, g_tdPlayerMenuFloatingHeader[playerid][1]);

    for (new j = 0; j < g_PlayerMenuFloatingLength[playerid]; j++)
    {
        PlayerTextDrawDestroy(playerid, g_tdPlayerMenuFloatingListItem[playerid][j][0]);
        PlayerTextDrawDestroy(playerid, g_tdPlayerMenuFloatingListItem[playerid][j][1]);
    }

    if (call)
    {
        CallLocalFunction("OnPlayerMenuFloatingResponse", "iiii", playerid, g_PlayerMenuFloatingID[playerid], false, -1);
    }

    g_PlayerMenuFloatingID[playerid] = -1;
    g_PlayerMenuFloatingLength[playerid] = 0;
    return 1;
}

static stock _fix_text(const string[])
{
    new
        text[128];

    for (new i = 0; i < strlen(string); i++)
    {
        text[i] = string[i];
        switch (text[i])
        {
            case 0xC0 .. 0xC3: text[i] -= 0x40;
            case 0xC7 .. 0xC9: text[i] -= 0x42;
            case 0xD2 .. 0xD5: text[i] -= 0x44;
            case 0xD9 .. 0xDC: text[i] -= 0x47;
            case 0xE0 .. 0xE3: text[i] -= 0x49;
            case 0xE7 .. 0xEF: text[i] -= 0x4B;
            case 0xF2 .. 0xF5: text[i] -= 0x4D;
            case 0xF9 .. 0xFC: text[i] -= 0x50;
            case 0xC4, 0xE4: text[i] = 0x83;
            case 0xC6, 0xE6: text[i] = 0x84;
            case 0xD6, 0xF6: text[i] = 0x91;
            case 0xD1, 0xF1: text[i] = 0xEC;
            case 0xDF: text[i] = 0x96;
            case 0xBF: text[i] = 0xAF;
        }
    }
    return text;
}

static _MenuFloating_OnPlayerConnect(playerid)
{
    g_PlayerMenuFloatingID[playerid] = -1;
    g_PlayerMenuFloatingLength[playerid] = 0;

    g_tdPlayerMenuFloatingHeader[playerid][0] = CreatePlayerTextDraw(playerid, 380.0, 148.0, !"LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, g_tdPlayerMenuFloatingHeader[playerid][0], 100.0, 26.0);
    PlayerTextDrawFont(playerid, g_tdPlayerMenuFloatingHeader[playerid][0], TEXT_DRAW_FONT_SPRITE_DRAW);

    g_tdPlayerMenuFloatingHeader[playerid][1] = CreatePlayerTextDraw(playerid, 430.0, 156.0, #);
    PlayerTextDrawLetterSize(playerid, g_tdPlayerMenuFloatingHeader[playerid][1], 0.22, 1.15);
    PlayerTextDrawTextSize(playerid, g_tdPlayerMenuFloatingHeader[playerid][1], 24.0, 98.0);
    PlayerTextDrawAlignment(playerid, g_tdPlayerMenuFloatingHeader[playerid][1], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawColour(playerid, g_tdPlayerMenuFloatingHeader[playerid][1], 0xE1E1E1FF);
    PlayerTextDrawSetShadow(playerid, g_tdPlayerMenuFloatingHeader[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, g_tdPlayerMenuFloatingHeader[playerid][1], 1);
    PlayerTextDrawBackgroundColour(playerid, g_tdPlayerMenuFloatingHeader[playerid][1], 0);
    
    
    // Flecha derecha '>'
    g_tdArrowRight[playerid] = CreatePlayerTextDraw(playerid, 467.0, 155.0, ">");
    PlayerTextDrawLetterSize(playerid, g_tdArrowRight[playerid], 0.23, 1.4);
    PlayerTextDrawColour(playerid, g_tdArrowRight[playerid], 0xE1E1E1FF);
    PlayerTextDrawSetShadow(playerid, g_tdArrowRight[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_tdArrowRight[playerid], 1);
    PlayerTextDrawBackgroundColour(playerid, g_tdArrowRight[playerid], 0);

    // Flecha izquierda '<'
    g_tdArrowLeft[playerid] = CreatePlayerTextDraw(playerid, 387.0, 155.0, "<");
    PlayerTextDrawLetterSize(playerid, g_tdArrowLeft[playerid], 0.23, 1.4);
    PlayerTextDrawColour(playerid, g_tdArrowLeft[playerid], 0xE1E1E1FF);
    PlayerTextDrawSetShadow(playerid, g_tdArrowLeft[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_tdArrowLeft[playerid], 1);
    PlayerTextDrawBackgroundColour(playerid, g_tdArrowLeft[playerid], 0);
    
    return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if (IsPlayerViewingMenuFloating(playerid))
    {
        if (clickedid == Text:INVALID_TEXT_DRAW)
        {
            CallLocalFunction("OnPlayerMenuFloatingResponse", "iiii", playerid, g_PlayerMenuFloatingID[playerid], false, -1);
            HideMenuFloatingForPlayer(playerid);
        }
    }

    #if defined mf_OnPlayerClickTextDraw
        return mf_OnPlayerClickTextDraw(playerid, Text:clickedid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif

#define OnPlayerClickTextDraw mf_OnPlayerClickTextDraw
#if defined mf_OnPlayerClickTextDraw
    forward mf_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if (IsPlayerViewingMenuFloating(playerid))
    {
        for (new i = 0; i < g_PlayerMenuFloatingLength[playerid]; i++)
        {
            if (playertextid == g_tdPlayerMenuFloatingListItem[playerid][i][0])
            {
                CallLocalFunction("OnPlayerMenuFloatingResponse", "iiii", playerid, g_PlayerMenuFloatingID[playerid], true, i);
                break;
            }
        }
    }

    #if defined mf_OnPlayerClickPlayerTextDraw
        return mf_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnClickPTD
    #undef OnPlayerClickPlayerTextDraw
#else
    #define _ALS_OnClickPTD
#endif


#define OnPlayerClickPlayerTextDraw mf_OnPlayerClickPlayerTextDraw
#if defined mf_OnPlayerClickPlayerTextDraw
    forward mf_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
#endif

public OnPlayerConnect(playerid)
{
    _MenuFloating_OnPlayerConnect(playerid);
    #if defined mf_OnPlayerConnect
        return mf_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect mf_OnPlayerConnect
#if defined mf_OnPlayerConnect
    forward mf_OnPlayerConnect(playerid);
#endif

public OnPlayerDeath(playerid, killerid)
{
    if (_destroy_menufloating(playerid, true))
    {
        CancelSelectTextDraw(playerid);
    }
    
    #if defined mf_OnPlayerDeath
        return mf_OnPlayerDeath(playerid, killerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath mf_OnPlayerDeath
#if defined mf_OnPlayerDeath
    forward mf_OnPlayerDeath(playerid, killerid);
#endif

#if defined FILTERSCRIPT
    public OnFilterScriptInit()
    {
        for (new playerid; playerid < MAX_PLAYERS; playerid++)
        {
            if (IsPlayerConnected(playerid))
                _MenuFloating_OnPlayerConnect(playerid);
        }

        #if defined mf_OnFilterScriptInit
            return mf_OnFilterScriptInit();
        #else
            return 1;
        #endif
    }
    #if defined _ALS_OnFilterScriptInit
        #undef OnFilterScriptInit
    #else
        #define _ALS_OnFilterScriptInit
    #endif

    #define OnFilterScriptInit mf_OnFilterScriptInit
    #if defined mf_OnFilterScriptInit
        forward mf_OnFilterScriptInit();
    #endif

    public OnFilterScriptExit()
    {
        for (new playerid, j = 0; playerid < MAX_PLAYERS; playerid++)
        {
            if (IsPlayerConnected(playerid))
            {
                PlayerTextDrawDestroy(playerid, g_tdPlayerMenuFloatingHeader[playerid][0]);
                PlayerTextDrawDestroy(playerid, g_tdPlayerMenuFloatingHeader[playerid][1]);

                for (j = 0; j < g_PlayerMenuFloatingLength[playerid]; j++)
                {
                    PlayerTextDrawDestroy(playerid, g_tdPlayerMenuFloatingListItem[playerid][j][0]);
                    PlayerTextDrawDestroy(playerid, g_tdPlayerMenuFloatingListItem[playerid][j][1]);
                }
            }
        }

        #if defined mf_OnFilterScriptExit
            return mf_OnFilterScriptExit();
        #else
            return 1;
        #endif
    }
    #if defined _ALS_OnFilterScriptExit
        #undef OnFilterScriptExit
    #else
        #define _ALS_OnFilterScriptExit
    #endif

    #define OnFilterScriptExit mf_OnFilterScriptExit
    #if defined mf_OnFilterScriptExit
        forward mf_OnFilterScriptExit();
    #endif
#else
    public OnGameModeInit()
    {
        #if defined mf_OnGameModeInit
            return mf_OnGameModeInit();
        #else
            return 1;
        #endif
    }
    #if defined _ALS_OnGameModeInit
        #undef OnGameModeInit
    #else
        #define _ALS_OnGameModeInit
    #endif

    #define OnGameModeInit mf_OnGameModeInit
    #if defined mf_OnGameModeInit
        forward mf_OnGameModeInit();
    #endif
#endif
