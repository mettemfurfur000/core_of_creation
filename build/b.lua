function b_bclk ( mx, my, button )
	local shape = button.text_part.text_box.position.shape
	local real_rect = button.text_part.text_box.position.real_rect
	
	local range = 20
	
	local dx = mx - real_rect.x
	local dy = my - real_rect.y
	
	if( dx > 0 and dx < range) then
		shape.w = shape.w - 10
	end
	
	if( dy > 0 and dy < range) then
		shape.h = shape.h - 10
	end
	
	if( dx > shape.w - range and dx < shape.w) then
		shape.w = shape.w + 10
	end
	
	if( dy > shape.h - range and dy < shape.h) then
		shape.h = shape.h + 10
	end

	return button
end