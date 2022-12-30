#include "SymbolDefs.h"

DMapDataSymbols DMapDataSymbols::singleton = DMapDataSymbols();

static AccessorTable DMapDataTable[] =
{
//	All of these return a function label error when used:
	//name,                       tag,            rettype,   var,               funcFlags,  params,optparams
	{ "GetName",                    0,          ZTID_VOID,   -1,                        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "SetName",                    0,          ZTID_VOID,   -1,                        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "GetTitle",                   0,          ZTID_VOID,   -1,                        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "SetTitle",                   0,          ZTID_VOID,   -1,                        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "GetIntro",                   0,          ZTID_VOID,   -1,                        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "SetIntro",                   0,          ZTID_VOID,   -1,                        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "GetMusic",                   0,          ZTID_VOID,   -1,                        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "SetMusic",                   0,          ZTID_VOID,   -1,                        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	//
	{ "getID",                      0,         ZTID_FLOAT,   DMAPDATAID,                0,  { ZTID_DMAPDATA },{} },
	{ "getMap",                     0,         ZTID_FLOAT,   DMAPDATAMAP,               0,  { ZTID_DMAPDATA },{} },
	{ "setMap",                     0,          ZTID_VOID,   DMAPDATAMAP,               0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getLevel",                   0,         ZTID_FLOAT,   DMAPDATALEVEL,             0,  { ZTID_DMAPDATA },{} },
	{ "setLevel",                   0,          ZTID_VOID,   DMAPDATALEVEL,             0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getOffset",                  0,         ZTID_FLOAT,   DMAPDATAOFFSET,            0,  { ZTID_DMAPDATA },{} },
	{ "setOffset",                  0,          ZTID_VOID,   DMAPDATAOFFSET,            0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getCompass",                 0,         ZTID_FLOAT,   DMAPDATACOMPASS,           0,  { ZTID_DMAPDATA },{} },
	{ "setCompass",                 0,          ZTID_VOID,   DMAPDATACOMPASS,           0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getPalette",                 0,         ZTID_FLOAT,   DMAPDATAPALETTE,           0,  { ZTID_DMAPDATA },{} },
	{ "setPalette",                 0,          ZTID_VOID,   DMAPDATAPALETTE,           0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getMIDI",                    0,         ZTID_FLOAT,   DMAPDATAMIDI,              0,  { ZTID_DMAPDATA },{} },
	{ "setMIDI",                    0,          ZTID_VOID,   DMAPDATAMIDI,              0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getContinue",                0,         ZTID_FLOAT,   DMAPDATACONTINUE,          0,  { ZTID_DMAPDATA },{} },
	{ "setContinue",                0,          ZTID_VOID,   DMAPDATACONTINUE,          0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getType",                    0,         ZTID_FLOAT,   DMAPDATATYPE,              0,  { ZTID_DMAPDATA },{} },
	{ "setType",                    0,          ZTID_VOID,   DMAPDATATYPE,              0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getScript",                  0,         ZTID_FLOAT,   DMAPSCRIPT,                0,  { ZTID_DMAPDATA },{} },
	{ "setScript",                  0,          ZTID_VOID,   DMAPSCRIPT,                0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getMusicTrack",              0,         ZTID_FLOAT,   DMAPDATAMUISCTRACK,        0,  { ZTID_DMAPDATA },{} },
	{ "setMusicTrack",              0,          ZTID_VOID,   DMAPDATAMUISCTRACK,        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getActiveSubscreen",         0,         ZTID_FLOAT,   DMAPDATASUBSCRA,           0,  { ZTID_DMAPDATA },{} },
	{ "setActiveSubscreen",         0,          ZTID_VOID,   DMAPDATASUBSCRA,           0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getPassiveSubscreen",        0,         ZTID_FLOAT,   DMAPDATASUBSCRP,           0,  { ZTID_DMAPDATA },{} },
	{ "setPassiveSubscreen",        0,          ZTID_VOID,   DMAPDATASUBSCRP,           0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
//	{ "getGravity[]",               0,         ZTID_FLOAT,   DMAPDATASUBSCRP,           0,  { ZTID_DMAPDATA },{} },
//	{ "setGravity[]",               0,          ZTID_VOID,   DMAPDATASUBSCRP,           0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
//	{ "getJumpThreshold",           0,         ZTID_FLOAT,   DMAPDATASUBSCRP,           0,  { ZTID_DMAPDATA },{} },
//	{ "setJumpThreshold",           0,          ZTID_VOID,   DMAPDATASUBSCRP,           0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getGrid[]",                  0,         ZTID_FLOAT,   DMAPDATAGRID,              0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setGrid[]",                  0,          ZTID_VOID,   DMAPDATAGRID,              0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	{ "getMiniMapTile[]",           0,         ZTID_FLOAT,   DMAPDATAMINIMAPTILE,       0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setMiniMapTile[]",           0,          ZTID_VOID,   DMAPDATAMINIMAPTILE,       0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	{ "getMiniMapCSet[]",           0,         ZTID_FLOAT,   DMAPDATAMINIMAPCSET,       0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setMiniMapCSet[]",           0,          ZTID_VOID,   DMAPDATAMINIMAPCSET,       0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	
	{ "getInitD[]",                 0,         ZTID_FLOAT,   DMAPINITD,                 0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setInitD[]",                 0,          ZTID_VOID,   DMAPINITD,                 0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	{ "getMapTile[]",               0,         ZTID_FLOAT,   DMAPDATALARGEMAPTILE,      0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setMapTile[]",               0,          ZTID_VOID,   DMAPDATALARGEMAPTILE,      0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	{ "getMapCSet[]",               0,         ZTID_FLOAT,   DMAPDATALARGEMAPCSET,      0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setMapCSet[]",               0,          ZTID_VOID,   DMAPDATALARGEMAPCSET,      0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	{ "getDisabledItems[]",         0,         ZTID_FLOAT,   DMAPDATADISABLEDITEMS,     0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setDisabledItems[]",         0,          ZTID_VOID,   DMAPDATADISABLEDITEMS,     0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	{ "getFlags",                   0,         ZTID_FLOAT,   DMAPDATAFLAGS,             0,  { ZTID_DMAPDATA },{} },
	{ "setFlags",                   0,          ZTID_VOID,   DMAPDATAFLAGS,             0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getFlagset[]",               0,         ZTID_FLOAT,   DMAPDATAFLAGARR,           0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setFlagset[]",               0,          ZTID_VOID,   DMAPDATAFLAGARR,           0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	
	{ "getSideview",                0,          ZTID_BOOL,   DMAPDATASIDEVIEW,          0,  { ZTID_DMAPDATA },{} },
	{ "setSideview",                0,          ZTID_VOID,   DMAPDATASIDEVIEW,          0,  { ZTID_DMAPDATA, ZTID_BOOL },{} },
	
	{ "getASubScript",              0,         ZTID_FLOAT,   DMAPDATAASUBSCRIPT,        0,  { ZTID_DMAPDATA },{} },
	{ "setASubScript",              0,          ZTID_VOID,   DMAPDATAASUBSCRIPT,        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getPSubScript",              0,         ZTID_FLOAT,   DMAPDATAPSUBSCRIPT,        0,  { ZTID_DMAPDATA },{} },
	{ "setPSubScript",              0,          ZTID_VOID,   DMAPDATAPSUBSCRIPT,        0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getSubInitD[]",              0,       ZTID_UNTYPED,   DMAPDATASUBINITD,          0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setSubInitD[]",              0,          ZTID_VOID,   DMAPDATASUBINITD,          0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getMapScript",               0,         ZTID_FLOAT,   DMAPDATAMAPSCRIPT,         0,  { ZTID_DMAPDATA },{} },
	{ "setMapScript",               0,          ZTID_VOID,   DMAPDATAMAPSCRIPT,         0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "getMapInitD[]",              0,       ZTID_UNTYPED,   DMAPDATAMAPINITD,          0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setMapInitD[]",              0,          ZTID_VOID,   DMAPDATAMAPINITD,          0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_UNTYPED },{} },
	{ "getCharted[]",               0,         ZTID_FLOAT,   DMAPDATACHARTED,           0,  { ZTID_DMAPDATA, ZTID_FLOAT },{} },
	{ "setCharted[]",               0,          ZTID_VOID,   DMAPDATACHARTED,           0,  { ZTID_DMAPDATA, ZTID_FLOAT, ZTID_FLOAT },{} },
	
	{ "",                           0,          ZTID_VOID,   -1,                        0,  {},{} }
};

DMapDataSymbols::DMapDataSymbols()
{
	table = DMapDataTable;
	refVar = REFDMAPDATA;
}

void DMapDataSymbols::generateCode()
{
}

