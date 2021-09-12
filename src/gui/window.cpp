#include "window.h"
#include "common.h"
#include "dialog.h"
#include "dialog_runner.h"
#include "../jwin.h"
#include "../zquest.h"
#include <algorithm>
#include <cassert>
#include <utility>

using std::max, std::shared_ptr;

namespace GUI
{

Window::Window(): content(nullptr), title(""), closeMessage(-1)
{
	hPadding=0;
	vPadding=0;
}

void Window::setTitle(std::string newTitle)
{
	title=std::move(newTitle);
	if(alDialog)
		alDialog->dp=title.data();
}

void Window::setContent(shared_ptr<Widget> newContent) noexcept
{
	content=std::move(newContent);
}

void Window::applyVisibility(bool visible)
{
	if(alDialog)
	{
		if(visible)
			alDialog->flags&=~D_HIDDEN;
		else
			alDialog->flags|=D_HIDDEN;
	}
	if(content)
		content->applyVisibility(visible);
}

void Window::calculateSize()
{
	if(content)
	{
		// Sized to content plus a bit of padding and space for the title bar.
		content->calculateSize();
		if(is_large)
		{
			setPreferredWidth(Size::pixels(max(
				content->getTotalWidth()+8,
				text_length(lfont, title.c_str())+20)));
			setPreferredHeight(Size::pixels(content->getTotalHeight()+36));
		}
		else
		{
			setPreferredWidth(Size::pixels(max(
				content->getTotalWidth()+12,
				text_length(lfont, title.c_str())+30)));
			setPreferredHeight(Size::pixels(content->getTotalHeight()+30));
		}
	}
	else
	{
		// No content, so whatever.
		setPreferredWidth(320_px);
		setPreferredHeight(240_px);
	}
}

void Window::arrange(int contX, int contY, int contW, int contH)
{
	// This will limit the window to the available space.
	// (But for now, at least, we're assuming it'll fit.)
	Widget::arrange(contX, contY, contW, contH);
	if(content)
	{
		// Then arrange the content with the final size.
		if(is_large)
			content->arrange(x+6, y+28, getWidth()-12, getHeight()-36);
		else
			content->arrange(x+4, y+26, getWidth()-8, getHeight()-30);
	}
}

void Window::realize(DialogRunner& runner)
{
	alDialog=runner.push(shared_from_this(), DIALOG {
		jwin_win_proc,
		x, y, getWidth(), getHeight(),
		fgColor, bgColor,
		0, // key
		getFlags()|(closeMessage>=0 ? D_EXIT : 0), // flags,
		0, 0, // d1, d2
		title.data(), lfont, nullptr // dp, dp2, dp3
	});

	if(content)
		content->realize(runner);

	realizeKeys(runner);
}

int Window::onEvent(int event, MessageDispatcher& sendMessage)
{
	if(event==ngeCLOSE)
	{
		if(closeMessage>=0)
			sendMessage(closeMessage, MessageArg::none);
		return -1;
	}

	return TopLevelWidget::onEvent(event, sendMessage);
}

}
