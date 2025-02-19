#include "scenes/board.hpp"
#include "cats_pics.hpp"

using namespace Astralbrew::Drawing;
using namespace Astralbrew::Video;

void Board::show_dialog(const char* actor_name, const char* message, void (*callback)(void*))
{	
	new_dialog = true;
	bgShow(0);
	bgShow(1);	
	
	set_dialog_character(CatsPics[actor_name].tiles, CatsPics[actor_name].palette);
	
	vwf_title.clear(Pal4bit);
	vwf_title.put_text(actor_name, Pal4bit, SolidColorBrush(0xE));
	
	dialog_stream = message;
	dialog_callback = callback;
}

void Board::hide_dialog()
{
	bgHide(0);
	bgHide(1);
}

bool Board::process_dialog()
{
	if(!dialog_stream) 
		return false;
	
	new_dialog = false;
	vwf_body.clear(Pal4bit);
	int count = vwf_body.put_text(dialog_stream, Pal4bit, SolidColorBrush(0xF));		
	dialog_stream+=count;		
	
	if(count==0) 
	{
		dialog_stream=nullptr;		
		hide_dialog();
		if(dialog_callback)
		{				
			dialog_callback(this);	
			if(!new_dialog)
				dialog_callback = 0;				
		}		
		return false;
	}
	else
	{
		wait_for_key_pressed(KEY_A);			
		return true;
	}
}

void Board::set_dialog_character(const void* tiles, const void* palette)
{
	if(tiles==nullptr)
	{
		for(int i=0;i<16;i++) BG_PALETTE[0xA0+i]=0;
		return;
	}
	dmaCopy(palette, BG_PALETTE_BANK(0xA), 32);
	dmaCopy(tiles, char_pic_addr.get_value(), 4*4*32);
}