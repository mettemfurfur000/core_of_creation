function love_bclk ( mx, my, button )
	
	local ch = math.random(4)
	local str = button.text_part.text_string
	
	if ch == 1 then str = "i love you 3000 times ever" end
	if ch == 2 then str = "love love love i love" end
	if ch == 3 then str = "<3 love love <3" end
	if ch == 4 then str = "<3" end
	
	button.text_part.text_string = str
	
	button.text_part.updated = true
	
	return button
end