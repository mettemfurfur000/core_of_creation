function smart_input( o_text_back, o_text_content )
	o_text_back.text_box.shown = string.len(o_text_content.text) == 0;
end

function init( window )
	window:loadMenu("LOGIN");
	return;
end