#include "base/zdefs.h"
#include "jwin.h"
#include "base/zapp.h"
#include "dialog/info.h"
#include <sstream>

extern byte quest_rules[QUESTRULES_NEW_SIZE];

using std::string;
using namespace util;

extern PALETTE RAMpal;
extern bool update_hw_pal;

bool global_z3_scrolling_extended_height_mode = true;
viewport_t viewport;
int32_t global_z3_cur_scr_drawing = -1;

const char months[13][13] =
{ 
	"Nonetober", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

char *VerStr(int32_t version)
{
    static char ver_str[12];
    sprintf(ver_str,"v%d.%02X",version>>8,version&0xFF);
    return ver_str;
}

RGB _RGB(byte *si)
{
	RGB x;
	x.r = si[0];
	x.g = si[1];
	x.b = si[2];
	x.filler=0;
	return x;
}

RGB _RGB(int32_t r,int32_t g,int32_t b)
{
	RGB x;
	x.r = r;
	x.g = g;
	x.b = b;
	x.filler=0;
	return x;
}

RGB invRGB(RGB s)
{
	RGB x;
	x.r = 63-s.r;
	x.g = 63-s.g;
	x.b = 63-s.b;
	x.filler=0;
	return x;
}

RGB mixRGB(int32_t r1,int32_t g1,int32_t b1,int32_t r2,int32_t g2,int32_t b2,int32_t ratio)
{
	RGB x;
	x.r = (r1*(64-ratio) + r2*ratio) >> 6;
	x.g = (g1*(64-ratio) + g2*ratio) >> 6;
	x.b = (b1*(64-ratio) + b2*ratio) >> 6;
	x.filler=0;
	return x;
}

static char themefile[2048] = {0};
char tmp_themefile[2048] = {0};
char const* get_themefile()
{
	return themefile;
}
void set_theme(char const* fpath)
{
	strcpy(themefile, fpath);
	reset_theme();
}
void reset_theme()
{
	load_themefile(themefile);
}
void load_themefile(char const* fpath)
{
	load_themefile(fpath, RAMpal);
}
void load_themefile(char const* fpath, PALETTE pal)
{
	push_config_state();
	set_config_file(fpath);
	pal[dvc(1)] = _RGB(zc_get_config("Theme","dvc1_r",4),zc_get_config("Theme","dvc1_g",38),zc_get_config("Theme","dvc1_b",46)); //box fg is text
	pal[dvc(2)] = _RGB(zc_get_config("Theme","dvc2_r",(16*63/255)), zc_get_config("Theme","dvc2_g",(10*63/255)), zc_get_config("Theme","dvc2_b",0));
	pal[dvc(3)] = _RGB(zc_get_config("Theme","dvc3_r",17),zc_get_config("Theme","dvc3_g",20),zc_get_config("Theme","dvc3_b",20)); //slate
	pal[dvc(4)] = _RGB(zc_get_config("Theme","dvc4_r",13),zc_get_config("Theme","dvc4_g",14),zc_get_config("Theme","dvc4_b",14)); //menu background
	pal[dvc(5)] = _RGB(zc_get_config("Theme","dvc5_r",0),zc_get_config("Theme","dvc5_g",0),zc_get_config("Theme","dvc5_b",0));//menu text bg
	pal[dvc(6)] = _RGB(zc_get_config("Theme","dvc6_r",13),zc_get_config("Theme","dvc6_g",14),zc_get_config("Theme","dvc6_b",14));//menu selected text
	pal[dvc(7)] = _RGB(zc_get_config("Theme","dvc7_r",42),zc_get_config("Theme","dvc7_g",60),zc_get_config("Theme","dvc7_b",48));
	pal[dvc(8)] = _RGB(zc_get_config("Theme","dvc8_r",6),zc_get_config("Theme","dvc8_g",49),zc_get_config("Theme","dvc8_b",35));//highlight on selected menu text
					   
	jwin_pal[jcBOX]	=dvc(zc_get_config("Theme","jcbox",4));
	jwin_pal[jcLIGHT]  =dvc(zc_get_config("Theme","jclight",5));
	jwin_pal[jcMEDLT]  =dvc(zc_get_config("Theme","jcmedlt",4));
	jwin_pal[jcMEDDARK]=dvc(zc_get_config("Theme","jcmeddark",3));
	jwin_pal[jcDARK]   =dvc(zc_get_config("Theme","jcdark",2));
	jwin_pal[jcBOXFG]  =dvc(zc_get_config("Theme","jcboxfg",1));
	jwin_pal[jcTITLEL] =dvc(zc_get_config("Theme","jctitlel",3));
	jwin_pal[jcTITLER] =dvc(zc_get_config("Theme","jctitler",5));
	jwin_pal[jcTITLEFG]=dvc(zc_get_config("Theme","jctitlefg",7));
	jwin_pal[jcTEXTBG] =dvc(zc_get_config("Theme","jctextbg",5));
	jwin_pal[jcTEXTFG] =dvc(zc_get_config("Theme","jctextfg",1));
	jwin_pal[jcSELBG]  =dvc(zc_get_config("Theme","jcselbg",8));
	jwin_pal[jcSELFG]  =dvc(zc_get_config("Theme","jcselfg",6));
	jwin_pal[jcCURSORMISC] = dvc(zc_get_config("Theme","jccursormisc",1));
	jwin_pal[jcCURSOROUTLINE] = dvc(zc_get_config("Theme","jccursoroutline",2));
	jwin_pal[jcCURSORLIGHT] = dvc(zc_get_config("Theme","jccursorlight",3));
	jwin_pal[jcCURSORDARK] = dvc(zc_get_config("Theme","jccursordark",5));
	jwin_pal[jcALT_TEXTFG] = dvc(zc_get_config("Theme","jc_alt_textfg",r_dvc(jwin_pal[jcMEDDARK])));
	jwin_pal[jcALT_TEXTBG] = dvc(zc_get_config("Theme","jc_alt_textbg",r_dvc(jwin_pal[jcTEXTFG])));
	jwin_pal[jcDISABLED_FG] = dvc(zc_get_config("Theme","jc_disabled_fg",r_dvc(jwin_pal[jcMEDDARK])));
	jwin_pal[jcDISABLED_BG] = dvc(zc_get_config("Theme","jc_disabled_bg",r_dvc(jwin_pal[jcBOX])));
	
	pop_config_state();
	
    gui_bg_color=jwin_pal[jcBOX];
    gui_fg_color=jwin_pal[jcBOXFG];
    jwin_set_colors(jwin_pal);
	update_hw_pal = true;
}

void save_themefile(char const* fpath)
{
	save_themefile(fpath, RAMpal);
}
void save_themefile(char const* fpath, PALETTE pal)
{
	push_config_state();
	set_config_file(fpath);
	zc_set_config("Theme","dvc1_r",pal[dvc(1)].r); zc_set_config("Theme","dvc1_g",pal[dvc(1)].g); zc_set_config("Theme","dvc1_b",pal[dvc(1)].b);
	zc_set_config("Theme","dvc2_r",pal[dvc(2)].r); zc_set_config("Theme","dvc2_g",pal[dvc(2)].g); zc_set_config("Theme","dvc2_b",pal[dvc(2)].b);
	zc_set_config("Theme","dvc3_r",pal[dvc(3)].r); zc_set_config("Theme","dvc3_g",pal[dvc(3)].g); zc_set_config("Theme","dvc3_b",pal[dvc(3)].b);
	zc_set_config("Theme","dvc4_r",pal[dvc(4)].r); zc_set_config("Theme","dvc4_g",pal[dvc(4)].g); zc_set_config("Theme","dvc4_b",pal[dvc(4)].b);
	zc_set_config("Theme","dvc5_r",pal[dvc(5)].r); zc_set_config("Theme","dvc5_g",pal[dvc(5)].g); zc_set_config("Theme","dvc5_b",pal[dvc(5)].b);
	zc_set_config("Theme","dvc6_r",pal[dvc(6)].r); zc_set_config("Theme","dvc6_g",pal[dvc(6)].g); zc_set_config("Theme","dvc6_b",pal[dvc(6)].b);
	zc_set_config("Theme","dvc7_r",pal[dvc(7)].r); zc_set_config("Theme","dvc7_g",pal[dvc(7)].g); zc_set_config("Theme","dvc7_b",pal[dvc(7)].b);
	zc_set_config("Theme","dvc8_r",pal[dvc(8)].r); zc_set_config("Theme","dvc8_g",pal[dvc(8)].g); zc_set_config("Theme","dvc8_b",pal[dvc(8)].b);
	zc_set_config("Theme","jcbox",r_dvc(jwin_pal[jcBOX]));
	zc_set_config("Theme","jclight",r_dvc(jwin_pal[jcLIGHT]));
	zc_set_config("Theme","jcmedlt",r_dvc(jwin_pal[jcMEDLT]));
	zc_set_config("Theme","jcmeddark",r_dvc(jwin_pal[jcMEDDARK]));
	zc_set_config("Theme","jcdark",r_dvc(jwin_pal[jcDARK]));
	zc_set_config("Theme","jcboxfg",r_dvc(jwin_pal[jcBOXFG]));
	zc_set_config("Theme","jctitlel",r_dvc(jwin_pal[jcTITLEL]));
	zc_set_config("Theme","jctitler",r_dvc(jwin_pal[jcTITLER]));
	zc_set_config("Theme","jctitlefg",r_dvc(jwin_pal[jcTITLEFG]));
	zc_set_config("Theme","jctextbg",r_dvc(jwin_pal[jcTEXTBG]));
	zc_set_config("Theme","jctextfg",r_dvc(jwin_pal[jcTEXTFG]));
	zc_set_config("Theme","jcselbg",r_dvc(jwin_pal[jcSELBG]));
	zc_set_config("Theme","jcselfg",r_dvc(jwin_pal[jcSELFG]));
	zc_set_config("Theme","jccursormisc",r_dvc(jwin_pal[jcCURSORMISC]));
	zc_set_config("Theme","jccursoroutline",r_dvc(jwin_pal[jcCURSOROUTLINE]));
	zc_set_config("Theme","jccursorlight",r_dvc(jwin_pal[jcCURSORLIGHT]));
	zc_set_config("Theme","jccursordark",r_dvc(jwin_pal[jcCURSORDARK]));
	zc_set_config("Theme","jc_alt_textfg",r_dvc(jwin_pal[jcALT_TEXTFG]));
	zc_set_config("Theme","jc_alt_textbg",r_dvc(jwin_pal[jcALT_TEXTBG]));
	zc_set_config("Theme","jc_disabled_fg",r_dvc(jwin_pal[jcDISABLED_FG]));
	zc_set_config("Theme","jc_disabled_bg",r_dvc(jwin_pal[jcDISABLED_BG]));
	pop_config_state();
}

const char* get_app_theme_filename()
{
	switch (get_app_id()) {
		case App::zelda: return "_custom_zc.ztheme";
		case App::zquest: return "_custom_zq.ztheme";
		case App::launcher: return "_custom_zcl.ztheme";
		default: return "_custom.ztheme";
	}
}

void load_udef_colorset(char const* fpath)
{
	load_udef_colorset(fpath, RAMpal);
}
void load_udef_colorset(char const* fpath, PALETTE pal)
{
	push_config_state();
	set_config_file(fpath);
	char const* darkthemename = "themes/dark.ztheme";
	char const* tfnm = zc_get_config("Theme", "theme_filename", "-");
	bool defaulted_theme = !(tfnm[0]&&tfnm[0]!='-');
	strcpy(tmp_themefile, defaulted_theme ? darkthemename : tfnm);
	
	fix_filename_case(tmp_themefile);
	fix_filename_slashes(tmp_themefile);
	if(defaulted_theme
		&& get_config_int("Theme","dvc1_r",4)==get_config_int("Theme","dvc1_r",5))
	{
		//Write these back to the custom theme file
		strcpy(tmp_themefile, get_app_theme_filename());
		load_themefile(zc_get_standard_config_name(), pal);
		save_themefile(tmp_themefile, pal);
	}
	else load_themefile(tmp_themefile, pal);
	if (defaulted_theme)
		zc_set_config("Theme", "theme_filename", tmp_themefile);
	pop_config_state();
}

void load_colorset(int32_t colorset)
{
	load_colorset(colorset, RAMpal);
}
void load_colorset(int32_t colorset, PALETTE pal)
{
	bool udef = false;
	switch(colorset)
	{
		case 1:  //Windows 98
		{
			pal[dvc(1)] = _RGB(0*63/255,   0*63/255,   0*63/255);
			pal[dvc(2)] = _RGB(128*63/255, 128*63/255, 128*63/255);
			pal[dvc(3)] = _RGB(192*63/255, 192*63/255, 192*63/255);
			pal[dvc(4)] = _RGB(223*63/255, 223*63/255, 223*63/255);
			pal[dvc(5)] = _RGB(255*63/255, 255*63/255, 255*63/255);
			pal[dvc(6)] = _RGB(255*63/255, 255*63/255, 225*63/255);
			pal[dvc(7)] = _RGB(255*63/255, 225*63/255, 160*63/255);
			pal[dvc(8)] = _RGB(0*63/255,   0*63/255,  80*63/255);
			
			byte palrstart=  0*63/255, palrend=166*63/255,
				 palgstart=  0*63/255, palgend=202*63/255,
				 palbstart=128*63/255, palbend=240*63/255,
				 paldivs=7;
				 
			for(int32_t i=0; i<paldivs; i++)
			{
				pal[dvc(15-paldivs+1)+i].r = palrstart+((palrend-palrstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].g = palgstart+((palgend-palgstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].b = palbstart+((palbend-palbstart)*i/(paldivs-1));
			}
			
			jwin_pal[jcBOX]	=dvc(3);
			jwin_pal[jcLIGHT]  =dvc(5);
			jwin_pal[jcMEDLT]  =dvc(4);
			jwin_pal[jcMEDDARK]=dvc(2);
			jwin_pal[jcDARK]   =dvc(1);
			jwin_pal[jcBOXFG]  =dvc(1);
			jwin_pal[jcTITLEL] =dvc(9);
			jwin_pal[jcTITLER] =dvc(15);
			jwin_pal[jcTITLEFG]=dvc(7);
			jwin_pal[jcTEXTBG] =dvc(5);
			jwin_pal[jcTEXTFG] =dvc(1);
			jwin_pal[jcSELBG]  =dvc(8);
			jwin_pal[jcSELFG]  =dvc(6);
		}
		break;
		
		case 2:  //Windows 99
		{
			pal[dvc(1)] = _RGB(0*63/255,   0*63/255,   0*63/255);
			pal[dvc(2)] = _RGB(64*63/255,  64*63/255,  64*63/255);
			pal[dvc(3)] = _RGB(128*63/255, 128*63/255, 128*63/255);
			pal[dvc(4)] = _RGB(192*63/255, 192*63/255, 192*63/255);
			pal[dvc(5)] = _RGB(223*63/255, 223*63/255, 223*63/255);
			pal[dvc(6)] = _RGB(255*63/255, 255*63/255, 255*63/255);
			pal[dvc(7)] = _RGB(255*63/255, 255*63/255, 225*63/255);
			pal[dvc(8)] = _RGB(255*63/255, 225*63/255, 160*63/255);
			pal[dvc(9)] = _RGB(0*63/255,   0*63/255,  80*63/255);
			
			byte palrstart=  0*63/255, palrend=166*63/255,
				 palgstart=  0*63/255, palgend=202*63/255,
				 palbstart=128*63/255, palbend=240*63/255,
				 paldivs=6;
				 
			for(int32_t i=0; i<paldivs; i++)
			{
				pal[dvc(15-paldivs+1)+i].r = palrstart+((palrend-palrstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].g = palgstart+((palgend-palgstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].b = palbstart+((palbend-palbstart)*i/(paldivs-1));
			}
			
			jwin_pal[jcBOX]	=dvc(4);
			jwin_pal[jcLIGHT]  =dvc(6);
			jwin_pal[jcMEDLT]  =dvc(5);
			jwin_pal[jcMEDDARK]=dvc(3);
			jwin_pal[jcDARK]   =dvc(2);
			jwin_pal[jcBOXFG]  =dvc(1);
			jwin_pal[jcTITLEL] =dvc(10);
			jwin_pal[jcTITLER] =dvc(15);
			jwin_pal[jcTITLEFG]=dvc(8);
			jwin_pal[jcTEXTBG] =dvc(6);
			jwin_pal[jcTEXTFG] =dvc(1);
			jwin_pal[jcSELBG]  =dvc(9);
			jwin_pal[jcSELFG]  =dvc(7);
		}
		break;
		
		case 3:  //Windows 2000 Blue
		{
			pal[dvc(1)] = _RGB(0*63/255,   0*63/255,   0*63/255);
			pal[dvc(2)] = _RGB(16*63/255,  15*63/255, 116*63/255);
			pal[dvc(3)] = _RGB(82*63/255,  80*63/255, 182*63/255);
			pal[dvc(4)] = _RGB(162*63/255, 158*63/255, 250*63/255);
			pal[dvc(5)] = _RGB(255*63/255, 255*63/255, 255*63/255);
			pal[dvc(6)] = _RGB(255*63/255, 255*63/255, 127*63/255);
			pal[dvc(7)] = _RGB(255*63/255, 225*63/255,  63*63/255);
			pal[dvc(8)] = _RGB(0*63/255,   0*63/255,  80*63/255);
			
			byte palrstart=  0*63/255, palrend=162*63/255,
				 palgstart=  0*63/255, palgend=158*63/255,
				 palbstart= 80*63/255, palbend=250*63/255,
				 paldivs=7;
				 
			for(int32_t i=0; i<paldivs; i++)
			{
				pal[dvc(15-paldivs+1)+i].r = palrstart+((palrend-palrstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].g = palgstart+((palgend-palgstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].b = palbstart+((palbend-palbstart)*i/(paldivs-1));
			}
			
			jwin_pal[jcBOX]	=dvc(4);
			jwin_pal[jcLIGHT]  =dvc(5);
			jwin_pal[jcMEDLT]  =dvc(4);
			jwin_pal[jcMEDDARK]=dvc(3);
			jwin_pal[jcDARK]   =dvc(2);
			jwin_pal[jcBOXFG]  =dvc(1);
			jwin_pal[jcTITLEL] =dvc(9);
			jwin_pal[jcTITLER] =dvc(15);
			jwin_pal[jcTITLEFG]=dvc(7);
			jwin_pal[jcTEXTBG] =dvc(5);
			jwin_pal[jcTEXTFG] =dvc(1);
			jwin_pal[jcSELBG]  =dvc(8);
			jwin_pal[jcSELFG]  =dvc(6);
		}
		break;
		
		case 687:  //Windows 2000 Gold (6-87 was the North American release date of LoZ)
		{
			pal[dvc(1)] = _RGB(0*63/255,   0*63/255,   0*63/255);
			pal[dvc(2)] = _RGB(64*63/255,  64*63/255,  43*63/255);
			pal[dvc(3)] = _RGB(170*63/255, 154*63/255,  96*63/255);
			pal[dvc(4)] = _RGB(223*63/255, 200*63/255, 128*63/255); // Old Gold
			pal[dvc(5)] = _RGB(240*63/255, 223*63/255, 136*63/255);
			pal[dvc(6)] = _RGB(255*63/255, 223*63/255, 128*63/255);
			pal[dvc(7)] = _RGB(255*63/255, 223*63/255, 128*63/255);
			pal[dvc(8)] = _RGB(255*63/255, 225*63/255, 160*63/255);
			pal[dvc(9)] = _RGB(80*63/255,  80*63/255,   0*63/255);
			
			byte palrstart=128*63/255, palrend=240*63/255,
				 palgstart=128*63/255, palgend=202*63/255,
				 palbstart=  0*63/255, palbend=166*63/255,
				 paldivs=6;
				 
			for(int32_t i=0; i<paldivs; i++)
			{
				pal[dvc(15-paldivs+1)+i].r = palrstart+((palrend-palrstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].g = palgstart+((palgend-palgstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].b = palbstart+((palbend-palbstart)*i/(paldivs-1));
			}
			
			jwin_pal[jcBOX]	=dvc(4);
			jwin_pal[jcLIGHT]  =dvc(6);
			jwin_pal[jcMEDLT]  =dvc(5);
			jwin_pal[jcMEDDARK]=dvc(3);
			jwin_pal[jcDARK]   =dvc(2);
			jwin_pal[jcBOXFG]  =dvc(1);
			jwin_pal[jcTITLEL] =dvc(10);
			jwin_pal[jcTITLER] =dvc(15);
			jwin_pal[jcTITLEFG]=dvc(8);
			jwin_pal[jcTEXTBG] =dvc(6);
			jwin_pal[jcTEXTFG] =dvc(1);
			jwin_pal[jcSELBG]  =dvc(9);
			jwin_pal[jcSELFG]  =dvc(7);
		}
		break;
		
		case 4104:  //Windows 2000 Easter (4-1-04 is April Fools Day, the date of this release)
		{
			pal[dvc(1)] = _RGB(0*63/255,   0*63/255,   0*63/255);
			pal[dvc(2)] = _RGB(64*63/255,  64*63/255,  64*63/255);
			pal[dvc(3)] = _RGB(128*63/255, 128*63/255, 128*63/255);
			pal[dvc(4)] = _RGB(252*63/255, 186*63/255, 188*63/255);
			pal[dvc(5)] = _RGB(254*63/255, 238*63/255, 238*63/255);
			pal[dvc(6)] = _RGB(244*63/255, 243*63/255, 161*63/255);
			pal[dvc(7)] = _RGB(120*63/255, 173*63/255, 189*63/255);
			pal[dvc(8)] = _RGB(220*63/255, 183*63/255, 227*63/255);
			
			byte palrstart=244*63/255, palrend=220*63/255,
				 palgstart=243*63/255, palgend=183*63/255,
				 palbstart=161*63/255, palbend=227*63/255,
				 paldivs=7;
				 
			for(int32_t i=0; i < paldivs; i++)
			{
				pal[dvc(15-paldivs+1)+i].r = palrstart+((palrend-palrstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].g = palgstart+((palgend-palgstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].b = palbstart+((palbend-palbstart)*i/(paldivs-1));
			}
			
			jwin_pal[jcBOX]	=dvc(4);
			jwin_pal[jcLIGHT]  =dvc(5);
			jwin_pal[jcMEDLT]  =dvc(4);
			jwin_pal[jcMEDDARK]=dvc(3);
			jwin_pal[jcDARK]   =dvc(2);
			jwin_pal[jcBOXFG]  =dvc(7);
			jwin_pal[jcTITLEL] =dvc(9);
			jwin_pal[jcTITLER] =dvc(15);
			jwin_pal[jcTITLEFG]=dvc(7);
			jwin_pal[jcTEXTBG] =dvc(5);
			jwin_pal[jcTEXTFG] =dvc(7);
			jwin_pal[jcSELBG]  =dvc(8);
			jwin_pal[jcSELFG]  =dvc(6);
		}
		break;
		
		case 2019:  //2.55 DARK Theme
		{
		   
			pal[dvc(1)] = _RGB(4,38,46); //box fg is text
			pal[dvc(2)] = _RGB(16*63/255, 10*63/255, 0*63/255);
			pal[dvc(3)] = _RGB(17,20,20); //slate
			pal[dvc(4)] = _RGB(13,14,14); //menu background
			pal[dvc(5)] = _RGB(0,0,0);//menu text bg
			pal[dvc(6)] = _RGB(13,14,14);//menu selected text
			pal[dvc(7)] = _RGB(42,60,48);
			pal[dvc(8)] = _RGB(6,49,35);//highlight on selected menu text
			
			byte palrstart= 10*63/255, palrend=166*63/255,
				 palgstart= 36*63/255, palgend=202*63/255,
				 palbstart=106*63/255, palbend=240*63/255,
				 paldivs=7;
		   
			jwin_pal[jcBOX]	=dvc(4);
			jwin_pal[jcLIGHT]  =dvc(5);
			jwin_pal[jcMEDLT]  =dvc(4);
			jwin_pal[jcMEDDARK]=dvc(3);
			jwin_pal[jcDARK]   =dvc(2);
			jwin_pal[jcBOXFG]  =dvc(1);
			jwin_pal[jcTITLEL] =dvc(3);
			jwin_pal[jcTITLER] =dvc(5);
			jwin_pal[jcTITLEFG]=dvc(7);
			jwin_pal[jcTEXTBG] =dvc(5);
			jwin_pal[jcTEXTFG] =dvc(1);
			jwin_pal[jcSELBG]  =dvc(8);
			jwin_pal[jcSELFG]  =dvc(6);
		}
		break;
		
		case 99:  //User Defined
		{
			udef = true;
			load_udef_colorset(zc_get_standard_config_name(), pal);
			strcpy(themefile, tmp_themefile);
		}
		break;
		
		case 201018:  //20-oct-2018, PureZC Expo
		{
			//16,10,0; dark chocolate
			//39,19,0 chocolate
			//63,23,0 orange
			//46,32,4 tan
			pal[dvc(1)] = _RGB(63,23,0); //box fg is text
			pal[dvc(2)] = _RGB(16*63/255, 10*63/255, 0*63/255);
			pal[dvc(3)] = _RGB(39,19,0);
		   // pal[dvc(4)] = _RGB(212*63/255, 208*63/255, 200*63/255);
			pal[dvc(4)] = _RGB(16,10,0); //menu background
			pal[dvc(5)] = _RGB(0,0,0);
			//pal[dvc(5)] = _RGB(63*63/255, 23*63/255, 0*63/255);
			//pal[dvc(5)] = _
			pal[dvc(6)] = _RGB(0,0,0);
			pal[dvc(7)] = _RGB(255*63/255, 225*63/255, 160*63/255);
			pal[dvc(8)] = _RGB(63,49,0);
			
			byte palrstart= 10*63/255, palrend=166*63/255,
				 palgstart= 36*63/255, palgend=202*63/255,
				 palbstart=106*63/255, palbend=240*63/255,
				 paldivs=7;
			/*	 
			for(int32_t i=0; i<paldivs; i++)
			{
				pal[dvc(15-paldivs+1)+i].r = palrstart+((palrend-palrstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].g = palgstart+((palgend-palgstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].b = palbstart+((palbend-palbstart)*i/(paldivs-1));
			}
			*/
			jwin_pal[jcBOX]	=dvc(4);
			jwin_pal[jcLIGHT]  =dvc(5);
			jwin_pal[jcMEDLT]  =dvc(4);
			jwin_pal[jcMEDDARK]=dvc(3);
			jwin_pal[jcDARK]   =dvc(2);
			jwin_pal[jcBOXFG]  =dvc(1);
			jwin_pal[jcTITLEL] =dvc(3);
			jwin_pal[jcTITLER] =dvc(3);
			jwin_pal[jcTITLEFG]=dvc(7);
			jwin_pal[jcTEXTBG] =dvc(5);
			jwin_pal[jcTEXTFG] =dvc(1);
			jwin_pal[jcSELBG]  =dvc(8);
			jwin_pal[jcSELFG]  =dvc(6);
		}
		break;
		
		default:  //Windows 2000
		{
			pal[dvc(1)] = _RGB(0*63/255,   0*63/255,   0*63/255);
			pal[dvc(2)] = _RGB(66*63/255,  65*63/255,  66*63/255);
			pal[dvc(3)] = _RGB(132*63/255, 130*63/255, 132*63/255);
			pal[dvc(4)] = _RGB(212*63/255, 208*63/255, 200*63/255);
			pal[dvc(5)] = _RGB(255*63/255, 255*63/255, 255*63/255);
			pal[dvc(6)] = _RGB(255*63/255, 255*63/255, 225*63/255);
			pal[dvc(7)] = _RGB(255*63/255, 225*63/255, 160*63/255);
			pal[dvc(8)] = _RGB(0*63/255,   0*63/255,  80*63/255);
			
			byte palrstart= 10*63/255, palrend=166*63/255,
				 palgstart= 36*63/255, palgend=202*63/255,
				 palbstart=106*63/255, palbend=240*63/255,
				 paldivs=7;
				 
			for(int32_t i=0; i<paldivs; i++)
			{
				pal[dvc(15-paldivs+1)+i].r = palrstart+((palrend-palrstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].g = palgstart+((palgend-palgstart)*i/(paldivs-1));
				pal[dvc(15-paldivs+1)+i].b = palbstart+((palbend-palbstart)*i/(paldivs-1));
			}
			
			jwin_pal[jcBOX]	=dvc(4);
			jwin_pal[jcLIGHT]  =dvc(5);
			jwin_pal[jcMEDLT]  =dvc(4);
			jwin_pal[jcMEDDARK]=dvc(3);
			jwin_pal[jcDARK]   =dvc(2);
			jwin_pal[jcBOXFG]  =dvc(1);
			jwin_pal[jcTITLEL] =dvc(9);
			jwin_pal[jcTITLER] =dvc(15);
			jwin_pal[jcTITLEFG]=dvc(7);
			jwin_pal[jcTEXTBG] =dvc(5);
			jwin_pal[jcTEXTFG] =dvc(1);
			jwin_pal[jcSELBG]  =dvc(8);
			jwin_pal[jcSELFG]  =dvc(6);
		}
		break;
	}
	
	if(!udef) //Auto-fill the indexes that aren't used in old styles
	{
		jwin_pal[jcCURSORMISC] = dvc(1);
		jwin_pal[jcCURSOROUTLINE] = dvc(2);
		jwin_pal[jcCURSORLIGHT] = dvc(3);
		jwin_pal[jcCURSORDARK] = dvc(5);
		jwin_pal[jcALT_TEXTFG] = jwin_pal[jcMEDDARK];
		jwin_pal[jcALT_TEXTBG] = jwin_pal[jcTEXTFG];
		jwin_pal[jcDISABLED_FG] = jwin_pal[jcMEDDARK];
		jwin_pal[jcDISABLED_BG] = jwin_pal[jcBOX];
	}
	
    gui_bg_color=jwin_pal[jcBOX];
    gui_fg_color=jwin_pal[jcBOXFG];
    
    jwin_set_colors(jwin_pal);
}

//MsgStr stuff
// Copy everything except listpos
MsgStr& MsgStr::operator=(MsgStr const& other)
{
	copyText(other);
	copyStyle(other);
	return *this;
}

// Copy text data - just s and nextstring
void MsgStr::copyText(MsgStr const& other)
{
	s.resize(other.s.size());
	s.assign(other.s.c_str());
	nextstring=other.nextstring;
}

// Copy style data - everything except s, nextstring, and listpos
void MsgStr::copyStyle(MsgStr const& other)
{
	tile=other.tile;
	cset=other.cset;
	trans=other.trans;
	font=other.font;
	x=other.x;
	y=other.y;
	w=other.w;
	h=other.h;
	sfx=other.sfx;
	vspace=other.vspace;
	hspace=other.hspace;
	stringflags=other.stringflags;
	for(int32_t q = 0; q < 4; ++q)
	{
		margins[q] = other.margins[q];
	}
	portrait_tile=other.portrait_tile;
	portrait_cset=other.portrait_cset;
	portrait_x=other.portrait_x;
	portrait_y=other.portrait_y;
	portrait_tw=other.portrait_tw;
	portrait_th=other.portrait_th;
	shadow_type=other.shadow_type;
	shadow_color=other.shadow_color;
	drawlayer=other.drawlayer;
}

void MsgStr::copyAll(MsgStr const& other)
{
	copyText(other);
	copyStyle(other);
	listpos = other.listpos;
}

void MsgStr::clear()
{
	s = "";
	s.shrink_to_fit();
	nextstring = 0;
	tile=0;
	cset=0;
	trans=0;
	font=0;
	x=0;
	y=0;
	w=0;
	h=0;
	sfx=0;
	vspace=0;
	hspace=0;
	stringflags=0;
	for(int32_t q = 0; q < 4; ++q)
	{
		margins[q] = 0;
	}
	portrait_tile=0;
	portrait_cset=0;
	portrait_x=0;
	portrait_y=0;
	portrait_tw=0;
	portrait_th=0;
	shadow_type=0;
	shadow_color=0;
	listpos=0;
	drawlayer=6;
}

bool valid_str(char const* ptr, char cancel)
{
	return ptr && ptr[0] && ptr[0] != cancel;
}

int32_t X_DIR(int32_t dir)
{
	dir = NORMAL_DIR(dir);
	if(dir < 0) return dir;
	return xDir[dir];
}
int32_t Y_DIR(int32_t dir)
{
	dir = NORMAL_DIR(dir);
	if(dir < 0) return dir;
	return yDir[dir];
}

direction XY_DELTA_TO_DIR(int32_t dx, int32_t dy)
{
	if (dx == 0 && dy == 0) return dir_invalid;
	if (dx == 1 && dy == 0) return right;
	if (dx == -1 && dy == 0) return left;
	if (dx == 0 && dy == 1) return down;
	if (dx == 0 && dy == -1) return up;
	return dir_invalid;
}

string get_dbreport_string()
{
	std::stringstream oss;
	
	char buf[256];
	
	oss << "```\n"
		<< ZQ_EDITOR_NAME
		<< "\nVersion: " << ZQ_EDITOR_V << " " << ALPHA_VER_STR
		<< "\nBuild: " << VERSION_BUILD;
		
	sprintf(buf,"Build Date: %s %s, %d at @ %s %s", dayextension(BUILDTM_DAY).c_str(),
		(char*)months[BUILDTM_MONTH], BUILDTM_YEAR, __TIME__, __TIMEZONE__);
	
	oss << "\n" << buf
		<< "\nDev Signoff: " << DEV_SIGNOFF
		<< "\nQR:" << get_qr_hexstr(quest_rules, true, false)
		<< "```";
	return oss.str();
}

string get_qr_hexstr(byte* qrs, bool hash, bool disctags)
{
	if(!qrs) qrs = quest_rules;
	std::ostringstream oss;
	if(hash)
	{
		if(disctags)
			oss << "`";
		oss << "##";
	}
	for (size_t i = 0; i < QUESTRULES_NEW_SIZE; ++i)
	{
		char hex_buf[3];
		sprintf(hex_buf, "%02X", qrs[i]);
		oss << hex_buf;
	}
	if(hash)
	{
		oss << "##";
		if(disctags)
			oss << "`";
	}
	return oss.str();
}

bool clipboard_has_text()
{
	return al_clipboard_has_text(all_get_display());
}
bool get_al_clipboard(std::string& clipboard)
{
	if(!clipboard_has_text()) return false;
	clipboard = al_get_clipboard_text(all_get_display());
	return true;
}
void set_al_clipboard(std::string const& clipboard)
{
	al_set_clipboard_text(all_get_display(), clipboard.c_str());
}

bool load_qr_hexstr(string hexstr)
{
	//Strings with '##' delimiting start/end of substring are valid
	size_t startpos = hexstr.find("##");
	if(startpos != string::npos)
	{
		startpos += 2; //cut out the '##'
		size_t endpos = hexstr.find("##", startpos); //find an end terminator
		if(endpos == string::npos)
			hexstr = hexstr.substr(startpos);
		else hexstr = hexstr.substr(startpos, endpos-startpos);
	}
	if(hexstr.size() != QUESTRULES_NEW_SIZE*2)
	{
		if(hexstr.find_first_not_of("0123456789ABCDEFabcdef") != string::npos)
		{
			return false;
		}
		else zprint2("Detected bad hexstr length '%d != %d'!\n", hexstr.size(), QUESTRULES_NEW_SIZE * 2);
	}
	size_t len = hexstr.size() / 2;
	if(len > QUESTRULES_NEW_SIZE) len = QUESTRULES_NEW_SIZE;
	for(size_t i = 0; i < len; ++i)
	{
		char hex_buf[3];
		sprintf(hex_buf, "%s", hexstr.substr(i*2,2).c_str());
		quest_rules[i] = (byte)zc_xtoi(hex_buf);
	}
	InfoDialog("QRs Loaded", "Quest Rules have been loaded from the clipboard").show();
	return true;
}
bool load_qr_hexstr_clipboard()
{
	if(!clipboard_has_text()) return false;
	char* clipboardstr = al_get_clipboard_text(all_get_display());
	if(!clipboardstr) return false;
	string str(clipboardstr);
	bool ret = load_qr_hexstr(str);
	al_free(clipboardstr);
	return true;
}

bool load_dev_info(string& devstr)
{
	if(devstr.empty())
		return false;
	if(!load_qr_hexstr(devstr))
		return false;
	removechar(devstr, '\r');
	size_t startchop = devstr.find("```\n");
	if(startchop == string::npos)
		startchop = 0;
	size_t endchop = devstr.find("\nQR:", startchop);
	InfoDialog("DEV About Data", devstr.substr(startchop,endchop-startchop)).show();
	return true;
}
bool load_dev_info_clipboard()
{
	if(!clipboard_has_text()) return false;
	char* clipboardstr = al_get_clipboard_text(all_get_display());
	if(!clipboardstr) return false;
	string str(clipboardstr);
	bool ret = load_dev_info(str);
	al_free(clipboardstr);
	return true;
}

string generate_zq_about()
{
	char buf1[256];
	std::ostringstream oss;
	sprintf(buf1,"%s (%s), Version: %s", ZQ_EDITOR_NAME,PROJECT_NAME,ZQ_EDITOR_V);
	oss << buf1 << '\n';
	sprintf(buf1, "%s, Build %d", ALPHA_VER_STR, VERSION_BUILD);
	oss << buf1 << '\n';
	sprintf(buf1,"Build Date: %s %s, %d at @ %s %s", dayextension(BUILDTM_DAY).c_str(), (char*)months[BUILDTM_MONTH], BUILDTM_YEAR, __TIME__, __TIMEZONE__);
	oss << buf1 << '\n';
	sprintf(buf1, "Built By: %s", DEV_SIGNOFF);
	oss << buf1 << '\n';
	
	return oss.str();
}
