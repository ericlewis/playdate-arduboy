struct Crate: Solid
{
	Crate() = default;

	Crate(Point spawn)
	{
		x = spawn.x, y = spawn.y;
		width = 16, height = 16;
		cbox_conf = Rect(0, 10, 16, 6);
		type = 'C';
	}

	void update()
	{
		updateCbox();
	}

	void draw() const
	{
		sketch(CRATE);
	}
};
