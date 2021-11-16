#include "bottleshop.h"
#include "common.h"
#include "info.h"
#include <gui/builder.h>
#include <boost/format.hpp>

extern bool saved;
extern miscQdata misc;

void call_bottleshop_dlg(int32_t index)
{
	if(index < 0 || index > 255) return;
	BottleShopDialog(index).show();
}

BottleShopDialog::BottleShopDialog(int32_t index):
	index(index), sourceBottleShop(misc.bottle_shop_types[index]),
	tempBottleShop(misc.bottle_shop_types[index]),
	list_bottletypes(GUI::ListData::bottletype())
{}

#define NUM_FIELD(member,_min,_max) \
TextField( \
	type = GUI::TextField::type::INT_DECIMAL, \
	low = _min, high = _max, val = tempBottleShop.member, \
	onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val) \
	{ \
		tempBottleShop.member = val; \
	})

std::shared_ptr<GUI::Widget> BottleShopDialog::view()
{
	using namespace GUI::Builder;
	using namespace GUI::Key;
	using namespace GUI::Props;
	
	char titlebuf[256];
	sprintf(titlebuf, "Bottle Shop (%d): %s", index, tempBottleShop.name);
	window = Window(
		title = titlebuf,
		minwidth = 30_em,
		info = "Bottle Types to be used by the Bottle itemclass. Bottles can be filled"
			" with various things, configurable here.\nThese can be set to restore up"
			" to 3 counters, either by a value or a percentage; and also set what will"
			" remain in the bottle after this type is consumed (ex. multi-use potions).",
		onClose = message::CANCEL,
		Column(
			Row(
				Label(text = "Name:", hAlign = 0.0),
				TextField(
					maxwidth = 15_em,
					maxLength = 31,
					text = tempBottleShop.name,
					onValChangedFunc = [&](GUI::TextField::type,std::string_view str,int32_t)
					{
						std::string name(str);
						strncpy(tempBottleShop.name, name.c_str(), 31);
						char buf[256];
						sprintf(buf, "Bottle Shop (%d): %s", index, name.c_str());
						window->setTitle(buf);
					}
				)
			),
			Rows<4>(
				Label(text = "Fill:"),
				Label(text = "Price:"),
				Label(text = "Info:"),
				Label(text = "Display:", hAlign = 0.0),
				//{
				DropDownList(data = list_bottletypes,
					fitParent = true,
					selectedValue = tempBottleShop.fill[0],
					onSelectFunc = [&](int32_t val)
					{
						tempBottleShop.fill[0] = val;
					}
				),
				NUM_FIELD(price[0],0,65535),
				NUM_FIELD(str[0],0,65535),
				SelComboSwatch(
					combo = tempBottleShop.comb[0],
					cset = tempBottleShop.cset[0],
					onSelectFunc = [&](int32_t cmb, int32_t c)
					{
						tempBottleShop.comb[0] = cmb;
						tempBottleShop.cset[0] = c;
					}
				),
				//}
				//{
				DropDownList(data = list_bottletypes,
					fitParent = true,
					selectedValue = tempBottleShop.fill[1],
					onSelectFunc = [&](int32_t val)
					{
						tempBottleShop.fill[1] = val;
					}
				),
				NUM_FIELD(price[1],0,65535),
				NUM_FIELD(str[1],0,65535),
				SelComboSwatch(
					combo = tempBottleShop.comb[1],
					cset = tempBottleShop.cset[1],
					onSelectFunc = [&](int32_t cmb, int32_t c)
					{
						tempBottleShop.comb[1] = cmb;
						tempBottleShop.cset[1] = c;
					}
				),
				//}
				//{
				DropDownList(data = list_bottletypes,
					fitParent = true,
					selectedValue = tempBottleShop.fill[2],
					onSelectFunc = [&](int32_t val)
					{
						tempBottleShop.fill[2] = val;
					}
				),
				NUM_FIELD(price[2],0,65535),
				NUM_FIELD(str[2],0,65535),
				SelComboSwatch(
					combo = tempBottleShop.comb[2],
					cset = tempBottleShop.cset[2],
					onSelectFunc = [&](int32_t cmb, int32_t c)
					{
						tempBottleShop.comb[2] = cmb;
						tempBottleShop.cset[2] = c;
					}
				)
				//}
			),
			Row(
				vAlign = 1.0,
				spacing = 2_em,
				Button(
					focused = true,
					text = "OK",
					onClick = message::OK),
				Button(
					text = "Cancel",
					onClick = message::CANCEL)
			)
		)
	);
	return window;
}

bool BottleShopDialog::handleMessage(const GUI::DialogMessage<message>& msg)
{
	switch(msg.message)
	{
		case message::OK:
			memcpy(&sourceBottleShop, &tempBottleShop, sizeof(tempBottleShop));
			saved = false;
			return true;

		case message::CANCEL:
		default:
			return true;
	}
}