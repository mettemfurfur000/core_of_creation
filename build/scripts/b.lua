b_hide = 1
b_moving = 0

function spec_round(num) 
    if num >= 0 then return math.ceil(num+.5) 
    else return math.floor(num-.5) end
end

function smt_mov( from, dest , div)
	return from + spec_round( (dest - from) / div );
end

function smooth_move_step( position )
	local dest_x = 0
	local dest_y = 0
	local div = 15

	if( b_hide == 1 )
	then
		dest_x = -position.shape.w
	end
	
	local need_move_x = not(dest_x - position.shape.x == 0)
	local need_move_y = not(dest_y - position.shape.y == 0)

	if((not need_move_x) and (not need_move_y))
	then
		b_moving = 0 
		return position
	end

	if(need_move_x == true)
	then
		position.shape.x = smt_mov( position.shape.x , dest_x, div );
	end

	if(need_move_y == true)
	then
		position.shape.y = smt_mov( position.shape.y , dest_y, div );
	end

	return position
end

function b_tick( window )
	if( b_moving == 0 )
	then 
		return 
	end

	local test_menu = window:getMenu("TEST_MENU");

	test_menu.menu_box.pos = smooth_move_step( test_menu.menu_box.pos );

end

function b_bclk ( last_mouse_pos, button, window )
	local test_menu = window:getMenu("TEST_MENU");
	test_menu.shown = true;
	
	b_hide = 1 - b_hide;
	
	b_moving = 1;
	
	return button;
end